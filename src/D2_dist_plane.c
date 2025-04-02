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
	c->in = 0;
	if (c->bot > EPSILON)
	{
		c->in = 1;
		if (calcul->print == 1)
			printf(C_321"calcul->vn before: [%.3f,%.3f,%.3f]\n", calcul->vn.dx, calcul->vn.dy, calcul->vn.dz);
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
		if (calcul->print == 1)
			printf(C_123"calcul->vn after: [%.3f,%.3f,%.3f]\n", calcul->vn.dx, calcul->vn.dy, calcul->vn.dz);
	
	}
	h_img_plane(calcul, c, plane);
	if (calcul->print == 1)
	{
		printf("c->bot: %.3f, we inverse? %d\n", c->bot, !!(c->bot > EPSILON));
		printf("calcul->vn: [%.3f,%.3f,%.3f]\n", calcul->vn.dx, calcul->vn.dy, calcul->vn.dz);
	}
	return (1);
}

static void	h_img_plane(t_c_px *calcul, t_c_plane *c, t_plane *plane)
{
	t_vect	normal_map;
	t_obj	local;

	c->u = c->u - floor(c->u);
	c->v = c->v - floor(c->v);
	update_mat_w_txt(calcul, (t_obj2 *)plane, c->u, c->v);
	if (plane->param.n_map)
	{
		normal_map = return_vect_img(plane->param.n_map, c->u, c->v);
		local.up = plane->O.up;
		local.right = plane->O.right;
		local.view = calcul->vn;
		// if (c->in == 1)
		// 	normal_map.dx *= -1;
		if (calcul->print == 1)
		{
			t_argb a = return_px_img(plane->param.n_map, c->u, c->v);
			printf("c->in? %d; uv: [%.3f,%.3f]\n", c->in, c->u, c->v);
			printf(C_413"normal_map? [%.3f,%.3f,%.3f]\n", normal_map.dx, normal_map.dy, normal_map.dz);
			printf("argb: [%d,%d,%d,%d]\n", a.a, a.r, a.g, a.b);
			local.c0 = calcul->inter;
			// h_render_v_space_2(calcul->data, &local);
		}
		calcul->vn = mult_3x3_vect(&local, &normal_map);
		ft_normalize_vect(&calcul->vn);
	}
}
