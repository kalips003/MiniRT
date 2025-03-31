/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Z_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/25 14:09:51 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void		destroy_models(t_data *data);
static void	ft_free_tree(t_bbox *node);
static void	ft_free_triangles(t_tri *f);

///////////////////////////////////////////////////////////////////////////////]
// 				CLEAN UP MODELS
///////////////////////////////////////////////////////////////////////////////]
void	destroy_models(t_data *data)
{
	t_model	**curseur;

	curseur = data->models;
	while (curseur && *curseur)
	{
		free_tab((char **)(*curseur)->v);
		free_tab((char **)(*curseur)->vn);
		free_tab((char **)(*curseur)->vt);
		ft_free_triangles((*curseur)->tree.f);
		ft_free_tree((*curseur)->tree.l);
		ft_free_tree((*curseur)->tree.r);
		free_tab((char **)(*curseur)->mat);
		free_s((*curseur)->path);
		curseur++;
	}
	data->models = (t_model **)free_tab((char **)data->models);
}

// Recursively free the tree
static void	ft_free_tree(t_bbox *node)
{
	if (!node)
		return ;
	ft_free_triangles(node->f);
	ft_free_tree(node->l);
	ft_free_tree(node->r);
	free_s(node);
}

// free chained list of triangles in leaf node
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
