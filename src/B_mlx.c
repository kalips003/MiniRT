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

int	ft_loop(t_data *data);
int		key_press(int keysym, t_data *data);
int		key_release(int keysym, t_data *data);

///////////////////////////////////////////////////////////////////////////////]
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void	put_pixel_bufferv2(t_data *data, int x, int y, unsigned int color)
{
	char	*dst;
	int		offset;

	if (x < 0 || y < 0 || x >= data->buffer.sz_x || y >= data->buffer.sz_y)
		return ;
	offset = (y * data->buffer.ll + x * (data->buffer.bpp / 8));
	dst = data->buffer.addr + offset;
	*(unsigned int *)dst = color;
}

int	ft_loop(t_data *data)
{
	int x;
	int y = -1;
	double	angle_α;
	double	angle_β;
	t_vect v;
	t_rgb color;

	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		while (++x < SIZE_SCREEN_X)
		{
			// where α is a rotation around the Y axis == screen width
			// β is a rotation around the X axis == screen height
			angle_α = data->x0 + x * data->px;
			angle_β = data->x0 + y * data->px;
			//	a.cosα + b.sinα.sinβ + c.sinα.cosβ
			//	b.cosβ - c.sinβ
			//	-a.sinα + b.cosα.sinβ + c.cosα.cosβ
			v.dx = data->eye->abc.dx * cos(angle_α) + data->eye->abc.dy * sin(angle_α) * sin(angle_β) + data->eye->abc.dz * sin(angle_α) * cos(angle_β);
			v.dy = data->eye->abc.dy * cos(angle_β) - data->eye->abc.dz * sin(angle_β);
			v.dz = -data->eye->abc.dx * sin(angle_α) + data->eye->abc.dy * cos(angle_α) * sin(angle_β) + data->eye->abc.dz * cos(angle_α) * cos(angle_β);

			//>	for each object, calculate if there is colision, if there is, calcul the closest intersection point, retrun color?
			color = calculate_pixel_color(data, &v);
			// printf("color rgb: [%d,%d] %d,%d,%d   vector coor = %f,%f,%f\n", x, y, color.r, color.g, color.b, v.dx, v.dy, v.dz);
			//> apply color at pixel pos x.y
			mlx_pixel_put(data->mlx, data->win, x, y, color.r << 16 | color.g << 8 | color.b);
			// put_pixel_buffer(data, x, y, color);
		}
	}


	// mlx_put_image_to_window(data->mlx, data->win, data->buffer.img, 0, 0);

	return (0);
}

int	key_press(int keysym, t_data *data)
{
	if (keysym == XK_Escape)
		end2(data);
	// if (data->player.time)
	// 	return (0);
	// if (keysym == XK_d || keysym == XK_Right)
	// 	f_save_time_player(data, 1, 0, 0);
	// else if (keysym == XK_s || keysym == XK_Down)
	// 	f_save_time_player(data, 0, 1, 4);
	// else if (keysym == XK_a || keysym == XK_Left)
	// 	f_save_time_player(data, -1, 0, 8);
	// else if (keysym == XK_w || keysym == XK_Up)
	// 	f_save_time_player(data, 0, -1, 12);
	// else if (keysym == XK_space && !data->throw.ball.time)
	// 	f_save_time_ball(data);
	return (0);
}

int	key_release(int keysym, t_data *data)
{
	// if (keysym == XK_Control_L && !data->player.time)
	// 	data->running ^= 1;
	// if (keysym == XK_e)
	// 	print_data(data);
	// if (keysym == XK_space)
	// 	data->running = 0;
	return (0);
}
