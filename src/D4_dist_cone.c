/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D4_dist_cone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 14:53:05 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			distance_from_cone(t_c_px *calcul, void *obj, int simple);
static int	h_dist_cone_calc(t_c_px *c1, t_cone *cone, t_cone_calc_v2 *c2);
static int	h_colision_cone(t_c_px *calcul, t_cone *cone, t_cone_calc_v1 *c, t_cone_calc_v2 *c2);
static int	h_dist_cone(t_c_px *calcul, t_cone *cone, t_cone_calc_v2 *c, int simple);
static void	h_img_cone(t_c_px *calcul, t_cone *cone, t_cone_calc_v2 *c);

///////////////////////////////////////////////////////////////////////////////]
int	distance_from_cone(t_c_px *calcul, void *obj, int simple)
{
	t_cone_calc_v2	c;
	t_cone			*cone;
	int				rtrn;

	cone = (t_cone *)obj;
	c.circle = (t_circle){CIRCLE, cone->O, cone->param, cone->radius};
	rtrn = distance_from_circle(calcul, &c.circle, simple);
	if (rtrn && simple)
		return (1);
	if (rtrn)
	{
		calcul->object = obj;
		calcul->mat = *(t_mat2 *)&cone->param;
	}
	if (!h_dist_cone_calc(calcul, cone, &c))
		return (rtrn - (rtrn == 3));
	if (c.dist < calcul->dist || calcul->dist < 0)
		return (h_dist_cone(calcul, cone, &c, simple));
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// fills the t_cone_calc_v2 with intersection point
// if no intersection, return 0
static int	h_dist_cone_calc(t_c_px *ca, t_cone *co, t_cone_calc_v2 *c1)
{
	t_cone_calc_v1	c;

	c.p = ft_dot_p(&ca->v, &co->O.view);
	c.q = co->O.view.dx * (ca->c0.x - co->apex.x) + co->O.view.dy * (ca->c0.y \
		- co->apex.y) + co->O.view.dz * (ca->c0.z - co->apex.z);
	c.a1 = ca->v.dx - c.p * co->O.view.dx;
	c.a2 = ca->v.dy - c.p * co->O.view.dy;
	c.a3 = ca->v.dz - c.p * co->O.view.dz;
	c.b1 = ca->c0.x - co->apex.x - c.q * co->O.view.dx;
	c.b2 = ca->c0.y - co->apex.y - c.q * co->O.view.dy;
	c.b3 = ca->c0.z - co->apex.z - c.q * co->O.view.dz;
	c.A = c.a1 * c.a1 + c.a2 * c.a2 + c.a3 * c.a3 - co->slope * c.p * c.p;
	c.B = 2 * (c.a1 * c.b1 + c.a2 * c.b2 + c.a3 * c.b3 - c.p * c.q * co->slope);
	c.C = c.b1 * c.b1 + c.b2 * c.b2 + c.b3 * c.b3 - c.q * c.q * co->slope;
	c.delta = c.B * c.B - 4 * c.A * c.C;
	if (c.delta < EPSILON || fabs(c.A) < EPSILON)
		return (0);
	c.det1 = (-c.B + sqrt(c.delta)) / (2 * c.A);
	c.det2 = (-c.B - sqrt(c.delta)) / (2 * c.A);
	c.d_a1 = -c.q + -c.p * c.det1;
	c.d_a2 = -c.q + -c.p * c.det2;
	if (!h_colision_cone(ca, co, &c, c1))
		return (0);
	c1->dist_apex = -c.q - c.p * c1->dist;
	return (1);
}

static int	h_colision_cone(t_c_px *calcul, t_cone *cone, t_cone_calc_v1 *c, t_cone_calc_v2 *c2)
{
	int		d1_valid;
	int		d2_valid;
	double	dot;

	c2->apex_to_camera = vect_ab_norm(&cone->apex, &calcul->c0);
	dot = -ft_dot_p(&cone->O.view, &c2->apex_to_camera);
	if (-c->q > EPSILON && -(c->q) < cone->h && dot > EPSILON && dot \
		> cos(cone->angle))
		c2->inside = 1;
	else
		c2->inside = 0;
	d1_valid = (c->det1 > EPSILON && c->d_a1 < cone->h && c->d_a1 > EPSILON);
	d2_valid = (c->det2 > EPSILON && c->d_a2 < cone->h && c->d_a2 > EPSILON);
	if (d1_valid && d2_valid)
		c2->dist = fmin(c->det1, c->det2);
	else if (d1_valid)
		c2->dist = c->det1;
	else if (d2_valid)
		c2->dist = c->det2;
	else
		return (0);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
// if closest object, update t_calcul
static int	h_dist_cone(t_c_px *calcul, t_cone *cone, t_cone_calc_v2 *c, int simple)
{
	t_vect	axis;

	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = cone;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	c->projec_point = new_moved_point(&cone->apex, &cone->O.view, \
		-c->dist_apex);
	calcul->vn = vect_ab_norm(&c->projec_point, &calcul->inter);
	axis = ft_cross_product(&calcul->vn, &cone->O.view);
	calcul->vn = quaternion_rota(&calcul->vn, &axis, cone->angle, 1);
	if (c->inside)
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
	calcul->mat = *(t_mat2 *)&cone->param;
	c->c_height = 1.0 - (cone->h - c->dist_apex) / cone->h;
	if (!cone->param.txt && cone->param.c2.r >= 0)
		calcul->mat.argb = dual_color(&cone->param.argb, &cone->param.c2, \
			c->c_height);
	if (is_there_txt(&cone->param))
		h_img_cone(calcul, cone, c);
	return (1 + c->inside);
}

///////////////////////////////////////////////////////////////////////////////]
// 		ATAN2 [−π,π][−π,π] > [0,1].
// 		cosϕ=[1top,-1bot]	ACOS [0,π] > [0,1].
static void	h_img_cone(t_c_px *calcul, t_cone *cone, t_cone_calc_v2 *c)
{
	int		in;
	double	u;
	t_vect	normal_map;
	t_obj	local;

	in = (1 - 2 * c->inside);
	u = fmin(1.0, fmax(0.0, atan2(ft_dot_p(&calcul->vn, &cone->O.right) * in, \
		ft_dot_p(&calcul->vn, &cone->O.up) * in) / (2 * PI) + 0.5));
	update_mat_w_txt(calcul, (t_obj2 *)cone, u, c->c_height);
	if (cone->param.n_map)
	{
		normal_map = return_vect_img(cone->param.n_map, u, c->c_height);
		local.view = calcul->vn;
		local.right = ft_cross_product_norm(&cone->O.view, &local.view);
		local.up = ft_cross_product_norm(&local.view, &local.right);
		normal_map.dx *= in;
		calcul->vn = mult_3x3_vect(&local, &calcul->vn);
		ft_normalize_vect(&calcul->vn);
	}
}
