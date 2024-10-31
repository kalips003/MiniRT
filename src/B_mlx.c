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
int	ft_loop(t_data *data)
{
	int x = 0;
	int y = 0;

	while (++y < SIZEY)
		while (++x < SIZEX)
		{
			
		}

	mlx_put_image_to_window(data->mlx, data->win, data->buffer.img, 0, 0);

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
