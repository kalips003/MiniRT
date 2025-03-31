/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P2_obj_parsing2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/30 10:22:31 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	parse_hy(t_data *data, char **raw_split);
int	parse_pa(t_data *data, char **raw_split);
int	parse_ar(t_data *data, char **raw_split);
int	parse_xi(t_data *data, char **raw_split);
int	parse_obj(t_data *data, char **raw_split);

///////////////////////////////////////////////////////////////////////////////]
// 			ARROW
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		RADIUS = float
// 		HEIGHT = float
// 		RGB [0, 255] int
int	parse_ar(t_data *data, char **raw_split)
{
	t_arrow	*arrow;

	arrow = mem(0, sizeof(t_arrow));
	if (!arrow)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, arrow);
	if (tab_size(raw_split) < 5)
		return (put(ERR1"bad number of args (ARROW OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[5], &arrow->param))
		return (1);
	arrow->type = ARROW;
	if (ato_coor(raw_split[0], &arrow->O.c0)
		|| ato_coor(raw_split[1], (t_coor *)&arrow->O.view)
		|| ft_atof(raw_split[2], &arrow->radius)
		|| ft_atof(raw_split[3], &arrow->h)
		|| ato_argb(raw_split[4], &arrow->param.argb))
		return (1);
	if (arrow->radius < EPSILON || arrow->h < EPSILON)
		return (put(ERR1"(ARROW OBJECT) too small\n"), 1);
	if (h_parse_vect_space(&arrow->O, &arrow->O.view))
		return (1);
	recalculate_obj_const((t_obj2 *)arrow);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			HYPERBOLOID
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		abc args float
// 		Radius	double
// 		RGB [0, 255] int
int	parse_hy(t_data *data, char **raw_split)
{
	t_hyper	*hyp;

	hyp = mem(0, sizeof(t_hyper));
	if (!hyp)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, hyp);
	if (tab_size(raw_split) < 5)
		return (put(ERR1"bad number of args (HYPERBOLOID OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[5], &hyp->param))
		return (1);
	hyp->type = HYPERBOLOID;
	if (ato_coor(raw_split[0], &hyp->O.c0)
		|| ato_coor(raw_split[1], (t_coor*)&hyp->O.view)
		|| ato_coor(raw_split[2], &hyp->abc)
		|| ft_atof(raw_split[3], &hyp->radius)
		|| ato_argb(raw_split[4], &hyp->param.argb))
		return (1);
	if (h_parse_vect_space(&hyp->O, &hyp->O.view))
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			PARABOLOID
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		abc args float
// 		RGB [0, 255] int
int	parse_pa(t_data *data, char **raw_split)
{
	t_hyper	*para;

	para = mem(0, sizeof(t_hyper));
	if (!para)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, para);
	if (tab_size(raw_split) < 4)
		return (put(ERR1"bad number of args (HYPERBOLOID OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[4], &para->param))
		return (1);
	para->type = PARABOLOID;
	if (ato_coor(raw_split[0], &para->O.c0)
		|| ato_coor(raw_split[1], (t_coor*)&para->O.view)
		|| ato_coor(raw_split[2], &para->abc)
		|| ato_argb(raw_split[3], &para->param.argb))
		return (1);
	if (h_parse_vect_space(&para->O, &para->O.view))
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			SPRITE
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		Size float
// 		RGB [0, 255] int
// 
int	parse_xi(t_data *data, char **raw_split)
{
	t_sprite	*sprite;

	sprite = mem(0, sizeof(t_sprite));
	if (!sprite)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, sprite);
	if (tab_size(raw_split) < 4)
		return (put(ERR1"bad number of args (SPRITE OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[4], &sprite->param))
		return (1);
	if (!sprite->param.txt)
		return (put(ERR1"SPRITE OBJECT need a sprite\n"), 1);
	sprite->type = SPRITE;
	if (ato_coor(raw_split[0], &sprite->O.c0)
		|| ato_coor(raw_split[1], (t_coor*)&sprite->O.view)
		|| ft_atof(raw_split[2], &sprite->size)
		|| ato_argb(raw_split[3], &sprite->param.argb))
		return (1);
	if (h_parse_vect_space(&sprite->O, &sprite->O.view))
		return (1);
	sprite->d = -(sprite->O.view.dx * sprite->O.c0.x + sprite->O.view.dy \
		* sprite->O.c0.y + sprite->O.view.dz * sprite->O.c0.z);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			OBJECTS
// 		XYZ = float
// 		vector VIEW [-1,1] float
// 		double	size-
// 		color	ARGB [0, 255] int
// 		.obj	file
int	parse_obj(t_data *data, char **raw_split)
{
	t_object	*obj;

	obj = mem(0, sizeof(t_object));
	if (!obj)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, obj);
	if (tab_size(raw_split) < 5)
		return (put(ERR2"bad number of args (OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[5], &obj->param))
		return (1);
	obj->type = OBJECT;
	if (ato_coor(raw_split[0], &obj->O.c0)
		|| ato_coor(raw_split[1], (t_coor *)&obj->O.view)
		|| ft_atof(raw_split[2], &obj->size)
		|| ato_argb(raw_split[3], &obj->param.argb))
		return (1);
	if (obj->size < EPSILON)
		return (put(ERR2"(OBJECT) too small\n"), 1);
	obj->model = rtrn_obj(data, raw_split[4], obj);
	if (!obj->model)
		return (put(ERR2"(OBJECT) model parsing error\n"), 1);
	if (h_parse_vect_space(&obj->O, &obj->O.view))
		return (1);
	return (0);
}
