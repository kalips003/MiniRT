/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D7_dist_cube.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/25 18:34:57 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

typedef struct s_cube_calc1 {
	t_vect	o_to_inter;
	double	top;
	double	bot;

	double	dist;

	double	u;
	double	v;

	t_vect	*main_v;
	t_vect	*v2;
	t_vect	*v3;

	t_coor	inter;
	t_vect	*v_norm;
	t_vect	v_right;
	t_vect	*v_up;
}	t_cube_calc1;

int		distance_from_cube(t_calcul_px *calcul, void *obj, int simple);
int		h_dist_cube1(t_calcul_px *calcul, t_cube *cube, t_cube_calc1 *c, int simple);
void	h_dist_cube2(t_calcul_px *calcul, t_cube *cube, t_cube_calc1 *c);
void	h_img_cube(t_calcul_px *calcul, t_cube *cube, t_cube_calc1 *c);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
// top = signed distance camera plane ~ cos angle
// bot = dot product view vector / plane vector (means nothing)
int	distance_from_cube(t_calcul_px *calcul, void *obj, int simple)
{
	t_cube_calc1	c;
	t_cube			*cube;
	int				rtrn;

	cube = (t_cube *)obj;
	c.dist = calcul->dist;
	c.main_v = &cube->O.view;
	c.v2 = &cube->O.up;
	c.v3 = &cube->O.right;
	rtrn = h_dist_cube1(calcul, cube, &c, simple);
	c.main_v = &cube->O.up;
	c.v2 = &cube->O.right;
	c.v3 = &cube->O.view;
	rtrn |= h_dist_cube1(calcul, cube, &c, simple);
	c.main_v = &cube->O.right;
	c.v2 = &cube->O.view;
	c.v3 = &cube->O.up;
	rtrn |= h_dist_cube1(calcul, cube, &c, simple);
	if (rtrn && simple)
		return (1);
	if (rtrn)
		h_dist_cube2(calcul, cube, &c);
	return (rtrn);
}

typedef struct s_cube_calc2 {
	double	d1;
	double	d2;

	double	top1;
	double	top2;

	double	bot;

	double	dist1;
	double	dist2;

	t_coor	inter;
	t_vect	o_to_inter;
	double	dot1;
	double	dot2;
}	t_cube_calc2;

int	h_dist_cube1(t_calcul_px *calcul, t_cube *cube, t_cube_calc1 *c, int simple)
{
	t_cube_calc2	c2;

	c2.d1 = -(c->main_v->dx * cube->O.c0.x + c->main_v->dy * cube->O.c0.y + c->main_v->dz * cube->O.c0.z);
	c2.d2 = -(c->main_v->dx * cube->other_p.x + c->main_v->dy * cube->other_p.y + c->main_v->dz * cube->other_p.z);
	c2.top1 = -(c->main_v->dx * calcul->c0.x + c->main_v->dy * calcul->c0.y + c->main_v->dz * calcul->c0.z + c2.d1);
	c2.top2 = c2.top1 + c2.d1 - c2.d2;
	c2.bot = c->main_v->dx * calcul->v.dx + c->main_v->dy * calcul->v.dy + c->main_v->dz * calcul->v.dz;
	if (fabs(c2.bot) < EPSILON)
		return (0);
	c2.dist1 = c2.top1 / c2.bot;
	c2.dist2 = c2.top2 / c2.bot;
	if (c2.dist1 < EPSILON && c2.dist2 < EPSILON)
		return (0);
	c2.dist1 = h_smalest_delta(c2.dist1, c2.dist2);
	c2.inter = new_moved_point(&calcul->c0, &calcul->v, c2.dist1);
	c2.o_to_inter = vect_ab(&cube->O.c0, &c2.inter);
	c2.dot1 = ft_dot_product(c->v2, &c2.o_to_inter);
	c2.dot2 = ft_dot_product(c->v3, &c2.o_to_inter);
	if (c2.dot1 < 0.0 || c2.dot2 < 0.0 || c2.dot1 > cube->size || c2.dot2 > cube->size)
		return (0);
	if (calcul->dist > 0.0 && c2.dist1 > c->dist)
		return (0);
	if (simple)
		return (1);
	c->dist = c2.dist1;
	c->inter = c2.inter;
	c->v_norm = c->main_v;
	c->v_up = c->v2;
	c->v_right = *c->v3;
	c->o_to_inter = c2.o_to_inter;
	c->u = c2.dot1 / cube->size;
	c->v = c2.dot2 / cube->size;
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
void	h_dist_cube2(t_calcul_px *calcul, t_cube *cube, t_cube_calc1 *c)
{
	t_vect	diag;
	double	dist_color;

	calcul->dist = c->dist;
	calcul->object = cube;
	calcul->inter = c->inter;
	calcul->mat = *(t_mat *)&cube->param;
	calcul->vn = *c->v_norm;
	if (!cube->param.texture && cube->param.color2.r >= 0)
	{
		diag = vect_ab_norm(&cube->O.c0, &cube->other_p);
		dist_color = ft_dot_product(&c->o_to_inter, &diag) / (SQRT3 * cube->size);
		calcul->mat.argb = dual_color_render(&cube->param.argb, &cube->param.color2, dist_color);
	}
	if (ft_dot_product(&calcul->v, c->v_norm) > 0.0)
	{
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
		c->v_right = (t_vect){-c->v_right.dx, -c->v_right.dy, -c->v_right.dz};
	}
	if (cube->param.texture || cube->param.normal_map || cube->param.alpha_map)
		h_img_cube(calcul, cube, c);
}

void	h_img_cube(t_calcul_px *calcul, t_cube *cube, t_cube_calc1 *c)
{
	t_vect	normal_map;

	if (cube->param.texture)
		calcul->mat.argb = return_px_img(cube->param.texture, c->u, c->v);
	if (cube->param.alpha_map)
		calcul->mat.argb.a = return_alpha_img(cube->param.alpha_map, c->u, c->v);
	if (cube->param.normal_map)
	{
		normal_map = return_vect_img(cube->param.normal_map, c->u, c->v);
		calcul->vn = (t_vect){
			c->v_right.dx * normal_map.dx + c->v_up->dx * normal_map.dy + calcul->vn.dx * normal_map.dz,
			c->v_right.dy * normal_map.dx + c->v_up->dy * normal_map.dy + calcul->vn.dy * normal_map.dz,
			c->v_right.dz * normal_map.dx + c->v_up->dz * normal_map.dy + calcul->vn.dz * normal_map.dz,
		};
		ft_normalize_vect(&calcul->vn);
	}
}
