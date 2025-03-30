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
double		h_dist_triangle(t_tri *tri, t_model *o, t_c_obj *c);

///////////////////////////////////////////////////////////////////////////////]
void	ft_rotate_camera_vect(t_c_px *calcul, t_object *obj, t_c_obj *c)
{
	c->new_o = new_moved_point(&calcul->c0, (t_vect *)&obj->O.c0, -1.0);
	c->new_o = (t_coor){
		c->new_o.x * obj->O.right.dx + c->new_o.y * obj->O.up.dx \
			+ c->new_o.z * obj->O.view.dx,
		c->new_o.x * obj->O.right.dy + c->new_o.y * obj->O.up.dy \
			+ c->new_o.z * obj->O.view.dy,
		c->new_o.x * obj->O.right.dz + c->new_o.y * obj->O.up.dz \
			+ c->new_o.z * obj->O.view.dz,
	};
	c->v_rotate = mult_3x3_vect(&obj->O, &calcul->v);
	ft_normalize_vect(&c->v_rotate);
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
	c->det = ft_dot_p(&c->e1, &c1.h);
	if (fabs(c->det) < EPSILON)
		return (-1.0);
	c1.s = vect_ab(&c1.p1, &c->new_o);
	c1.u = ft_dot_p(&c1.s, &c1.h) / c->det;
	if (c1.u < 0 || c1.u > 1)
		return (-1.0);
	c1.q = ft_cross_product(&c1.s, &c->e1);
	c1.v = ft_dot_p(&c->v_rotate, &c1.q) / c->det;
	if (c1.v < 0 || c1.u + c1.v > 1)
		return (-1.0);
	c1.t = ft_dot_p(&c->e2, &c1.q) / c->det;
	if (c1.t < EPSILON)
		return (-1.0);
	return (c1.t);
}
