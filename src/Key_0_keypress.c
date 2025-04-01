/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Key_0_keypress.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 09:56:07 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		key_press(int keysym, t_data *data);
int		mouse_clic(int button, int x, int y, void *data);
int		mouse_move(int x, int y, void *d);
int		mouse_release(int button, int x, int y, void *d);

///////////////////////////////////////////////////////////////////////////////]
int	key_press(int keysym, t_data *data)
{
	if (keysym == XK_Escape)
		end2(data);
	if (keysym == XK_KP_Enter)
		return (ft_cat_timing(data, 2));
	if (direction_pad(keysym, data))
		return (ft_render_frame_multi(data, RENDERING_LVL));
	else if (fuctions_number_pad(keysym, data))
		return (0);
	else if (keys_wasd(keysym, data))
		return (ft_render_frame_multi(data, RENDERING_LVL));
	else if (extra_keys(keysym, data))
		return (ft_render_frame_multi(data, RENDERING_LVL));
	else
		return (0);
	ft_render_frame_multi(data, RENDERING_LVL);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// Left Button: 1, Middle: 2, Right: 3, Scroll Up: 4, Scroll Down: 5
int	mouse_clic(int button, int x, int y, void *data)
{
	if (button == 3)
		print_clic((t_data *)data, x, y);
	else if (button == 1)
	{
		((t_data *)data)->eye.clic_x = x;
		((t_data *)data)->eye.clic_y = y;
		((t_data *)data)->in_movement = 1;
	}
	else if (button == 4 || button == 5)
	{
		if (button == 4)
			((t_data *)data)->zoom *= 2;
		else
			((t_data *)data)->zoom /= 2;
		printf(C_411"MOVEMENT: "RED"%.3f\n"RESET, ((t_data *)data)->zoom);
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// Left Button: 1, Middle: 2, Right: 3, Scroll Up: 4, Scroll Down: 5
int	mouse_move(int x, int y, void *d)
{
	t_data	*data;
	double	a;
	double	b;
	int		sca;

	data = (t_data *)d;
	if (data->in_movement && data->eye.clic_x != x && data->eye.clic_y != y)
	{
		sca = 1 + !!(data->change_obj) * -3;
		a = 1.5 * sca * (data->eye.clic_x - x) * data->eye.c->fov_cst;
		b = 1.5 * sca * (data->eye.clic_y - y) * data->eye.c->fov_cst;
		data->eye.c->O.view = dbl_quaternion_rota(&data->eye.c->O, a, b);
		if (data->change_obj)
			data->eye.c->O.c0 = new_moved_point(&data->change_obj->O.c0, \
				&data->eye.c->O.view, -dist_two_points(&data->eye.c->O.c0, \
				&data->change_obj->O.c0));
		create_vector_space(&data->eye.c->O);
		if (data->change_function == f_progressive_rt && data->change)
			f_progressive_rt(data, NULL, 2);
		else
			ft_render_frame_multi(data, RENDER_MOVING_MOUSE);
		data->eye.clic_x = x;
		data->eye.clic_y = y;
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	mouse_release(int button, int x, int y, void *d)
{
	t_data	*data;
	double	a;
	double	b;
	int		scalar;

	data = (t_data *)d;
	data->in_movement = 0;
	if (button == 1)
	{
		scalar = 1 + !!(data->change_obj) * -3;
		a = 1.5 * scalar * (data->eye.clic_x - x) * data->eye.c->fov_cst;
		b = 1.5 * scalar * (data->eye.clic_y - y) * data->eye.c->fov_cst;
		data->eye.c->O.view = dbl_quaternion_rota(&data->eye.c->O, a, b);
		if (data->change_obj)
			data->eye.c->O.c0 = new_moved_point(&data->change_obj->O.c0, \
				&data->eye.c->O.view, -dist_two_points(&data->eye.c->O.c0, \
				&data->change_obj->O.c0));
		create_vector_space(&data->eye.c->O);
		if (data->change_function == f_progressive_rt && data->change)
			f_progressive_rt(data, NULL, 2);
		else
			ft_render_frame_multi(data, RENDERING_LVL);
	}
	return (0);
}
