/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/14 11:14:55 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	ft_loop_empty(t_data *data);
int	ft_loop_empty_v2(t_data *data);
int	ft_render_frame(t_data *data);
int	ft_render_frame_plus(t_data *data);
void	calculate_pixel_color_simple(t_data *data, t_calcul_px *c);
void	calculate_pixel_color_plus(t_data *data, t_calcul_px *c);

///////////////////////////////////////////////////////////////////////////////]
// main loop doesnt need to do anything, just open for key press
int 	ft_loop_empty(t_data *data)
{
	(void)data;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// main loop refresh if file is changed
int 	ft_loop_empty_v2(t_data *data)
{
	struct stat file_stat;
	
	if (stat(data->av[1], &file_stat) == 0)
	{
		if (file_stat.st_mtime != data->last_modif_time)
		{
			usleep(100);
			data->last_modif_time = file_stat.st_mtime;
			put("REFRESH\n");
			end(data, 0, 0);
			read_file(2, data->av, data);
			data->eye.c = data->camera[0];
			data->current_camera = 0;
			ft_render_frame(data);
		}
	}
	else
		perror("stat");
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// render simple shadows, uses adjusted by tan angle
// used to render while moving
int	ft_render_frame(t_data *data)
{
	int y;
	int x;
	double angleA;
	double angleB;
	t_calcul_px c;

	ft_memset(&c, 0, sizeof(t_calcul_px));
	c.c0 = data->eye.c->xyz;

	y = -1;
	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		angleB = atan((y - SIZE_SCREEN_Y / 2) * data->eye.c->fov_cst_y);
		while (++x < SIZE_SCREEN_X)
		{
			angleA = atan((x - SIZE_SCREEN_X / 2) * data->eye.c->fov_cst_x);
			f_calculate_combined_quaternion(data, angleA, angleB, &c.v_view);
			calculate_pixel_color_simple(data, &c);
			mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
		}
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	ft_render_frame_plus(t_data *data)
{
	int y;
	int x;
	double angleA;
	double angleB;
	t_calcul_px c;

	ft_memset(&c, 0, sizeof(t_calcul_px));
	c.c0 = data->eye.c->xyz;

	y = -1;
	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		angleB = atan((y - SIZE_SCREEN_Y / 2) * data->eye.c->fov_cst_y);
		while (++x < SIZE_SCREEN_X)
		{
			angleA = atan((x - SIZE_SCREEN_X / 2) * data->eye.c->fov_cst_x);
			f_calculate_combined_quaternion(data, angleA, angleB, &c.v_view);
			calculate_pixel_color_plus(data, &c);
			mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
		}
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
void	calculate_pixel_color_simple(t_data *data, t_calcul_px *c)
{
	if (!ft_find_pixel_colision(data, c))
	{
		c->px_color.r = (int)(round(data->bg_light[0]->color.r * data->bg_light[0]->ratio));
		c->px_color.g = (int)(round(data->bg_light[0]->color.g * data->bg_light[0]->ratio));
		c->px_color.b = (int)(round(data->bg_light[0]->color.b * data->bg_light[0]->ratio));
	}
	else
		ft_handle_shadows_simple(data, c);
}

///////////////////////////////////////////////////////////////////////////////]
void	calculate_pixel_color_plus(t_data *data, t_calcul_px *c)
{
	if (!ft_find_pixel_colision(data, c))
	{
		c->px_color.r = (int)(round(data->bg_light[0]->color.r * data->bg_light[0]->ratio));
		c->px_color.g = (int)(round(data->bg_light[0]->color.g * data->bg_light[0]->ratio));
		c->px_color.b = (int)(round(data->bg_light[0]->color.b * data->bg_light[0]->ratio));
	}
	else
		ft_handle_shadows_plus(data, c);
}
