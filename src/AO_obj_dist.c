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

int		distance_from_object(t_c_px *calcul, void *object, int simple);
int		h_distance_from_object(t_c_px *calcul, t_object *obj, t_c_obj *c, int simple);
int		h_closest_triangle(t_c_px *calcul, t_object *obj, t_c_obj *c);
void	h_img_obj(t_c_px *calcul, t_object *obj, t_c_obj *c);
void	f_return_obj_normal(t_c_px *calcul, t_c_obj *c, t_object *obj);
t_coor	h_uvw(t_c_px *calcul, t_c_obj *c, t_model *m);
t_argb	h_obj_color2(t_c_px *calcul, t_c_obj *c, t_model *m);

void print_bbox(t_bbox *node, char*color)
{
	if (!node) 
		return;
	printf("%sNode %p: (%d) min[%.2f %.2f %.2f]max[%.2f %.2f %.2f]\n",
		color, node, node->how_many_f, node->min.x, node->min.y, node->min.z, node->max.x, node->max.y, node->max.z);
	// printf("Triangles: ");
	// t_tri	*ptr = node->f;
	// while (ptr)
	// {
	// 	printf("[%d %d %d] ", ptr->p1, ptr->p2, ptr->p3);
	// 	ptr = ptr->next;
	// }
	// printf("\n");

	if (node->l)
		print_bbox(node->l, GREEN);
	if (node->r)
		print_bbox(node->r, RED);
}
///////////////////////////////////////////////////////////////////////////////]
// int	distance_from_object(t_calcul_px *calcul, void *object, int simple)
// {
// 	t_object	*obj;
// 	t_obj_calc	c;

// 	obj = (t_object *)object;
// 	c.dist = calcul->dist;
// 	ft_rotate_camera_vect(calcul, obj, &c);
// 	if (!f_check_if_in_box(calcul, obj, &c))
// 		return (0);
// 	return (h_distance_from_object(calcul, object, &c, simple));
// }
int	distance_from_object(t_c_px *calcul, void *object, int simple)
{
	t_object	*obj;
	t_c_obj	c;



	obj = (t_object *)object;
	// if (calcul->print == 1)
	// 	print_bbox(&obj->model->tree, C_412);
	c.dist = calcul->dist;
	c.closest_tri = NULL;
	ft_rotate_camera_vect(calcul, obj, &c);

	find_inter_tri(&obj->model->tree, obj->model, &c, calcul);
	if (!c.closest_tri)
	{
		if (calcul->print == 1)
			printf(RED"didnt found anything\n");
		return (0);
	}
	// return (h_distance_from_object(calcul, object, &c, simple));
	return (h_closest_triangle(calcul, obj, &c));
}

///////////////////////////////////////////////////////////////////////////////]
// if in bound volume, find intersection triangle
// int	h_distance_from_object(t_calcul_px *calcul, t_object *obj, t_obj_calc *c, int simple)
// {
// 	t_tri	*ptr;
// 	double	temp_dist;

// 	c->closest_tri = NULL;
// 	ptr = obj->model->first;
// 	while (ptr)
// 	{
// 		temp_dist = h_dist_triangle(ptr, obj->model, c);
// 		if (temp_dist > EPSILON && temp_dist < c->dist)
// 		{
// 			c->closest_tri = ptr;
// 			c->dist = temp_dist;
// 			if (simple)
// 				return (1);
// 		}
// 		ptr = ptr->next;
// 	}
// 	if (c->closest_tri)
// 		return (h_closest_triangle(calcul, obj, c));
// 	return (0);
// }
int	h_distance_from_object(t_c_px *calcul, t_object *obj, t_c_obj *c, int simple)
{
	t_tri	*ptr;
	double	temp_dist;

	c->closest_tri = NULL;
	ptr = c->bbox->f;
	while (ptr)
	{
		temp_dist = h_dist_triangle(ptr, obj->model, c);
		
		if (calcul->print == 1)
			printf(C_303"temp_dist %.3f\n", temp_dist);	
		if (temp_dist > EPSILON && temp_dist < c->dist)
		{
			c->closest_tri = ptr;
			c->dist = temp_dist;
			if (simple)
				return (1);
		}
		ptr = ptr->next;
	}
	if (c->closest_tri)
		return (h_closest_triangle(calcul, obj, c));
	if (calcul->print == 1)
		printf(C_043"did we found it3? %p\n", c->closest_tri);	
	return (0);
}

int	h_closest_triangle(t_c_px *calcul, t_object *obj, t_c_obj *c)
{
	int	in;

	calcul->dist = h_dist_triangle(c->closest_tri, obj->model, c);
	calcul->object = obj;
	if (c->closest_tri->mat)
		calcul->mat = *c->closest_tri->mat;
	else
		calcul->mat = *(t_mat *)&obj->param;
	c->inter2 = new_moved_point(&c->new_o, &c->v_rotate, c->dist);
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, c->dist);
	f_return_obj_normal(calcul, c, obj);
	in = (c->det < EPSILON);
	if (in)
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
	// if (obj->param.texture || obj->param.normal_map || obj->param.alpha_map)
	h_img_obj(calcul, obj, c);
	if (obj->param.color2.r >= 0 && !obj->param.texture)
		calcul->mat.argb = h_obj_color2(calcul, c, obj->model);
	return (1 + in);
}

