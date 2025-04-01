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

void	ft_rotate_around_obj(t_c_px *calcul, t_obj2 *o, t_obj *c);
double	h_dist_triangle(t_tri *tri, t_model *o, t_c_obj *c);
void	f_return_obj_normal(t_c_px *ca, t_c_obj *c, t_object *obj);
t_coor	h_uvw(t_c_obj *c, t_model *m);

///////////////////////////////////////////////////////////////////////////////]
// rotate cam around obj, instead of rotating object
// take the address of c->new_O / c->v_rotate as a t_obj
void	ft_rotate_around_obj(t_c_px *calcul, t_obj2 *o, t_obj *c)
{
	c->c0 = new_moved_point(&calcul->c0, (t_vect *)&o->O.c0, -1.0);
	c->c0 = (t_coor){
		c->c0.x * o->O.right.dx + c->c0.y * o->O.up.dx + c->c0.z \
			* o->O.view.dx,
		c->c0.x * o->O.right.dy + c->c0.y * o->O.up.dy + c->c0.z \
			* o->O.view.dy,
		c->c0.x * o->O.right.dz + c->c0.y * o->O.up.dz + c->c0.z \
			* o->O.view.dz};
	c->view = (t_vect){
		calcul->v.dx * o->O.right.dx + calcul->v.dy * o->O.up.dx \
			+ calcul->v.dz * o->O.view.dx,
		calcul->v.dx * o->O.right.dy + calcul->v.dy * o->O.up.dy \
			+ calcul->v.dz * o->O.view.dy,
		calcul->v.dx * o->O.right.dz + calcul->v.dy * o->O.up.dz \
			+ calcul->v.dz * o->O.view.dz};
	ft_normalize_vect(&c->view);
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

///////////////////////////////////////////////////////////////////////////////]
// calcul->v_normal = f_return_obj_normal
void	f_return_obj_normal(t_c_px *ca, t_c_obj *c, t_object *obj)
{
	t_coor	uvw;
	t_obj	temp;

	uvw = h_uvw(c, obj->model);
	if (c->t->vn[0] < 0)
		ca->vn = ft_cross_product_norm(&c->e1, &c->e2);
	else
	{
		temp.right = *obj->model->vn[c->t->vn[0]];
		temp.up = *obj->model->vn[c->t->vn[1]];
		temp.view = *obj->model->vn[c->t->vn[2]];
		ca->vn = mult_3x3_vect(&temp, (t_vect *)&uvw);
	}
	ca->vn = (t_vect){ca->vn.dx * obj->O.right.dx + ca->vn.dy * \
		obj->O.right.dy + ca->vn.dz * obj->O.right.dz, ca->vn.dx * \
		obj->O.up.dx + ca->vn.dy * obj->O.up.dy + ca->vn.dz * obj->O.up.dz, \
		ca->vn.dx * obj->O.view.dx + ca->vn.dy * obj->O.view.dy + ca->vn.dz \
		* obj->O.view.dz};
	ft_normalize_vect(&ca->vn);
}

///////////////////////////////////////////////////////////////////////////////]
// barycenter
t_coor	h_uvw(t_c_obj *c, t_model *m)
{
	t_vect	a_c;
	double	d[3][2];
	double	denom;
	t_coor	uvw;

	uvw = scale_point(*m->v[c->t->p[0]], c->size);
	a_c = vect_ab(&uvw, &c->inter2);
	d[0][0] = ft_dot_p(&c->e1, &c->e1);
	d[0][1] = ft_dot_p(&c->e1, &c->e2);
	d[1][1] = ft_dot_p(&c->e2, &c->e2);
	d[2][0] = ft_dot_p(&c->e1, &a_c);
	d[2][1] = ft_dot_p(&c->e2, &a_c);
	denom = d[0][0] * d[1][1] - d[0][1] * d[0][1];
	if (fabs(denom) < EPSILON2)
		return ((t_coor){0});
	uvw.y = (d[1][1] * d[2][0] - d[0][1] * d[2][1]) / denom;
	uvw.z = (d[0][0] * d[2][1] - d[0][1] * d[2][0]) / denom;
	uvw.x = 1.0 - uvw.y - uvw.z;
	return (uvw);
}
