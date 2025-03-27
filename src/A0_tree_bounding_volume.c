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

typedef struct s_tree1	t_bbox;
typedef struct s_make_tree	t_make_tree;

void	do_the_tree_splitting(t_model *model);
static void	ft_find_smallest_biggest(t_model *model);
static void	ft_give_centroid(t_model *model, t_bbox *node);
static void	ft_split_by_xyz(t_model *model, t_bbox *node, int xyz);
static double	find_median(t_bbox *node, int xyz);
static t_make_tree	h_loop_split(t_model *model, t_bbox *node, int xyz, t_make_tree t);
static void	h_double(t_tri *tri, t_bbox *node, t_model *model, int xyz);
void	find_inter_tri(t_bbox *node, t_model *model, t_obj_calc *c, t_calcul_px *calcul);
void	h_find_inter_tri(t_bbox *node, t_model *model, t_obj_calc *c, t_calcul_px *calcul);
static int	f_check_if_in_box_2(t_bbox *bbox, t_obj_calc *c);
static int	h_bounding_min_max(double min_max_xyz[2][3], int xyz, t_bbox *bbox, t_obj_calc *c);
static void ft_free_triangles(t_tri *f);
void ft_free_tree(t_bbox *node);

///////////////////////////////////////////////////////////////////////////////]
void	do_the_tree_splitting(t_model *model)
{
	// ft_find_smallest_biggest(model);
	ft_give_centroid(model, &model->tree);
	ft_split_by_xyz(model, &model->tree, 0);
}

///////////////////////////////////////////////////////////////////////////////]
// find the first bounding box value min max, from vertices
static void	ft_find_smallest_biggest(t_model *model)
{
	t_coor	**ptr;

	model->tree.min = **model->v;
	model->tree.max = **model->v;
	ptr = model->v - 1;
	while (++ptr && *ptr)
	{
		if ((*ptr)->x < model->tree.min.x)
			model->tree.min.x = (*ptr)->x + EPSILON;
		else if ((*ptr)->x > model->tree.max.x)
			model->tree.max.x = (*ptr)->x + EPSILON;
		if ((*ptr)->y < model->tree.min.y)
			model->tree.min.y = (*ptr)->y + EPSILON;
		else if ((*ptr)->y > model->tree.max.y)
			model->tree.max.y = (*ptr)->y + EPSILON;
		if ((*ptr)->z < model->tree.min.z)
			model->tree.min.z = (*ptr)->z + EPSILON;
		else if ((*ptr)->z > model->tree.max.z)
			model->tree.max.z = (*ptr)->z + EPSILON;
	}
}

static void	ft_find_smallest_biggest_2(t_model *model, t_bbox *node)
{
	t_tri	*ptr;

	if (!node->f)
		return ;
	

	node->min = (t_coor){INFINITY, INFINITY, INFINITY};
	node->max = (t_coor){-INFINITY, -INFINITY, -INFINITY};
	ptr = node->f;
	while (ptr)
	{
		if (model->v[ptr->p1]->x < node->min.x)
			node->min.x = model->v[ptr->p1]->x - EPSILON;
		else if (model->v[ptr->p1]->x > node->max.x)
			node->max.x = model->v[ptr->p1]->x + EPSILON;
		if (model->v[ptr->p1]->y < node->min.y)
			node->min.y = model->v[ptr->p1]->y - EPSILON;
		else if (model->v[ptr->p1]->y > node->max.y)
			node->max.y = model->v[ptr->p1]->y + EPSILON;
		if (model->v[ptr->p1]->z < node->min.z)
			node->min.z = model->v[ptr->p1]->z - EPSILON;
		else if (model->v[ptr->p1]->z > node->max.z)
			node->max.z = model->v[ptr->p1]->z + EPSILON;

		if (model->v[ptr->p2]->x < node->min.x)
			node->min.x = model->v[ptr->p2]->x - EPSILON;
		else if (model->v[ptr->p2]->x > node->max.x)
			node->max.x = model->v[ptr->p2]->x + EPSILON;
		if (model->v[ptr->p2]->y < node->min.y)
			node->min.y = model->v[ptr->p2]->y - EPSILON;
		else if (model->v[ptr->p2]->y > node->max.y)
			node->max.y = model->v[ptr->p2]->y + EPSILON;
		if (model->v[ptr->p2]->z < node->min.z)
			node->min.z = model->v[ptr->p2]->z - EPSILON;
		else if (model->v[ptr->p2]->z > node->max.z)
			node->max.z = model->v[ptr->p2]->z + EPSILON;

		if (model->v[ptr->p3]->x < node->min.x)
			node->min.x = model->v[ptr->p3]->x - EPSILON;
		else if (model->v[ptr->p3]->x > node->max.x)
			node->max.x = model->v[ptr->p3]->x + EPSILON;
		if (model->v[ptr->p3]->y < node->min.y)
			node->min.y = model->v[ptr->p3]->y - EPSILON;
		else if (model->v[ptr->p3]->y > node->max.y)
			node->max.y = model->v[ptr->p3]->y + EPSILON;
		if (model->v[ptr->p3]->z < node->min.z)
			node->min.z = model->v[ptr->p3]->z - EPSILON;
		else if (model->v[ptr->p3]->z > node->max.z)
			node->max.z = model->v[ptr->p3]->z + EPSILON;
		ptr = ptr->next;
	}
}



