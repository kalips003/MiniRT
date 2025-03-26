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

/*



void	clean_buffer(t_data *data);
int 	ft_loop_progressive(t_data *data);
int	ft_render_random_px(t_data *data, int sublim, t_camera *cam);
void	put_pixel_buffer_v2(t_img *buffer, int x, int y, unsigned int color);
unsigned int	ft_new_average_color(t_img *buffer, int x, int y, t_argb new_ray);

///////////////////////////////////////////////////////////////////////////////]
void	clean_buffer(t_data *data)
{
	int		x;
	int		y;
	char	*dst;
	int		offset;

	y = -1;
	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		while (++x < SIZE_SCREEN_X)
		{
			offset = (y * data->buffer.ll + x * (data->buffer.bpp / 8));
			dst = data->buffer.addr + offset;
			*(unsigned int *)dst = 0;
		}
	}
	mlx_put_image_to_window(data->mlx, data->win, data->buffer.img, 0, 0);
}

// ray bacck trackinng loop
int 	ft_loop_progressive(t_data *data)
{
	struct stat	file_stat;
	
	if (stat(data->av[1], &file_stat) == 0)
	{
		if (file_stat.st_mtime != data->last_modif_time)
		{
			// h_refresh_input_file(data, file_stat.st_mtime);
			clean_buffer(data);
		}
	}
	else
		perror("stat");
	// sennnd ranndom ray


	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	ft_render_random_px(t_data *data, int sublim, t_camera *cam)
{
	t_calcul_px	c;

	ft_memset(&c, 0, sizeof(t_calcul_px));
	ini_stack(data, &c);
	c.c0 = data->eye.c->O.c0;

	data->change = 1;
	while (data->change)
	{
		int			x = rand() % SIZE_SCREEN_X;
		int			y = rand() % SIZE_SCREEN_Y;
		double		angleA = atan((x - SIZE_SCREEN_X / 2) * cam->fov_cst);
		double		angleB = atan((y - SIZE_SCREEN_Y / 2) * cam->fov_cst);

		c.transparence_depth = 0; 
		c.reflected_depth = 0; 
		c.v = combined_quaternion_rotation(&cam->O, angleA, angleB);
		calculate_pixel_color(data, &c, 3);
		unsigned int av_color = ft_new_average_color(&data->buffer, x, y, c.argb);
		mlx_pixel_put(data->mlx, data->win, x, y, av_color);
	}
	return (0);
}


///////////////////////////////////////////////////////////////////////////////]
// read the pixel xy, weight it by scalar
void	put_pixel_buffer_v2(t_img *buffer, int x, int y, unsigned int color)
{
	char	*dst;
	int		offset;

	offset = (y * buffer->ll + x * (buffer->bpp / 8));
	dst = buffer->addr + offset;
	*(unsigned int *)dst = color;
}

///////////////////////////////////////////////////////////////////////////////]
unsigned int	ft_new_average_color(t_img *buffer, int x, int y, t_argb new_ray)
{
	unsigned int color;
	color = *(unsigned int *)(buffer->addr + (y * buffer->ll + x * (buffer->bpp / 8)));

	int px_iteration = ((color >> 24) & 0xFF);

	t_rgb	current_px;
	current_px.r = (((color >> 16) & 0xFF) * px_iteration + new_ray.r) / (px_iteration + 1);
	current_px.g = (((color >> 8) & 0xFF) * px_iteration + new_ray.g) / (px_iteration + 1);
	current_px.b = ((color & 0xFF) * px_iteration + new_ray.b) / (px_iteration + 1);

	color = min(px_iteration + 1, 255) << 24 | current_px.r << 16 | current_px.g << 8 | current_px.b;
	put_pixel_buffer_v2(buffer, x, y, color);
	return (color);
}


*/