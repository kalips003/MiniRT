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

void			find_inter_tri(t_bbox *node, t_model *model, t_c_obj *c, t_c_px *calcul);
void			h_find_tri(t_bbox *node, t_model *model, t_c_obj *c);
static int		f_check_if_in_box_2(t_bbox *bbox, t_c_obj *c);
static void		h_bound_mmax(double mmxyz[2][3], int xyz, t_bbox *bbox, t_c_obj *c);

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
		h_find_tri(node, model, c);
		return ;
	}
	if (f_check_if_in_box_2(node, c))
	{
		find_inter_tri(node->l, model, c, calcul);
		find_inter_tri(node->r, model, c, calcul);
	}
}

// does collision check for all triangles in the leaf node
void	h_find_tri(t_bbox *node, t_model *model, t_c_obj *c)
{
	t_tri	*ptr;
	double	temp_dist;

	ptr = node->f;
	while (ptr)
	{
		temp_dist = h_dist_triangle(ptr, model, c);
		if (temp_dist > EPSILON && (temp_dist < c->dist || c->dist < 0))
		{
			c->t = ptr;
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

	t_min = (((double *)&bbox->min)[xyz] * c->size - \
		((double *)&c->new_o)[xyz]) / ((double *)&c->v_rotate)[xyz];
	t_max = (((double *)&bbox->max)[xyz] * c->size - \
		((double *)&c->new_o)[xyz]) / ((double *)&c->v_rotate)[xyz];
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
