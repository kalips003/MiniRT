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

int 	ft_loop_empty(t_data *data);
int		key_press(int keysym, t_data *data);
int	key_release(int keysym, t_data *data);

///////////////////////////////////////////////////////////////////////////////]
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int 	ft_loop_empty(t_data *data)
{
	(void)data;
	return 0;
}


int	key_press(int keysym, t_data *data)
{
	if (keysym == XK_Escape)
		end2(data);
	// if (data->player.time)
	// 	return (0);
	if (keysym == XK_Right)
		data->camera[0]->xyz.x += 15.0;
	else if (keysym == XK_Down)
		data->camera[0]->xyz.z -= 15.0;
	else if (keysym == XK_Left)
		data->camera[0]->xyz.x += -15.0;
	else if (keysym == XK_Up)
		data->camera[0]->xyz.z += 15.0;
	else if (keysym == XK_d)
		data->camera[0]->abc.dx += 15 * data->px;
	else if (keysym == XK_s)
		data->camera[0]->abc.dz -= 15 * data->px;
	else if (keysym == XK_a)
		data->camera[0]->abc.dx -= 15 * data->px;
	else if (keysym == XK_w)
		data->camera[0]->abc.dz += 15 * data->px;
	else if (keysym == XK_n)
	{
		data->current_camera++;
		data->eye = data->camera[data->current_camera];
		if (!data->eye)
		{
			data->eye = data->camera[0];
			data->current_camera = 0;
		}
	}
	// else if (keysym == XK_space && !data->throw.ball.time)
	// 	f_save_time_ball(data);
	return (0);
}

int	key_release(int keysym, t_data *data)
{
	if (keysym == XK_d || keysym == XK_Right || 
	keysym == XK_s || keysym == XK_Down || 
	keysym == XK_a || keysym == XK_Left ||
	keysym == XK_w || keysym == XK_Up || keysym == XK_n);
		ft_render_rt(data);
	// if (keysym == XK_Control_L && !data->player.time)
	// 	data->running ^= 1;
	// if (keysym == XK_e)
	// 	print_data(data);
	// if (keysym == XK_space)
	// 	data->running = 0;
	return (0);
}
