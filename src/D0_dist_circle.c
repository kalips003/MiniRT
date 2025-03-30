/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D0_dist_circle.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/11 11:45:12 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			distance_from_circle(t_c_px *calcul, void *obj, int simple);
static int	h_dist_circle(t_c_px *calcul, t_circle_calc *c, t_circle *circle, int simple);
static void	h_img_circle(t_c_px *ca, t_circle *circle, t_circle_calc *c);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// RESOLVE PLANE EQUATION: A(t.Vx + EYEx) + B(t.Vy + EYEy) + C(t.Vz + EYEz) + D = 0
// ==> t = top / bot;
// if top = 0, the camera is on the plane
// if bot = 0, the view_vector is parallele to d plane
int	distance_from_circle(t_c_px *calcul, void *obj, int simple)
{
	t_circle_calc	c;
	t_circle		*circle;

	circle = (t_circle *)obj;
	c.pl_cst = -(circle->O.view.dx * circle->O.c0.x + circle->O.view.dy * circle->O.c0.y + circle->O.view.dz * circle->O.c0.z);
	c.top = -(circle->O.view.dx * calcul->c0.x + circle->O.view.dy * calcul->c0.y + circle->O.view.dz * calcul->c0.z + c.pl_cst);
	c.bot = circle->O.view.dx * calcul->v.dx + circle->O.view.dy * calcul->v.dy + circle->O.view.dz * calcul->v.dz;
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (0);
	c.dist = c.top / c.bot;
	if (c.dist <= 0.0)
		return (0);
	c.inter_temp = new_moved_point(&calcul->c0, &calcul->v, c.dist);
	c.dist_center = dist_two_points(&c.inter_temp, &circle->O.c0);
	if (c.dist_center > circle->radius)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		return (h_dist_circle(calcul, &c, circle, simple));
	return (0);
}

static int	h_dist_circle(t_c_px *calcul, t_circle_calc *c, t_circle *circle, int simple)
{
	if (simple)
		return (1);
	calcul->object = circle;
	calcul->dist = c->dist;
	calcul->inter = c->inter_temp;
	calcul->mat = *(t_mat2 *)&circle->param;
	if (circle->param.c2.r >= 0)
		calcul->mat.argb = dual_color(&circle->param.argb, &circle->param.c2, c->dist_center / circle->radius);
	calcul->vn = circle->O.view;
	if (ft_dot_p(&calcul->v, &circle->O.view) > 0.0)
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
	if (circle->param.txt || circle->param.n_map || circle->param.a_map \
		|| circle->param.ao_map || circle->param.s_map)
		h_img_circle(calcul, circle, c);
	return (1);
}


///////////////////////////////////////////////////////////////////////////////]
// 		ATAN2 [−π,π][−π,π] > [0,1].
// 		cosϕ=[1top,-1bot]	ACOS [0,π] > [0,1].
static void	h_img_circle(t_c_px *ca, t_circle *circle, t_circle_calc *c)
{
	t_vect	normal_map;
	t_obj	local;
	double	u;
	double	v;

	normal_map = vect_ab_norm(&circle->O.c0, &c->inter_temp);
	v = fmin(1.0, fmax(0.0, atan2(ft_dot_p(&normal_map, &circle->O.right), \
		ft_dot_p(&normal_map, &circle->O.up))  / (2 * PI) + 0.5));
	u = c->dist_center / circle->radius;
	update_mat_w_txt(ca, (t_obj2 *)circle, u, v);
	if (circle->param.n_map)
	{
		normal_map = return_vect_img(circle->param.n_map, u, v);
		local.view = ca->vn;
		local.right = circle->O.right;
		local.up = circle->O.up;
		ca->vn = mult_3x3_vect(&local, &ca->vn);
		ft_normalize_vect(&ca->vn);
	}
}
