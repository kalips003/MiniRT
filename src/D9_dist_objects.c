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

int				distance_from_object(t_c_px *calcul, void *object, int simple);
static int		h_closest_triangle(t_c_px *calcul, t_object *obj, t_c_obj *c);
static void		h_img_obj(t_c_px *calcul, t_object *obj, t_c_obj *c);
static t_argb	h_obj_color2(t_c_px *calcul, t_c_obj *c, t_model *m);

///////////////////////////////////////////////////////////////////////////////]
int	distance_from_object(t_c_px *calcul, void *object, int simple)
{
	t_object	*obj;
	t_c_obj		c;

	obj = (t_object *)object;
	c.dist = calcul->dist;
	c.size = obj->size;
	c.t = NULL;
	ft_rotate_around_obj(calcul, (t_obj2 *)object, (t_obj *)&c.new_o);
	find_inter_tri(&obj->model->tree, obj->model, &c, calcul);
	if (!c.t)
		return (0);
	if (c.t && simple)
		return (1);
	return (h_closest_triangle(calcul, obj, &c));
}

///////////////////////////////////////////////////////////////////////////////]
static int	h_closest_triangle(t_c_px *calcul, t_object *obj, t_c_obj *c)
{
	int	in;

	calcul->dist = h_dist_triangle(c->t, obj->model, c);
	calcul->object = (t_obj2 *)obj;
	if (c->t->mat)
		calcul->mat = *(t_mat2 *)c->t->mat;
	else
		calcul->mat = *(t_mat2 *)&obj->param;
	c->inter2 = new_moved_point(&c->new_o, &c->v_rotate, c->dist);
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	f_return_obj_normal(calcul, c, obj);
	in = (c->det < EPSILON);
	if (c->t->vt[0] >= 0)
		h_img_obj(calcul, obj, c);
	if (in)
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};// not correct inverse
	if (obj->param.c2.r >= 0 && !obj->param.txt)
		calcul->mat.argb = h_obj_color2(calcul, c, obj->model);
	return (1 + in);
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_img_obj(t_c_px *calcul, t_object *obj, t_c_obj *c)
{
	t_tri	*t;
	t_vt	**vt;
	t_vect	normal_map;
	t_obj	local_v_space;

	c->uvw = h_uvw(c, obj->model);
	t = c->t;
	vt = obj->model->vt;
	c->u = c->uvw.x * vt[t->vt[0]]->u + c->uvw.y * vt[t->vt[1]]->u \
		+ c->uvw.z * vt[t->vt[2]]->u;
	c->v = c->uvw.x * vt[t->vt[0]]->v + c->uvw.y * vt[t->vt[1]]->v \
		+ c->uvw.z * vt[t->vt[2]]->v;
	if (t->mat && t->mat->txt)
		calcul->mat.argb = return_px_img(t->mat->txt, c->u, 1.0 - c->v);
	update_mat_w_txt(calcul, (t_obj2 *)obj, c->u, c->v);
	if (obj->param.n_map)
	{
		normal_map = return_vect_img(obj->param.n_map, c->u, c->v);
		local_v_space.view = calcul->vn;
		create_vector_space(&local_v_space);
		calcul->vn = mult_3x3_vect(&local_v_space, &normal_map);
		ft_normalize_vect(&calcul->vn);
	}
}

///////////////////////////////////////////////////////////////////////////////]
static t_argb	h_obj_color2(t_c_px *calcul, t_c_obj *c, t_model *m)
{
	t_argb	rtrn;
	t_coor	min;
	t_coor	max;

	min = scale_point(m->tree.min, c->size);
	max = scale_point(m->tree.max, c->size);
	rtrn = (t_argb){
		calcul->mat.argb.a,
		(int)round(fmin(1.0, fmax(0, (c->inter2.x - min.x) / \
			(max.x - min.x))) * 255),
		(int)round(fmin(1.0, fmax(0, (c->inter2.y - min.y) / \
			(max.y - min.y))) * 255),
		(int)round(fmin(1.0, fmax(0, (c->inter2.z - min.z) / \
			(max.z - min.z))) * 255)
	};
	return (rtrn);
}
