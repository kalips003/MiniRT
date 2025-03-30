/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D2_dist_plane.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/11 11:47:30 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			distance_from_plane(t_c_px *calcul, void *obj, int simple);
static int	h_dist_plane(t_c_px *calcul, t_plane *plane, t_c_plane *c, int simple);
static void	h_img_plane(t_c_px *calcul, t_c_plane *c, t_plane *plane);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
int	distance_from_plane(t_c_px *calcul, void *obj, int simple)
{
	t_c_plane	c;
	t_plane			*plane;

	plane = (t_plane *)obj;
	c.top = -(plane->O.view.dx * calcul->c0.x + plane->O.view.dy * calcul->c0.y + plane->O.view.dz * calcul->c0.z + plane->d);
	c.bot = plane->O.view.dx * calcul->v.dx + plane->O.view.dy * calcul->v.dy + plane->O.view.dz * calcul->v.dz;
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (0);
	c.dist = c.top / c.bot;
	if (c.dist <= EPSILON)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		return (h_dist_plane(calcul, plane, &c, simple));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
static int	h_dist_plane(t_c_px *calcul, t_plane *plane, t_c_plane *c, int simple)
{
	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = plane;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	calcul->mat = *(t_mat2 *)&plane->param;
	calcul->vn = plane->O.view;
	if (c->bot > 0.0)
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
	if (plane->param.c2.r >= 0 || plane->param.txt || plane->param.n_map || plane->param.a_map)
		h_img_plane(calcul, c, plane);
	if (!plane->param.txt && plane->param.c2.r >= 0)
		if (c->o_to_inter.dx)
			calcul->mat.argb = (t_argb){calcul->mat.argb.a, plane->param.c2.r, plane->param.c2.g, plane->param.c2.b};
	return (1);
}

static void	h_img_plane(t_c_px *calcul, t_c_plane *c, t_plane *plane)
{
	t_vect	normal_map;
	t_obj	local;

	c->o_to_inter = vect_ab(&plane->O.c0, &calcul->inter);
	c->u = ft_dot_p(&c->o_to_inter, &plane->O.right) / plane->param.gamma;
	c->v = 1.0 - ft_dot_p(&c->o_to_inter, &plane->O.up) / plane->param.gamma;
	c->o_to_inter.dx = (((int)floor(c->u) + (int)floor(c->v)) % 2);
	c->u = c->u - floor(c->u);
	c->v = c->v - floor(c->v);
	if (plane->param.txt)
		calcul->mat.argb = return_px_img(plane->param.txt, c->u, c->v);
	if (plane->param.a_map)
		calcul->mat.argb.a = return_alpha_img(plane->param.a_map, c->u, c->v);
	if (plane->param.n_map)
	{
		normal_map = return_vect_img(plane->param.n_map, c->u, c->v);
		local.view = calcul->vn;
		local.right = plane->O.right;
		local.up = plane->O.up;
		calcul->vn = mult_3x3_vect(&local, &calcul->vn);
		// calcul->vn = (t_vect){
		// 	plane->O.right.dx * normal_map.dx + plane->O.up.dx * normal_map.dy + calcul->vn.dx * normal_map.dz,
		// 	plane->O.right.dy * normal_map.dx + plane->O.up.dy * normal_map.dy + calcul->vn.dy * normal_map.dz,
		// 	plane->O.right.dz * normal_map.dx + plane->O.up.dz * normal_map.dy + calcul->vn.dz * normal_map.dz,
		// };
		ft_normalize_vect(&calcul->vn);
	}
}
