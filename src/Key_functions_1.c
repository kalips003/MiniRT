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

int		fuctions_number_pad(int keysym, t_data *data);
void	ft_print_help(void);
void	f_anti_aliasing(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_change_transp(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_loop_light_ratio(t_data *data, t_obj2 *obj, int k_or_loop);

///////////////////////////////////////////////////////////////////////////////]
static const t_dico_pair2	g_numpad_fuctions[] = {
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
int	fuctions_number_pad(int keysym, t_data *data)
{
	int	key;

	if (keysym >= XK_0 && keysym <= XK_9)
	{
		key = wii(keysym, "0123456789");
		printf(C_042"Selected function: ("C_420" %d "C_042")\n"RESET, key);
		printf(C_321"\t%s\n"RESET, g_numpad_fuctions[key].name);
		if (!g_numpad_fuctions[key].exe)
			return (printf(C_042"Selected function: ("C_420 \
				" - IS EMPTY - "C_042")\n"RESET), 1);
		if (data->change_function != g_numpad_fuctions[key].exe)
			data->ram[0] = 0;
		data->change_function = g_numpad_fuctions[key].exe;
		g_numpad_fuctions[key].exe(data, data->change_obj, 0);
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

///////////////////////////////////////////////////////////////////////////////]
void	ft_print_help(void)
{
	int	i;

	printf(CLEAR C_042"HELP:\n"RESET);
	i = -1;
	while (++i < 10)
		printf(C_042"FUNCTION ("C_420" %d "C_042"): "C_411"%s\n"RESET, \
			i, g_numpad_fuctions[i].name);
}

///////////////////////////////////////////////////////////////////////////////]
void	f_anti_aliasing(t_data *data, t_obj2 *obj, int k_or_loop)
{
	(void)obj;
	(void)k_or_loop;
	ft_render_frame_aa(data, RENDERING_LVL);
}

///////////////////////////////////////////////////////////////////////////////]
// Loop transparence
void	f_change_transp(t_data *data, t_obj2 *obj, int k_or_loop)
{
	if (!obj)
		return ;
	else if (!k_or_loop)
	{
		printf(C_451"Starting Transparance Loop, Transparence set to 1 \
			(Press Space)\n");
		obj->param.transparence = 1;
	}
	else
	{
		obj->param.transparence -= (1.0 / 60.0);
		if (obj->param.transparence < EPSILON)
			obj->param.transparence = 1.0;
	}
	ft_render_frame_multi(data, RENDERING_LVL);
}

///////////////////////////////////////////////////////////////////////////////]
void	f_loop_light_ratio(t_data *data, t_obj2 *obj, int k_or_loop)
{
	if (!k_or_loop)
	{
		printf(C_451"Starting Light Loop, Ratio set to 1\n");
		data->light[0]->ratio = 1.0;
	}
	else
	{
		data->light[0]->ratio -= (1.0 / 60.0);
		if (data->light[0]->ratio < EPSILON)
			data->light[0]->ratio = 1.0;
	}
	ft_render_frame_multi(data, RENDERING_LVL);
}
