/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Z_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2024/12/13 13:22:00 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	end(t_data *data, int exit_code, int full_clean);
int	end2(t_data *data);

///////////////////////////////////////////////////////////////////////////////]
void	end(t_data *data, int exit_code, int full_clean)
{
	data->bg_light = free_tab((char **)data->bg_light);
	data->camera = free_tab((char **)data->camera);
	data->light_source = free_tab((char **)data->light_source);
	data->spheres = free_tab((char **)data->spheres);
	data->planes = free_tab((char **)data->planes);
	data->cylinders = free_tab((char **)data->cylinders);
	data->eye.c = NULL;

	// if (data->buffer.img)
	// 	mlx_destroy_image(data->mlx, data->buffer.img);
	if (full_clean && data->mlx)
	{
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
	free_tab((char **)data->bg_light);
	free_tab((char **)data->camera);
	free_tab((char **)data->light_source);
	free_tab((char **)data->spheres);
	free_tab((char **)data->planes);
	free_tab((char **)data->cylinders);
	// if (data->buffer.img)
	// 	mlx_destroy_image(data->mlx, data->buffer.img);
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
