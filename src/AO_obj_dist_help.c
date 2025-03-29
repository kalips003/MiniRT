/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_builtin_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void		ft_rotate_camera_vect(t_c_px *calcul, t_object *obj, t_c_obj *c);
int			f_check_if_in_box(t_c_px *calcul, t_object *obj, t_c_obj *c);
static void	h_bounding_min_max(double min_max_xyz[2][3], int xyz, t_object *obj, t_c_obj *c);
double		h_dist_triangle(t_tri *tri, t_model *o, t_c_obj *c);

///////////////////////////////////////////////////////////////////////////////]

void	ft_rotate_camera_vect(t_c_px *calcul, t_object *obj, t_c_obj *c)
{
	c->new_o = new_moved_point(&calcul->c0, (t_vect *)&obj->O.c0, -1.0);
	c->new_o = (t_coor){
		c->new_o.x * obj->O.right.dx + c->new_o.y * obj->O.up.dx + c->new_o.z * obj->O.view.dx,
		c->new_o.x * obj->O.right.dy + c->new_o.y * obj->O.up.dy + c->new_o.z * obj->O.view.dy,
		c->new_o.x * obj->O.right.dz + c->new_o.y * obj->O.up.dz + c->new_o.z * obj->O.view.dz,
	};
	c->v_rotate = (t_vect)
	{
		calcul->v.dx * obj->O.right.dx + calcul->v.dy * obj->O.up.dx + calcul->v.dz * obj->O.view.dx,
		calcul->v.dx * obj->O.right.dy + calcul->v.dy * obj->O.up.dy + calcul->v.dz * obj->O.view.dy,
		calcul->v.dx * obj->O.right.dz + calcul->v.dy * obj->O.up.dz + calcul->v.dz * obj->O.view.dz,
	};
	ft_normalize_vect(&c->v_rotate);
}

///////////////////////////////////////////////////////////////////////////////]
// checck if there is intersection in bound volume
int	f_check_if_in_box(t_c_px *calcul, t_object *obj, t_c_obj *c)
{
	double	min_max_xyz[2][3];

	(void)calcul;
	h_bounding_min_max(min_max_xyz, 0, obj, c);
	h_bounding_min_max(min_max_xyz, 1, obj, c);
	h_bounding_min_max(min_max_xyz, 2, obj, c);
	c->t_enter = fmax(fmax(min_max_xyz[0][0], min_max_xyz[0][1]), min_max_xyz[0][2]);
	c->t_exit = fmin(fmin(min_max_xyz[1][0], min_max_xyz[1][1]), min_max_xyz[1][2]);
	if (c->t_enter > c->t_exit || c->t_exit < EPSILON)
		return (0);
	if (c->dist > EPSILON && c->t_enter > c->dist)
		return (0);
	if (c->dist < 0)
		c->dist = c->t_exit + 1.0;
	return (1);
}

static void	h_bounding_min_max(double min_max_xyz[2][3], int xyz, t_object *obj, t_c_obj *c)
{
	double	t_min;
	double	t_max;

	// t_min = (((double *)&obj->model->min)[xyz] - ((double *)&c->new_o)[xyz]) / ((double *)&c->v_rotate)[xyz];
	// t_max = (((double *)&obj->model->max)[xyz] - ((double *)&c->new_o)[xyz]) / ((double *)&c->v_rotate)[xyz];
	if (t_min > t_max)
	{
		min_max_xyz[0][xyz] = t_max;
		min_max_xyz[1][xyz] = t_min;
	}
	else
	{
		min_max_xyz[0][xyz] = t_min;
		min_max_xyz[1][xyz] = t_max;
	}
}

///////////////////////////////////////////////////////////////////////////////]
// mathemagic, return -1.0 if no collision with triangle
double	h_dist_triangle(t_tri *tri, t_model *o, t_c_obj *c)
{
	t_calc_dist_tri	c1;

	c1.p1 = scale_point(*(o->v[tri->p[0]]), c->size);
	c1.p2 = scale_point(*(o->v[tri->p[1]]), c->size);
	c1.p3 = scale_point(*(o->v[tri->p[2]]), c->size);
	c->e1 = vect_ab(&c1.p1, &c1.p2);
	c->e2 = vect_ab(&c1.p1, &c1.p3);
	c1.h = ft_cross_product(&c->v_rotate, &c->e2);
	c->det = ft_dot_product(&c->e1, &c1.h);
	if (fabs(c->det) < EPSILON)
		return (-1.0);
	c1.s = vect_ab(&c1.p1, &c->new_o);
	c1.u = ft_dot_product(&c1.s, &c1.h) / c->det;
	if (c1.u < 0 || c1.u > 1)
		return (-1.0);
	c1.q = ft_cross_product(&c1.s, &c->e1);
	c1.v = ft_dot_product(&c->v_rotate, &c1.q) / c->det;
	if (c1.v < 0 || c1.u + c1.v > 1)
		return (-1.0);
	c1.t = ft_dot_product(&c->e2, &c1.q) / c->det;
	if (c1.t < EPSILON)
		return (-1.0);
	return (c1.t);
}
