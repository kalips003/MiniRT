/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D2_dist_plane.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 16:04:43 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			distance_from_plane(t_c_px *calcul, void *obj, int simple);
static int	h_dist_plane(t_c_px *calcul, t_plane *plane, t_c_plane *c, \
	int simple);
static void	h_img_plane(t_c_px *calcul, t_c_plane *c, t_plane *plane);

///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
int	distance_from_plane(t_c_px *calcul, void *obj, int simple)
{
	t_c_plane	c;
	t_plane		*plane;

	plane = (t_plane *)obj;
	c.top = -(ft_dot_p(&plane->O.view, (t_vect *)&calcul->c0) + plane->d);
	c.bot = ft_dot_p(&plane->O.view, &calcul->v);
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (0);
	c.dist = c.top / c.bot;
	if (c.dist <= EPSILON)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0)
		return (h_dist_plane(calcul, plane, &c, simple));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
static int	h_dist_plane(t_c_px *calcul, t_plane *plane, t_c_plane *c, \
	int simple)
{
	if (simple)
		return (1);
	calcul->object = (t_obj2 *)plane;
	calcul->dist = c->dist;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	calcul->mat = *(t_mat2 *)&plane->param;
	calcul->vn = plane->O.view;
	if (is_there_txt(&plane->param) || plane->param.c2.r >= 0)
	{
		c->o_to_inter = vect_ab(&plane->O.c0, &calcul->inter);
		c->u = ft_dot_p(&c->o_to_inter, &plane->O.right) / plane->param.gamma;
		c->v = 1.0 - ft_dot_p(&c->o_to_inter, &plane->O.up) / \
			plane->param.gamma;
	}
	if (plane->param.c2.r >= 0)
		if (((int)floor(c->u) + (int)floor(c->v)) % 2)
			calcul->mat.argb = (t_argb){calcul->mat.argb.a, plane->param.c2.r, \
				plane->param.c2.g, plane->param.c2.b};
	h_img_plane(calcul, c, plane);
	if (c->bot > EPSILON)
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
	return (1);
}

static void	h_img_plane(t_c_px *calcul, t_c_plane *c, t_plane *plane)
{
	t_vect	normal_map;

	c->u = c->u - floor(c->u);
	c->v = c->v - floor(c->v);
	update_mat_w_txt(calcul, (t_obj2 *)plane, c->u, c->v);
	if (plane->param.n_map)
	{
		normal_map = return_vect_img(plane->param.n_map, c->u, c->v);
		normal_map.dy *= -1.0;
		calcul->vn = mult_3x3_vect(&plane->O, &normal_map);
		ft_normalize_vect(&calcul->vn);
	}
}
