/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D8_dist_sprite.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/11 11:52:11 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			distance_from_sprite(t_c_px *calcul, void *obj, int simple);
static int	h_dist_sprite(t_c_px *calcul, t_sprite *plane, t_plane_calc *c, int simple);
static void	h_img_sprite(t_c_px *calcul, t_plane_calc *c, t_sprite *sprite);

///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
int	distance_from_sprite(t_c_px *calcul, void *obj, int simple)
{
	t_plane_calc	c;
	t_sprite		*sprite;
	t_coor			inter;

	sprite = (t_sprite *)obj;
	c.top = -(sprite->O.view.dx * calcul->c0.x + sprite->O.view.dy * calcul->c0.y + sprite->O.view.dz * calcul->c0.z + sprite->d);
	c.bot = sprite->O.view.dx * calcul->v.dx + sprite->O.view.dy * calcul->v.dy + sprite->O.view.dz * calcul->v.dz;
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (0);
	c.dist = c.top / c.bot;
	if (c.dist < EPSILON)
		return (0);
	inter = new_moved_point(&calcul->c0, &calcul->v, c.dist);
	c.o_to_inter = vect_ab(&sprite->O.c0, &inter);
	c.u = ft_dot_product(&c.o_to_inter, &sprite->O.right);
	c.v = ft_dot_product(&c.o_to_inter, &sprite->O.up);
	if (c.u < EPSILON || c.u > sprite->size
		|| c.v < EPSILON || c.v > sprite->size * sprite->param.texture->sz_y \
			/ sprite->param.texture->sz_x)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		return (h_dist_sprite(calcul, sprite, &c, simple));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
static int	h_dist_sprite(t_c_px *calcul, t_sprite *sprite, t_plane_calc *c, int simple)
{
	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = sprite;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	calcul->mat = *(t_mat *)&sprite->param;
	calcul->vn = sprite->O.view;
	if (c->bot > 0.0)
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
	h_img_sprite(calcul, c, sprite);
	if (sprite->param.color2.r >= 0)
		calcul->mat.argb = (t_argb){calcul->mat.argb.a, sprite->param.color2.r, sprite->param.color2.g, sprite->param.color2.b};
	return (1);
}

static void	h_img_sprite(t_c_px *calcul, t_plane_calc *c, t_sprite *sprite)
{
	double	text_x;
	double	text_y;
	t_vect	normal_map;

	text_x = c->u / sprite->size;
	text_y = 1.0 - c->v / (sprite->size * sprite->param.texture->sz_y / sprite->param.texture->sz_x);
	calcul->mat.argb = return_px_img(sprite->param.texture, text_x, text_y);
	if (sprite->param.alpha_map)
		calcul->mat.argb.a = return_alpha_img(sprite->param.alpha_map, text_x, text_y);
	if (sprite->param.normal_map)
	{
		normal_map = return_vect_img(sprite->param.normal_map, text_x, text_y);
		calcul->vn = (t_vect){
			sprite->O.right.dx * normal_map.dx + sprite->O.up.dx * normal_map.dy + calcul->vn.dx * normal_map.dz,
			sprite->O.right.dy * normal_map.dx + sprite->O.up.dy * normal_map.dy + calcul->vn.dy * normal_map.dz,
			sprite->O.right.dz * normal_map.dx + sprite->O.up.dz * normal_map.dy + calcul->vn.dz * normal_map.dz,
		};
		ft_normalize_vect(&calcul->vn);
	}
}