///////////////////////////////////////////////////////////////////////////////]
void	h_img_obj(t_c_px *calcul, t_object *obj, t_c_obj *c)
{
	t_coor	uvw;
	t_tri	*t;
	t_vt	**vt;
	double	u;
	double	v;	

	if (c->closest_tri->vt[0] < 0)
		return ;
	uvw = h_uvw(calcul, c, obj->model);
	t = c->closest_tri;
	vt = obj->model->vt;
	u = uvw.x * vt[t->vt[0]]->u + uvw.y * vt[t->vt[1]]->u + uvw.z * vt[t->vt[2]]->u;
	v = uvw.x * vt[t->vt[0]]->v + uvw.y * vt[t->vt[1]]->v + uvw.z * vt[t->vt[2]]->v;
	if (obj->param.texture && t->mat && !t->mat->txt) 
		calcul->mat.argb = return_px_img(obj->param.texture, u, v);
	else if (t->mat && t->mat->txt)
		calcul->mat.argb = return_px_img_inverse(t->mat->txt, u, v);
	if (obj->param.alpha_map)
		calcul->mat.argb.a = return_alpha_img(obj->param.alpha_map, u, v);
	if (obj->param.normal_map)
	{
		t_vect	normal_map;
		t_obj	local;
		normal_map = return_vect_img(obj->param.normal_map, u, v);
		local.view = calcul->vn;
		create_vector_space(&local);
		calcul->vn = (t_vect){
			local.right.dx * normal_map.dx + local.up.dx * normal_map.dy + local.view.dx * normal_map.dz,
			local.right.dy * normal_map.dx + local.up.dy * normal_map.dy + local.view.dy * normal_map.dz,
			local.right.dz * normal_map.dx + local.up.dz * normal_map.dy + local.view.dz * normal_map.dz,
		};
		ft_normalize_vect(&calcul->vn);
	}
}

///////////////////////////////////////////////////////////////////////////////]
// calcul->v_normal = f_return_obj_normal
void	f_return_obj_normal(t_c_px *calcul, t_c_obj *c, t_object *obj)
{
	t_tri	*t;
	t_coor	uvw;
	t_vect	**vn;

	t = c->closest_tri;
	uvw = h_uvw(calcul, c, obj->model);
	vn = obj->model->vn;
	if (c->closest_tri->vn[0] < 0)
		calcul->vn = ft_cross_product_norm(&c->e1, &c->e2);
	else
	{
		calcul->vn = (t_vect){
			uvw.x * vn[t->vn[0]]->dx + uvw.y * vn[t->vn[1]]->dx + uvw.z * vn[t->vn[2]]->dx,
			uvw.x * vn[t->vn[0]]->dy + uvw.y * vn[t->vn[1]]->dy + uvw.z * vn[t->vn[2]]->dy,
			uvw.x * vn[t->vn[0]]->dz + uvw.y * vn[t->vn[1]]->dz + uvw.z * vn[t->vn[2]]->dz
		};
	}
	calcul->vn = (t_vect){
		calcul->vn.dx * obj->O.right.dx + calcul->vn.dy * obj->O.right.dy + calcul->vn.dz * obj->O.right.dz,
		calcul->vn.dx * obj->O.up.dx + calcul->vn.dy * obj->O.up.dy + calcul->vn.dz * obj->O.up.dz,
		calcul->vn.dx * obj->O.view.dx + calcul->vn.dy * obj->O.view.dy + calcul->vn.dz * obj->O.view.dz
	};
	ft_normalize_vect(&calcul->vn);
}

// barycenter
t_coor	h_uvw(t_c_px *calcul, t_c_obj *c, t_model *m)
{
	t_vect	a_c;
	double	d[3][2];
	double	denom;
	t_coor	uvw;

	a_c = vect_ab(m->v[c->closest_tri->p[0]], &c->inter2);
	d[0][0] = ft_dot_product(&c->e1, &c->e1);
	d[0][1] = ft_dot_product(&c->e1, &c->e2);
	d[1][1] = ft_dot_product(&c->e2, &c->e2);
	d[2][0] = ft_dot_product(&c->e1, &a_c);
	d[2][1] = ft_dot_product(&c->e2, &a_c);
	denom = d[0][0] * d[1][1] - d[0][1] * d[0][1];
	if (fabs(denom) < EPSILON2)
		return ((t_coor){0});
	uvw.y = (d[1][1] * d[2][0] - d[0][1] * d[2][1]) / denom;
	uvw.z = (d[0][0] * d[2][1] - d[0][1] * d[2][0]) / denom;
	uvw.x = 1.0 - uvw.y - uvw.z;
	return (uvw);
}

t_argb	h_obj_color2(t_c_px *calcul, t_c_obj *c, t_model *m)
{
	t_argb	rtrn;

	c->inter2 = new_moved_point(&c->new_o, &c->v_rotate, c->dist);
	rtrn = (t_argb){
		calcul->mat.argb.a,
		(int)round(fmin(1.0, fmax(0, (c->inter2.x - m->tree.min.x) / (m->tree.max.x - m->tree.min.x))) * 255),
		(int)round(fmin(1.0, fmax(0, (c->inter2.y - m->tree.min.y) / (m->tree.max.y - m->tree.min.y))) * 255),
		(int)round(fmin(1.0, fmax(0, (c->inter2.z - m->tree.min.z) / (m->tree.max.z - m->tree.min.z))) * 255)
	};
	return (rtrn);
}
