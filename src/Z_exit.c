/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Z_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2024/10/31 16:36:56 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	end(t_data *data, int exit_code);
int	end2(t_data *data);

///////////////////////////////////////////////////////////////////////////////]
void	end(t_data *data, int exit_code)
{
	free_tab((char **)data->light);
	free_tab((char **)data->camera);
	free_tab((char **)data->light_source);
	free_tab((char **)data->spheres);
	free_tab((char **)data->planes);
	free_tab((char **)data->cylinders);
	if (data->buffer.img)
		mlx_destroy_image(data->mlx, data->buffer.img);
	if (data->mlx)
	{
		if (data->win)
			mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free_s(data->mlx);
	}
	exit(exit_code);
}

int	end2(t_data *data)
{
	free_tab((char **)data->light);
	free_tab((char **)data->camera);
	free_tab((char **)data->light_source);
	free_tab((char **)data->spheres);
	free_tab((char **)data->planes);
	free_tab((char **)data->cylinders);
	if (data->buffer.img)
		mlx_destroy_image(data->mlx, data->buffer.img);
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