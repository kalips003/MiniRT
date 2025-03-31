/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Key_1_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/31 08:18:34 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		ft_cat_timing(t_data *data, int sublim);
int		direction_pad(int keysym, t_data *data);
int		keys_wasd(int keysym, t_data *data);
int		fuctions_number_pad(int keysym, t_data *data);

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
	return (recalculate_obj_const(data->change_obj), 1);
}

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
	else if (keysym == XK_n)
	{
		data->eye.current_camera++;
		if (!data->camera[data->eye.current_camera])
			data->eye.current_camera = 0;
		data->eye.c = data->camera[data->eye.current_camera];
	}
	else
		return (0);
	return (recalculate_obj_const(data->change_obj), 1);
}

///////////////////////////////////////////////////////////////////////////////]
static const t_dico_pair2	numpad_fuctions[] = {
{C_413"ANTI ALISING"RESET, f_anti_aliasing},
{"Loop the selected object transparence", f_change_transp},
{"Loop the first light's ratio", f_loop_light_ratio},
{"Set the Color of the selected object", f_set_color},
{"Move the selected object in given direction", f_move_obj},
{"Toogle Cam: Vector / Quaternion", f_toogle_cam},
{"Render the normal at clicked point", f_render_normal_arrow},
{"Progressively does magic", f_progressive_rt},
{"TEMP8", NULL},
{"TEMP9", NULL}
};

///////////////////////////////////////////////////////////////////////////////]
void	ft_print_help(void)
{
	int	i;

	printf(CLEAR C_042"HELP:\n"RESET);
	i = -1;
	while (++i < 10)
		printf(C_042"FUNCTION ("C_420" %d "C_042"): "C_411"%s\n"RESET, i, numpad_fuctions[i].name);
}

///////////////////////////////////////////////////////////////////////////////]
int	fuctions_number_pad(int keysym, t_data *data)
{
	int	key;

	if (keysym >= XK_0 && keysym <= XK_9)
	{
		key = wii(keysym, "0123456789");
		printf(C_042"Selected function: ("C_420" %d "C_042")\n"RESET, key);
		printf(C_321"\t%s\n"RESET, numpad_fuctions[key].name);
		if (!numpad_fuctions[key].exe)
			return (printf(C_042"Selected function: ("C_420 \
				" - IS EMPTY - "C_042")\n"RESET), 1);
		if (data->change_function != numpad_fuctions[key].exe)
			data->ram[0] = 0;
		data->change_function = numpad_fuctions[key].exe;
		numpad_fuctions[key].exe(data, data->change_obj, 0);
	}
	else if (keysym == XK_plus || keysym == XK_KP_Add)
	{
		data->change_function = NULL;
		printf(C_042"Selected function: ("C_420" - CANCELED - "C_042")\n"RESET);
	}
	else
		return (0);
	return (1);
}
