/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D3_dist_cylinder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:07:55 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 14:34:30 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			distance_from_cylinder(t_c_px *calcul, void *obj, int simple);
static int	h_dist_cylinder_calc(t_c_px *c1, t_cylinder *cy, t_c_cy *c2);
static int	h_dist_cylinder(t_c_px *calcul, t_cylinder *cylinder, t_c_cy *c, int simple);
static void	h_img_cylinder(t_c_px *calcul, t_cylinder *cylinder, t_c_cy *c);

///////////////////////////////////////////////////////////////////////////////]
int	distance_from_cylinder(t_c_px *calcul, void *obj, int simple)
{
	t_c_cy		c;
	t_cylinder	*cy;
	int			rtrn;

	cy = (t_cylinder *)obj;
	c.circle = (t_circle){CIRCLE, cy->O, cy->param, cy->radius};
	rtrn = distance_from_circle(calcul, &c.circle, simple);
	c.circle.O.c0 = new_moved_point(&cy->O.c0, &cy->O.view, cy->height);
	c.circle.O.view = scale_vect(c.circle.O.view, -1.0);
	rtrn |= distance_from_circle(calcul, &c.circle, simple);
	if (rtrn && simple)
		return (1);
	if (rtrn)
	{
		calcul->object = obj;
		calcul->mat = *(t_mat2 *)&cy->param;
	}
	if (!h_dist_cylinder_calc(calcul, cy, &c))
		return (rtrn - (rtrn == 3));
	if (c.dist < calcul->dist || calcul->dist < 0)
		return (h_dist_cylinder(calcul, cy, &c, simple));
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// fills the t_cone_calc_v2 with intersection point
// if no intersection, return 0
static int	h_dist_cylinder_calc(t_c_px *ca, t_cylinder *cy, t_c_cy *c2)
{
	t_c_cyl2	c;

	c.A = ft_dot_p(&ca->v, &cy->O.view);
	c.B = cy->O.view.dx * (ca->c0.x - cy->O.c0.x) + cy->O.view.dy * (ca->c0.y \
		- cy->O.c0.y) + cy->O.view.dz * (ca->c0.z - cy->O.c0.z);
	c.x0 = ca->v.dx - c.A * cy->O.view.dx;
	c.y0 = ca->v.dy - c.A * cy->O.view.dy;
	c.z0 = ca->v.dz - c.A * cy->O.view.dz;
	c.x1 = ca->c0.x - c.B * cy->O.view.dx - cy->O.c0.x;
	c.y1 = ca->c0.y - c.B * cy->O.view.dy - cy->O.c0.y;
	c.z1 = ca->c0.z - c.B * cy->O.view.dz - cy->O.c0.z;
	c.a = c.x0 * c.x0 + c.y0 * c.y0 + c.z0 * c.z0;
	c.b = 2 * (c.x0 * c.x1 + c.y0 * c.y1 + c.z0 * c.z1);
	c.c = c.x1 * c.x1 + c.y1 * c.y1 + c.z1 * c.z1 - cy->radius * cy->radius;
	c.delta = c.b * c.b - 4 * c.a * c.c;
	if (c.delta < EPSILON || fabs(c.a) < EPSILON)
		return (0);
	c2->det1 = (-c.b + sqrt(c.delta)) / (2 * c.a);
	c2->det2 = (-c.b - sqrt(c.delta)) / (2 * c.a);
	c2->dist = h_smalest_delta(c2->det1, c2->det2);
	c2->dist_h = c.A * c2->dist + c.B;
	if (c2->dist < EPSILON || c2->dist_h > cy->height || c2->dist_h < EPSILON)
		return (0);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
static int	h_dist_cylinder(t_c_px *calcul, t_cylinder *cylinder, t_c_cy *c, int simple)
{
	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = cylinder;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	c->projec_p = new_moved_point(&cylinder->O.c0, &cylinder->O.view, \
		c->dist_h);
	calcul->vn = vect_ab_norm(&c->projec_p, &calcul->inter);
	calcul->mat = *(t_mat2 *)&cylinder->param;
	if (!cylinder->param.txt && cylinder->param.c2.r >= 0)
		calcul->mat.argb = dual_color(&cylinder->param.argb, \
			&cylinder->param.c2, c->dist_h / cylinder->height);
	c->in = 1;
	if (c->det1 < EPSILON || c->det2 < EPSILON)
	{
		c->in = -1;
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
	}
	if (is_there_txt(&cylinder->param))
		h_img_cylinder(calcul, cylinder, c);
	return (1 + (c->in == -1));
}

///////////////////////////////////////////////////////////////////////////////]
// 		ATAN2 [−π,π][−π,π] > [0,1].
// 		cosϕ=[1top,-1bot]	ACOS [0,π] > [0,1].
static void	h_img_cylinder(t_c_px *calcul, t_cylinder *cylinder, t_c_cy *c)
{
	double	u;
	double	h;
	t_vect	normal_map;
	t_obj	local;

	u = fmin(1.0, fmax(0.0, atan2(ft_dot_p(&calcul->vn, &cylinder->O.right) \
		* c->in, ft_dot_p(&calcul->vn, &cylinder->O.up) * c->in) \
		/ (2 * PI) + 0.5));
	h = 1.0 - c->dist_h / cylinder->height;
	update_mat_w_txt(calcul, (t_obj2 *)cylinder, u, h);
	if (cylinder->param.n_map)
	{
		normal_map = return_vect_img(cylinder->param.n_map, u, h);
		local.view = calcul->vn;
		local.up = cylinder->O.view;
		local.right = ft_cross_product_norm(&local.up, &local.view);
		normal_map.dx *= c->in;
		calcul->vn = mult_3x3_vect(&local, &calcul->vn);
		ft_normalize_vect(&calcul->vn);
	}
}
