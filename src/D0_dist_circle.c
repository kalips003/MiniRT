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

int			distance_from_circle(t_calcul_px *calcul, void *obj, int simple);
static int	h_dist_circle(t_calcul_px *calcul, t_circle_calc *c, t_circle *circle, int simple);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// RESOLVE PLANE EQUATION: A(t.Vx + EYEx) + B(t.Vy + EYEy) + C(t.Vz + EYEz) + D = 0
// ==> t = top / bot;
// if top = 0, the camera is on the plane
// if bot = 0, the view_vector is parallele to d plane
int	distance_from_circle(t_calcul_px *calcul, void *obj, int simple)
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

static int	h_dist_circle(t_calcul_px *calcul, t_circle_calc *c, t_circle *circle, int simple)
{
	if (simple)
		return (1);
	calcul->object = circle;
	calcul->dist = c->dist;
	calcul->inter = c->inter_temp;
	calcul->mat = *(t_mat *)&circle->param;
	if (circle->param.color2.r >= 0)
		calcul->mat.argb = dual_color_render(&circle->param.argb, &circle->param.color2, c->dist_center / circle->radius);
	calcul->vn = circle->O.view;
	if (ft_dot_product(&calcul->v, &circle->O.view) > 0.0)
	{
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
		return (1);
	}
	return (3);
}