///////////////////////////////////////////////////////////////////////////////]
// give centroid to each triangle
static void	ft_give_centroid(t_model *model, t_bbox *node)
{
	t_coor	**v;
	t_tri	*ptr;

	v = model->v;
	ptr = node->f;
	while (ptr)
	{
		ptr->centroid.x = (v[ptr->p1]->x + v[ptr->p2]->x + v[ptr->p3]->x) / 3.0;
		ptr->centroid.y = (v[ptr->p1]->y + v[ptr->p2]->y + v[ptr->p3]->y) / 3.0;
		ptr->centroid.z = (v[ptr->p1]->z + v[ptr->p2]->z + v[ptr->p3]->z) / 3.0;
		ptr = ptr->next;
	}
}

typedef struct s_make_tree
{
	double	avg;
	t_tri	*ptr;
	t_tri	*ptr_r;
	t_tri	*ptr_l;
}	t_make_tree;

///////////////////////////////////////////////////////////////////////////////]
static void	ft_split_by_xyz(t_model *model, t_bbox *node, int xyz)
{
	t_make_tree	t;

	t.avg = find_median(node, xyz);
	ft_find_smallest_biggest_2(model, node);
	if (node->how_many_f <= 1)
		return ;
	node->l = mem(0, sizeof(t_bbox));
	node->r = mem(0, sizeof(t_bbox));
	if (!node->l || !node->r)
		return (put(ERRM), (void)0);
	// ft_memcpy(node->l, node, sizeof(t_coor) * 2);
	// ft_memcpy(node->r, node, sizeof(t_coor) * 2);
	// ((double*)&node->l->max)[xyz] = ((double*)&node->min)[xyz];

	t = h_loop_split(model, node, xyz, t);
	if (t.ptr_l)
		t.ptr_l->next = NULL;
	if (t.ptr_r)
		t.ptr_r->next = NULL;
	node->f = NULL;
	ft_split_by_xyz(model, node->l, (xyz + 1) % 3);
	ft_split_by_xyz(model, node->r, (xyz + 1) % 3);
}

///////////////////////////////////////////////////////////////////////////////]
// return the median for the group of triangle in the node
// count at the same time how many triangles in the bbox
static double	find_median(t_bbox *node, int xyz)
{
	int num = 0;
	double	avg = 0.0;
	t_tri	*ptr = node->f;

	while (ptr && ++num)
	{
		avg += ((double*)&ptr->centroid)[xyz];
		ptr = ptr->next;
	}
	node->how_many_f = num;
	return (avg / num);
}

