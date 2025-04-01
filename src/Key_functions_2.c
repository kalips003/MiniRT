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

void	f_set_color(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_move_obj(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_toogle_cam(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_render_normal_arrow(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_progressive_rt(t_data *data, t_obj2 *obj, int k_or_loop);

///////////////////////////////////////////////////////////////////////////////]
void	f_set_color(t_data *data, t_obj2 *obj, int k_or_loop)
{
	char	*input;
	int		i;
	int		err;

	(void)k_or_loop;
	if (!obj)
		return (put(C_515"First, select an object (Right Clic)\n"), (void)0);
	printf(C_515"Input the object's color in this format:\t\' R,G,B \'\n"RESET);
	input = gnl(0);
	if (!input)
		return ;
	i = 0;
	err = 0;
	obj->param.argb.r = min(255, max(0, ft_atoi_v2(input, &i, &err)));
	i += (input[i] == ',');
	obj->param.argb.g = min(255, max(0, ft_atoi_v2(input, &i, &err)));
	i += (input[i] == ',');
	obj->param.argb.b = min(255, max(0, ft_atoi_v2(input, &i, &err)));
	free_s(input);
	ft_render_frame_multi(data, RENDERING_LVL);
}

///////////////////////////////////////////////////////////////////////////////]
void	f_move_obj(t_data *data, t_obj2 *obj, int k_or_loop)
{
	char	*read;

	if (!k_or_loop)
	{
		printf(C_451"Starting Object Movement, Select a direction\n");
		read = gnl(0);
		if (ato_coor(read, (t_coor *)&data->ram))
			return (free_s(read), printf(RED"you f*ing donkey!\n"), (void)0);
		free_s(read);
	}
	else if (!obj)
		return ;
	else
	{
		obj->O.c0 = new_moved_point(&obj->O.c0, (t_vect *)&data->ram, 1);
		recalculate_obj_const(obj);
	}
	ft_render_frame_multi(data, RENDERING_LVL);
}

///////////////////////////////////////////////////////////////////////////////]
void	f_toogle_cam(t_data *data, t_obj2 *obj, int k_or_loop)
{
	(void)obj;
	if (k_or_loop)
		return ;
	if (data->f_cam == VECTOR)
		printf(C_451"Changing: Camera calculs with Quaternions\n");
	else if (data->f_cam == QUATERNION)
		printf(C_451"Changing: Camera calculs with Vectors\n");
	data->f_cam ^= 1;
	ft_render_frame_multi(data, RENDERING_LVL);
}

///////////////////////////////////////////////////////////////////////////////]
void	f_render_normal_arrow(t_data *data, t_obj2 *obj, int k_or_loop)
{
	(void)data;
	(void)obj;
	if (!k_or_loop)
		printf(C_451"Click an Object to see the normal\n");
}

///////////////////////////////////////////////////////////////////////////////]
// progressive RT
void	f_progressive_rt(t_data *data, t_obj2 *obj, int k_or_loop)
{
	(void)obj;
	if (k_or_loop != 1)
	{
		if (!k_or_loop)
			printf(C_451"Starting Progressive RT (Press Space)\n");
		clean_buffer(data);
		*(int *)&data->ram = 0;
		return ;
	}
	ft_render_frame_multi_prog(data, RENDERING_LVL);
	(*(int *)&data->ram)++;
	printf("loop: %d\n", *(int *)&data->ram);
}
