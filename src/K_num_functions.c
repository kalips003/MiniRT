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

void	f_anti_aliasing(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_change_transp(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_loop_light_ratio(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_set_color(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_move_obj(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_toogle_cam(t_data *data, t_obj2 *obj, int k_or_loop);

///////////////////////////////////////////////////////////////////////////////]
void	f_anti_aliasing(t_data *data, t_obj2 *obj, int k_or_loop)
{
	(void)obj;
	(void)k_or_loop;
	ft_render_frame_aa(data, RENDERING_LVL);
}

///////////////////////////////////////////////////////////////////////////////]
// funcction de transformatioin basee sur un tableau, touches "-" et "+" swap through
void	f_change_transp(t_data *data, t_obj2 *obj, int k_or_loop)
{
	if (!obj)
		return ;
	else if (!k_or_loop)
	{
		printf(C_451"Starting Transparance Loop, Transparence set to 1 (Press Space)\n");
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
		data->light_source[0]->ratio = 1.0;
	}
	else
	{
		data->light_source[0]->ratio -= (1.0 / 60.0);
		if (data->light_source[0]->ratio < EPSILON)
			data->light_source[0]->ratio = 1.0;
	}
	ft_render_frame_multi(data, RENDERING_LVL);
}

///////////////////////////////////////////////////////////////////////////////]
void	f_set_color(t_data *data, t_obj2 *obj, int k_or_loop)
{
	char	*input;
	int		i;
	int		err;

	if (!obj)
		return ;
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
	if (!k_or_loop)
	{
		printf(C_451"Starting Object Movement, Select a direction\n");
		char *read = gnl(0);
		if (ato_coor(read, (t_coor*)&data->ram))
			return (free_s(read), printf(RED"you f*ing donkey!\n"), (void)0);
		free_s(read);
		// ft_normalize_vect((t_coor*)&data->ram);
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

/*
void	f_change_color_random(t_data *data, t_obj2 *obj, int k_or_loop)
{
	if (!k_or_loop)
	{
		printf(C_451"Starting Transparance Loop, Transparence set to 1\n");
		return ;
	}
	if (!data->change_obj)
		return ;
	if (!data->ram[0])
	{
		data->ram[0] = 1;
	}
	obj->param.argb.a = (obj->param.argb.r + 1) % 256;

}

void	f_move_up_down(t_data *data, t_obj2 *obj)
{
	if (!data->ram[0])
	{
		printf(C_451"Starting Bobbling\n");
		*(t_coor*)&data->ram[1];
		// y = base_y + amplitude * sin(phase);
		// phase += frequency;
	}

	ft_memset(data->ram, 0, sizeof(data->ram));
	data->ram[0] = 1;
	*(t_coor*)&data->ram[1] = new_moved_point(&obj->O.c0, &obj->O.right, 3.0);


}

void	f_move_in_circle(t_data *data, t_obj2 *obj)
{
	if (!data->ram[0])
	{
		printf(C_451"Center of rotation:\n");
	}

	ft_memset(data->ram, 0, sizeof(data->ram));
	data->ram[0] = 1;
	*(t_coor*)&data->ram[1] = new_moved_point(&obj->O.c0, &obj->O.right, 3.0);

}


void	f_rotate_light_source(t_data *data, t_obj2 *obj)
{
	ft_memset(data->ram, 0, sizeof(data->ram));
	data->ram[0] = 1;
	*(t_coor*)&data->ram[1] = new_moved_point(&obj->O.c0, &obj->O.right, 3.0);

}


unsigned int	ft_r_yellow(void)
{
	int	color;

	color = (rand() % (255 - 30 + 1)) + 30;
	return (color << 16 | color << 8);
}

///////////////////////////////////////////////////////////////////////////////]


*/