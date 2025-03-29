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
typedef struct s_make_tree	t_mtree;

void	do_the_tree_splitting(t_model *model);
static void	ft_find_smallest_biggest_2(t_model *model, t_bbox *node);
static void	ft_give_centroid(t_model *model, t_bbox *node);
static void	ft_split_by_xyz(t_model *model, t_bbox *node, int xyz);
static double	find_median(t_bbox *node, int xyz);
static t_mtree	h_loop_split(t_model *model, t_bbox *node, int xyz, t_mtree t);
static void	h_double(t_tri *tri, t_bbox *node, t_model *model, int xyz);
void	find_inter_tri(t_bbox *node, t_model *model, t_c_obj *c, t_c_px *calcul);
void	h_find_tri(t_bbox *node, t_model *model, t_c_obj *c, t_c_px *calcul);
static int	f_check_if_in_box_2(t_bbox *bbox, t_c_obj *c);
static void	h_bound_mmax(double min_max_xyz[2][3], int xyz, t_bbox *bbox, t_c_obj *c);
static void ft_free_triangles(t_tri *f);
void ft_free_tree(t_bbox *node);

///////////////////////////////////////////////////////////////////////////////]
void	do_the_tree_splitting(t_model *model)
{
	ft_give_centroid(model, &model->tree);
	ft_split_by_xyz(model, &model->tree, 0);
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
		ptr->centroid.x = (v[ptr->p1]->x + v[ptr->p2]->x + v[ptr->p3]->x) / 3;
		ptr->centroid.y = (v[ptr->p1]->y + v[ptr->p2]->y + v[ptr->p3]->y) / 3;
		ptr->centroid.z = (v[ptr->p1]->z + v[ptr->p2]->z + v[ptr->p3]->z) / 3;
		ptr = ptr->next;
	}
}

///////////////////////////////////////////////////////////////////////////////]
// Recursively split by axis x/y/z
static void	ft_split_by_xyz(t_model *model, t_bbox *node, int xyz)
{
	t_mtree	t;

	t.avg = find_median(node, xyz);
	ft_find_smallest_biggest_2(model, node);
	if (node->how_many_f < 5)
		return ;
	node->l = mem(0, sizeof(t_bbox));
	node->r = mem(0, sizeof(t_bbox));
	if (!node->l || !node->r)
		return (put(ERRM), (void)0);
	t = h_loop_split(model, node, xyz, t);
	if (t.ptr_l)
		t.ptr_l->next = NULL;
	if (t.ptr_r)
		t.ptr_r->next = NULL;
	node->f = NULL;
	ft_split_by_xyz(model, node->l, (xyz + 1) % 3);
	ft_split_by_xyz(model, node->r, (xyz + 1) % 3);
}

// send each triangles left or right
static t_mtree	h_loop_split(t_model *model, t_bbox *node, int xyz, t_mtree t)
{
	t.ptr = node->f;
	t.ptr_r = NULL;
	t.ptr_l = NULL;
	while (t.ptr)
	{
		if (((double *)&t.ptr->centroid)[xyz] <= t.avg)
		{
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
// return the median for the group of triangle in the node
// count at the same time how many triangles in the bbox
static double	find_median(t_bbox *node, int xyz)
{
	int		num;
	double	avg;
	t_tri	*ptr;

	avg = 0.0;
	num = 0;
	ptr = node->f;
	while (ptr && ++num)
	{
		avg += ((double *)&ptr->centroid)[xyz];
		ptr = ptr->next;
	}
	node->how_many_f = num;
	return (avg / num);
}

///////////////////////////////////////////////////////////////////////////////]
void	h_big_small(t_bbox *node, t_coor *p);
///////////////////////////////////////////////////////////////////////////////]
// find the first bounding box value min max, from vertices
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
		h_big_small(node, model->v[ptr->p1]);
		h_big_small(node, model->v[ptr->p2]);
		h_big_small(node, model->v[ptr->p3]);
		ptr = ptr->next;
	}
}

void	h_big_small(t_bbox *node, t_coor *p)
{
	if (p->x < node->min.x)
		node->min.x = p->x;
	if (p->x > node->max.x)
		node->max.x = p->x;
	if (p->y < node->min.y)
		node->min.y = p->y;
	if (p->y > node->max.y)
		node->max.y = p->y;
	if (p->z < node->min.z)
		node->min.z = p->z;
	if (p->z > node->max.z)
		node->max.z = p->z;
}

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
// Recursively go down the tree, until it founds a leaf node
// Once in leaf node, does collision check for all triangles
void	find_inter_tri(t_bbox *node, t_model *model, t_c_obj *c, t_c_px *calcul)
{
	if (!node)
		return ;
	if (node->f)
	{
		h_find_tri(node, model, c, calcul);
		return ;
	}
	if (f_check_if_in_box_2(node, c))
	{
		find_inter_tri(node->l, model, c, calcul);
		find_inter_tri(node->r, model, c, calcul);
	}
}

// does collision check for all triangles in the leaf node
void	h_find_tri(t_bbox *node, t_model *model, t_c_obj *c, t_c_px *calcul)
{
	t_tri	*ptr;
	double	temp_dist;

	ptr = node->f;
	while (ptr)
	{
		temp_dist = h_dist_triangle(ptr, model, c);
		if (temp_dist > EPSILON && (temp_dist < c->dist || c->dist < 0))
		{
			c->closest_tri = ptr;
			c->dist = temp_dist;
		}
		ptr = ptr->next;
	}
}

///////////////////////////////////////////////////////////////////////////////]
// check if there is intersection in bound volume
static int	f_check_if_in_box_2(t_bbox *bbox, t_c_obj *c)
{
	double	m[2][3];

	h_bound_mmax(m, X, bbox, c);
	h_bound_mmax(m, Y, bbox, c);
	h_bound_mmax(m, Z, bbox, c);
	c->t_enter = fmax(fmax(m[MIN][X], m[MIN][Y]), m[MIN][Z]);
	c->t_exit = fmin(fmin(m[MAX][X], m[MAX][Y]), m[MAX][Z]);
	if (c->t_enter > c->t_exit || c->t_exit < EPSILON)
		return (0);
	if (c->dist > EPSILON && c->t_enter > c->dist)
		return (0);
	if (c->dist < 0)
		c->dist = c->t_exit + 1.0;
	return (1);
}

// set the t_dist min/max for the given x/y/z axis
static void	h_bound_mmax(double mmxyz[2][3], int xyz, t_bbox *bbox, t_c_obj *c)
{
	double	t_min;
	double	t_max;

	t_min = (((double *)&bbox->min)[xyz] - ((double *)&c->new_o)[xyz]) \
		/ ((double *)&c->v_rotate)[xyz];
	t_max = (((double *)&bbox->max)[xyz] - ((double *)&c->new_o)[xyz]) \
		/ ((double *)&c->v_rotate)[xyz];
	if (t_min > t_max)
	{
		mmxyz[MIN][xyz] = t_max;
		mmxyz[MAX][xyz] = t_min;
	}
	else
	{
		mmxyz[MIN][xyz] = t_min;
		mmxyz[MAX][xyz] = t_max;
	}
}

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
// free
static void	ft_free_triangles(t_tri *f)
{
	t_tri	*tmp;

	while (f)
	{
		tmp = f;
		f = f->next;
		free_s(tmp);
	}
}

// Recursively free the tree
void	ft_free_tree(t_bbox *node)
{
	if (!node)
		return ;
	ft_free_triangles(node->f);
	ft_free_tree(node->l);
	ft_free_tree(node->r);
	free_s(node);
}
