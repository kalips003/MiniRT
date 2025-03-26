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

void		end(t_data *data, int exit_code, int full_clean);
int			end2(t_data *data);
static void	destroy_textures(t_data *data);
static void	destroy_models(t_data *data);
static void	*ft_free_list(t_model *model);

///////////////////////////////////////////////////////////////////////////////]
void	end(t_data *data, int exit_code, int full_clean)
{
	data->bg = (t_ambient_light **)free_tab((char **)data->bg);
	data->camera = (t_camera **)free_tab((char **)data->camera);
	data->light_source = (t_light **)free_tab((char **)data->light_source);
	data->objects = free_tab((char **)data->objects);
	data->eye.c = NULL;
	destroy_textures(data);
	destroy_models(data);
	if (full_clean && data->mlx)
	{
		if (data->buffer.img)
			mlx_destroy_image(data->mlx, data->buffer.img);
		if (data->buff_aa.img)
			mlx_destroy_image(data->mlx, data->buff_aa.img);
		if (data->win)
			mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free_s(data->mlx);
	}
	if (exit_code)
		exit(exit_code);
}

int	end2(t_data *data)
{
	free_tab((char **)data->bg);
	free_tab((char **)data->camera);
	free_tab((char **)data->light_source);
	free_tab((char **)data->objects);
	destroy_textures(data);
	destroy_models(data);
	if (data->buffer.img)
		mlx_destroy_image(data->mlx, data->buffer.img);
	if (data->buff_aa.img)
		mlx_destroy_image(data->mlx, data->buff_aa.img);
	if (data->mlx)
	{
		if (data->win)
			mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free_s(data->mlx);
	}
	exit(0);
	return (0);
}

static void	destroy_textures(t_data *data)
{
	t_img	**curseur;

	curseur = data->textures;
	while (curseur && *curseur)
	{
		if ((*curseur)->img)
			mlx_destroy_image(data->mlx, (*curseur)->img);
		free_s((*curseur)->path);
		curseur++;
	}
	data->textures = (t_img **)free_tab((char **)data->textures);
}

///////////////////////////////////////////////////////////////////////////////]
// 				CLEAN UP MODELS
///////////////////////////////////////////////////////////////////////////////]
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
// static void	destroy_models(t_data *data)
// {
// 	t_model	**curseur;

// 	curseur = data->models;
// 	while (curseur && *curseur)
// 	{
// 		free_tab((char **)(*curseur)->v);
// 		free_tab((char **)(*curseur)->vn);
// 		free_tab((char **)(*curseur)->vt);
// 		if ((*curseur)->first)
// 			ft_free_list(*curseur);
// 		free_tab((char **)(*curseur)->mat);
// 		free_s((*curseur)->path);
// 		curseur++;
// 	}
// 	data->models = (t_model **)free_tab((char **)data->models);
// }
static void	destroy_models(t_data *data)
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

// free linked list of triangles + names of all mat
static void	*ft_free_list(t_model *model)
{
	t_tri	*curseur;
	t_tri	*tmp_value;
	t_mat	**ptr;

	// curseur = model->first;
	while (curseur)
	{
		tmp_value = curseur;
		curseur = tmp_value->next;
		free_s(tmp_value);
	}
	ptr = model->mat - 1;
	while (++ptr && *ptr)
		free_s((*ptr)->name);
	return (NULL);
}
