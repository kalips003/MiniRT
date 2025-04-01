/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Key_1_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 09:54:53 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		ft_cat_timing(t_data *data, int sublim);
int		direction_pad(int keysym, t_data *data);
int		keys_wasd(int keysym, t_data *data);
int		extra_keys(int keysym, t_data *data);

///////////////////////////////////////////////////////////////////////////////]
int	ft_cat_timing(t_data *data, int sublim)
{
	struct timeval	start;
	struct timeval	end;
	double			time;

	gettimeofday(&start, NULL);
	ft_render_frame_multi(data, sublim);
	gettimeofday(&end, NULL);
	time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
	printf(C_423"Run-time: "C_135"%.4f"C_423" seconds\n", time);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// move pad
int	direction_pad(int keysym, t_data *data)
{
	t_coor	*xyz;

	if (data->change_obj)
		xyz = &data->change_obj->O.c0;
	else
		xyz = &data->eye.c->O.c0;
	if (keysym == XK_Home)
		move_point(xyz, &data->eye.c->O.up, data->zoom);
	else if (keysym == XK_End)
		move_point(xyz, &data->eye.c->O.up, -data->zoom);
	else if (keysym == XK_Up)
		move_point(xyz, &data->eye.c->O.view, data->zoom);
	else if (keysym == XK_Down)
		move_point(xyz, &data->eye.c->O.view, -data->zoom);
	else if (keysym == XK_Right)
		move_point(xyz, &data->eye.c->O.right, data->zoom);
	else if (keysym == XK_Left)
		move_point(xyz, &data->eye.c->O.right, -data->zoom);
	else
		return (0);
	if (data->change_function == f_progressive_rt && data->change)
		return (f_progressive_rt(data, NULL, 2), 0);
	return (recalculate_obj_const(data->change_obj), 1);
}

///////////////////////////////////////////////////////////////////////////////]
// rotation pad
int	keys_wasd(int keysym, t_data *data)
{
	t_obj	*obj;

	if (data->change_obj)
		obj = &data->change_obj->O;
	else
		obj = &data->eye.c->O;
	if (keysym == XK_a)
		rotation_obj(obj, &obj->up, DELTA_ROTA, 1);
	else if (keysym == XK_d)
		rotation_obj(obj, &obj->up, DELTA_ROTA, -1);
	else if (keysym == XK_w)
		rotation_obj(obj, &obj->right, DELTA_ROTA, 1);
	else if (keysym == XK_s)
		rotation_obj(obj, &obj->right, DELTA_ROTA, -1);
	else if (keysym == XK_q)
		rotation_obj(obj, &obj->view, DELTA_ROTA, 1);
	else if (keysym == XK_e)
		rotation_obj(obj, &obj->view, DELTA_ROTA, -1);
	else
		return (0);
	if (data->change_function == f_progressive_rt && data->change)
		return (f_progressive_rt(data, NULL, 2), 0);
	return (recalculate_obj_const(data->change_obj), 1);
}

int	extra_keys(int keysym, t_data *data)
{
	if (keysym == XK_n)
	{
		data->eye.current_camera++;
		if (!data->camera[data->eye.current_camera])
			data->eye.current_camera = 0;
		data->eye.c = data->camera[data->eye.current_camera];
		return (1);
	}
	else if (keysym == XK_space)
		data->change ^= 1;
	else if (keysym == 0xff08)
		data->change_function = NULL;
	else if (keysym == XK_h)
		ft_print_help();
	return (0);
}
