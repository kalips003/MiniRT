/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   K_keypress.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/11/26 12:58:40 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		key_press(int keysym, t_data *data);
int		key_release(int keysym, t_data *data);

///////////////////////////////////////////////////////////////////////////////]
int	key_press(int keysym, t_data *data)
{
	if (keysym == XK_Escape)
		end2(data);
	// if (data->player.time)
	// 	return (0);
	if (keysym == XK_KP_Enter)
	{
		data->is_not_moving = 1;
		ft_render_rt(data);
		return (0);
	}
	else
		data->is_not_moving = 0;

	if (keysym == XK_Up)
		data->eye->xyz.z += 15.0;
	else if (keysym == XK_Down)
		data->eye->xyz.z -= 15.0;
	else if (keysym == XK_Right)
		data->eye->xyz.x += 15.0;
	else if (keysym == XK_Left)
		data->eye->xyz.x -= 15.0;
	else if (keysym == XK_d)
		data->eye->view.dx += 15 * data->e.px;
	else if (keysym == XK_s)
		data->eye->view.dz -= 15 * data->e.px;
	else if (keysym == XK_a)
		data->eye->view.dx -= 15 * data->e.px;
	else if (keysym == XK_w)
		data->eye->view.dz += 15 * data->e.px;
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
	// if (keysym == XK_d || keysym == XK_Right || 
	// keysym == XK_s || keysym == XK_Down || 
	// keysym == XK_a || keysym == XK_Left ||
	// keysym == XK_w || keysym == XK_Up || keysym == XK_n)
	// 	ft_render_rt(data);
	// if (keysym == XK_Control_L && !data->player.time)
	// 	data->running ^= 1;
	// if (keysym == XK_e)
	// 	print_data(data);
	// if (keysym == XK_space)
	// 	data->running = 0;
	return (0);
}