///////////////////////////////////////////////////////////////////////////////]
static t_make_tree	h_loop_split(t_model *model, t_bbox *node, int xyz, t_make_tree t)
{
	t.ptr = node->f;
	t.ptr_r = NULL;
	t.ptr_l = NULL;
	while (t.ptr)
	{
		if (((double*)&t.ptr->centroid)[xyz] <= t.avg)
		{
			// h_double(t.ptr, node, model, xyz);
			if (!node->l->f)
				node->l->f = t.ptr;
			else
				t.ptr_l->next = t.ptr;
			t.ptr_l = t.ptr;
		}
		else
		{
			if (!node->r->f)
				node->r->f = t.ptr;
			else
				t.ptr_r->next = t.ptr;
			t.ptr_r = t.ptr;
		}
		t.ptr = t.ptr->next;
	}
	return (t);
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_double(t_tri *tri, t_bbox *node, t_model *model, int xyz)
{
	double	p[3];
	int		i;

	p[0] = ((double*)model->v[tri->p1])[xyz];
	p[1] = ((double*)model->v[tri->p2])[xyz];
	p[2] = ((double*)model->v[tri->p3])[xyz];
	
	i = -1;
	while (++i < 3)
	{
		if (p[i] < ((double*)&node->l->min)[xyz])
			((double*)&node->l->min)[xyz] = p[i];
		if (p[i] > ((double*)&node->l->max)[xyz])
			((double*)&node->l->max)[xyz] = p[i];
		if (p[i] < ((double*)&node->r->min)[xyz])
			((double*)&node->r->min)[xyz] = p[i];
		if (p[i] > ((double*)&node->r->max)[xyz])
			((double*)&node->r->max)[xyz] = p[i];
	}
}

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
void	find_inter_tri(t_bbox *node, t_model *model, t_obj_calc *c, t_calcul_px *calcul)
{
	if (!node)
		return (node);
	if (node->f)
	{		
		if (calcul->print ==1)
		{
			printf(C_411"checking in box: ");
			printf("(%d) min[%.2f %.2f %.2f]max[%.2f %.2f %.2f]\n", node->how_many_f, node->min.x, node->min.y, node->min.z, node->max.x, node->max.y, node->max.z);
		}
		h_find_inter_tri(node, model, c, calcul);
		return ;
	}
	if (f_check_if_in_box_2(node, c))
	{
		if (calcul->print ==1)
		{
			printf(C_342"FOUND in box: ");
			printf("(%d) min[%.2f %.2f %.2f]max[%.2f %.2f %.2f]\n", node->how_many_f, node->min.x, node->min.y, node->min.z, node->max.x, node->max.y, node->max.z);
		}
		
		find_inter_tri(node->l, model, c, calcul);
		find_inter_tri(node->r, model, c, calcul);
	}
	else if (calcul->print ==1)
	{
		printf(C_243"NOT found in box: ");
		printf("(%d) min[%.2f %.2f %.2f]max[%.2f %.2f %.2f]\n", node->how_many_f, node->min.x, node->min.y, node->min.z, node->max.x, node->max.y, node->max.z);
	}
	return ;
}

void	h_find_inter_tri(t_bbox *node, t_model *model, t_obj_calc *c, t_calcul_px *calcul)
{
	t_tri	*ptr;
	double	temp_dist;

	ptr = node->f;
	while (ptr)
	{
		temp_dist = h_dist_triangle(ptr, model, c);
		if (calcul->print == 1)
		{
			t_coor	**p = model->v;
			if (temp_dist > 0)
			{
				printf(C_512"temp dist = %.3f:", temp_dist);
				printf("[%.3f,%.3f,%.3f]=", p[ptr->p1]->x, p[ptr->p1]->y, p[ptr->p1]->z);
				printf("[%.3f,%.3f,%.3f]=", p[ptr->p2]->x, p[ptr->p2]->y, p[ptr->p2]->z);
				printf("[%.3f,%.3f,%.3f]\n", p[ptr->p3]->x, p[ptr->p3]->y, p[ptr->p3]->z);
				printf("\tCentr:[%.3f,%.3f,%.3f]\n", ptr->centroid.x, ptr->centroid.y, ptr->centroid.z);
			}
		}
		if (temp_dist > EPSILON && (temp_dist < c->dist || c->dist < 0))
		{
			c->closest_tri = ptr;
			c->dist = temp_dist;
		}
		ptr = ptr->next;
	}
}
///////////////////////////////////////////////////////////////////////////////]
// checck if there is intersection in bound volume
static int	f_check_if_in_box_2(t_bbox *bbox, t_obj_calc *c)
{
	double	min_max_xyz[2][3];

	if (h_bounding_min_max(min_max_xyz, 0, bbox, c) || h_bounding_min_max(min_max_xyz, 1, bbox, c) || h_bounding_min_max(min_max_xyz, 2, bbox, c))
		return (1);
	// h_bounding_min_max(min_max_xyz, 0, bbox, c);
	// h_bounding_min_max(min_max_xyz, 1, bbox, c);
	// h_bounding_min_max(min_max_xyz, 2, bbox, c);
	
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

static int	h_bounding_min_max(double min_max_xyz[2][3], int xyz, t_bbox *bbox, t_obj_calc *c)
{
	double	t_min;
	double	t_max;

	if (fabs(((double*)&c->v_rotate)[xyz]) < EPSILON)
	{
		printf("0 denom\n");
		if (((double*)&c->new_o)[xyz] < ((double*)&bbox->min)[xyz] ||
			((double*)&c->new_o)[xyz] > ((double*)&bbox->max)[xyz])
			return (1); // Ray is outside, no intersection
		min_max_xyz[0][xyz] = -INFINITY;
		min_max_xyz[1][xyz] = INFINITY;
		return (1);
	}
	t_min = (((double *)&bbox->min)[xyz] - ((double *)&c->new_o)[xyz]) / ((double *)&c->v_rotate)[xyz];
	t_max = (((double *)&bbox->max)[xyz] - ((double *)&c->new_o)[xyz]) / ((double *)&c->v_rotate)[xyz];
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
	return (0);
}



///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
// free
static void ft_free_triangles(t_tri *f)
{
	t_tri *tmp;
	while (f)
	{
		tmp = f;
		f = f->next;
		free_s(tmp);
	}
}

// Recursively free the tree
void ft_free_tree(t_bbox *node)
{
	if (!node)
		return;
	ft_free_triangles(node->f);
	ft_free_tree(node->l);
	ft_free_tree(node->r);
	free_s(node);
}

