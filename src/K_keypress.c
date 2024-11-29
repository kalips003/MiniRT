/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   K_keypress.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/11/29 03:50:39 by kalipso          ###   ########.fr       */
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
	if (keysym == XK_KP_Enter)
	{
		data->is_not_moving = 1;
		ft_render_rt(data);
		return (0);
	}
	else
		data->is_not_moving = 0;

	if (keysym == XK_Up)
	{
		data->e.c->xyz.x += 5.0 * data->e.c->view.dx;
		data->e.c->xyz.y += 5.0 * data->e.c->view.dy;
		data->e.c->xyz.z += 5.0 * data->e.c->view.dz;
	}
	else if (keysym == XK_Down)
	{
		data->e.c->xyz.x -= 5.0 * data->e.c->view.dx;
		data->e.c->xyz.y -= 5.0 * data->e.c->view.dy;
		data->e.c->xyz.z -= 5.0 * data->e.c->view.dz;
	}
	else if (keysym == XK_Right)
	{
		data->e.c->xyz.x += 5.0 * data->e.c->right.dx;
		data->e.c->xyz.y += 5.0 * data->e.c->right.dy;
		data->e.c->xyz.z += 5.0 * data->e.c->right.dz;
	}
	else if (keysym == XK_Left)
	{
		data->e.c->xyz.x -= 5.0 * data->e.c->right.dx;
		data->e.c->xyz.y -= 5.0 * data->e.c->right.dy;
		data->e.c->xyz.z -= 5.0 * data->e.c->right.dz;
	}

	else if (keysym == XK_d)
		rotation_camera(data, &data->e.c->up, 1);
	else if (keysym == XK_a)
		rotation_camera(data, &data->e.c->up, -1);
	else if (keysym == XK_s)
		rotation_camera(data, &data->e.c->right, -1);
	else if (keysym == XK_w)
		rotation_camera(data, &data->e.c->right, 1);
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
