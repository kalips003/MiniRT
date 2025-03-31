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

///////////////////////////////////////////////////////////////////////////////]
void	do_the_tree_splitting(t_model *model)
{
	ft_give_centroid(model, &model->tree);
	ft_split_by_xyz(model, &model->tree, X);
}

///////////////////////////////////////////////////////////////////////////////]
// give centroid to each triangle
static void	ft_give_centroid(t_model *model, t_bbox *node)
{
	t_coor	**v;
	t_tri	*t;

	v = model->v;
	t = node->f;
	while (t)
	{
		t->centroid.x = (v[t->p[0]]->x + v[t->p[1]]->x + v[t->p[2]]->x) / 3;
		t->centroid.y = (v[t->p[0]]->y + v[t->p[1]]->y + v[t->p[2]]->y) / 3;
		t->centroid.z = (v[t->p[0]]->z + v[t->p[1]]->z + v[t->p[2]]->z) / 3;
		t = t->next;
	}
}
