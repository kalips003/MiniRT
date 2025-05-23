/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D8_dist_sprite.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/04 17:53:33 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			distance_from_sprite(t_c_px *calcul, void *obj, int simple);
static int	h_dist_sprite(t_c_px *calcul, t_sprite *plane, t_c_plane *c, \
	int simple);
static void	h_img_sprite(t_c_px *calcul, t_c_plane *c, t_sprite *sprite);

///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
int	distance_from_sprite(t_c_px *calcul, void *obj, int simple)
{
	t_c_plane	c;
	t_sprite	*sp;
	t_coor		inter;

	sp = (t_sprite *)obj;
	c.top = -(ft_dot_p(&sp->O.view, (t_vect *)&calcul->c0) + sp->d);
	c.bot = ft_dot_p(&sp->O.view, &calcul->v);
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (0);
	c.dist = c.top / c.bot;
	if (c.dist < EPSILON)
		return (0);
	inter = new_moved_point(&calcul->c0, &calcul->v, c.dist);
	c.o_to_inter = vect_ab(&sp->O.c0, &inter);
	c.u = ft_dot_p(&c.o_to_inter, &sp->O.right);
	c.v = ft_dot_p(&c.o_to_inter, &sp->O.up);
	if (c.u < EPSILON || c.u > sp->size || c.v < EPSILON \
		|| c.v > sp->size * sp->param.txt->sz_y / sp->param.txt->sz_x)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0)
		return (h_dist_sprite(calcul, sp, &c, simple));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
static int	h_dist_sprite(t_c_px *calcul, t_sprite *sprite, t_c_plane *c, \
	int simple)
{
	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = (t_obj2 *)sprite;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	calcul->mat = *(t_mat2 *)&sprite->param;
	calcul->vn = sprite->O.view;
	c->in = 1;
	if (c->bot > EPSILON)
	{
		c->in = 0;
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
	}
	h_img_sprite(calcul, c, sprite);
	if (sprite->param.c2.r >= 0)
		calcul->mat.argb = (t_argb){calcul->mat.argb.a, sprite->param.c2.r, \
			sprite->param.c2.g, sprite->param.c2.b};
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_img_sprite(t_c_px *calcul, t_c_plane *c, t_sprite *sprite)
{
	double	text_x;
	double	text_y;
	t_vect	normal_map;
	t_obj	local;

	text_x = c->u / sprite->size;
	text_y = 1.0 - c->v / (sprite->size * sprite->param.txt->sz_y / \
		sprite->param.txt->sz_x);
	update_mat_w_txt(calcul, (t_obj2 *)sprite, text_x, text_y);
	if (sprite->param.n_map)
	{
		normal_map = return_vect_img(sprite->param.n_map, text_x, text_y);
		local.up = sprite->O.up;
		local.right = sprite->O.right;
		local.view = calcul->vn;
		calcul->vn = mult_3x3_vect(&local, &normal_map);
		ft_normalize_vect(&calcul->vn);
	}
}
