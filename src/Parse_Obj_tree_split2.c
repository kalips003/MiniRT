/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse_Obj_tree_split2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/05 13:09:28 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void			ft_split_by_xyz(t_model *model, t_bbox *node, int xyz);
static t_mtree	h_loop_split(t_bbox *node, int xyz, t_mtree t);
static double	find_median(t_bbox *node, int xyz);
static void		ft_find_smallest_biggest(t_model *model, t_bbox *node);
static void		h_big_small(t_bbox *node, t_coor *p);

///////////////////////////////////////////////////////////////////////////////]
// Recursively split by axis x/y/z
void	ft_split_by_xyz(t_model *model, t_bbox *node, int xyz)
{
	t_mtree	t;

	t.avg = find_median(node, xyz);
	ft_find_smallest_biggest(model, node);
	if (node->how_many_f < LEAF_NODE)
		return ;
	node->l = mem(0, sizeof(t_bbox));
	node->r = mem(0, sizeof(t_bbox));
	if (!node->l || !node->r)
		return (put(ERRM), (void)0);
	t = h_loop_split(node, xyz, t);
	if (t.ptr_l)
		t.ptr_l->next = NULL;
	if (t.ptr_r)
		t.ptr_r->next = NULL;
	node->f = NULL;
	ft_split_by_xyz(model, node->l, (xyz + 1) % 3);
	ft_split_by_xyz(model, node->r, (xyz + 1) % 3);
}

// send each triangles left or right
static t_mtree	h_loop_split(t_bbox *node, int xyz, t_mtree t)
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
// find the first bounding box value min max, from vertices
static void	ft_find_smallest_biggest(t_model *model, t_bbox *node)
{
	t_tri	*ptr;

	if (!node->f)
		return ;
	node->min = (t_coor){INFINITY, INFINITY, INFINITY};
	node->max = (t_coor){-INFINITY, -INFINITY, -INFINITY};
	ptr = node->f;
	while (ptr)
	{
		h_big_small(node, model->v[ptr->p[0]]);
		h_big_small(node, model->v[ptr->p[1]]);
		h_big_small(node, model->v[ptr->p[2]]);
		ptr = ptr->next;
	}
}

static void	h_big_small(t_bbox *node, t_coor *p)
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
