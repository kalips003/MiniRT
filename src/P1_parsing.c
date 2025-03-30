/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P1_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/30 10:16:58 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	ft_parse_line(t_data *data, char *line);
int	h_parse_vect_space(t_obj *obj, t_vect *view);
int	parse_a(t_data *data, char **raw_split);
int	parse_l(t_data *data, char **raw_split);
int	parse_c(t_data *data, char **raw_split);

static const t_dico_pair	dico[] = {
{"A", parse_a},
{"L", parse_l},
{"C", parse_c},
{"ci", parse_ci},
{"sp", parse_sp},
{"pl", parse_pl},
{"cy", parse_cy},
{"co", parse_co},
{"hy", parse_hy},
{"ar", parse_ar},
{"xi", parse_xi},
{"ob", parse_obj},
{NULL, NULL}
};

///////////////////////////////////////////////////////////////////////////////]
// input line = "# C 	0,0,-100		0,0,1	70"
int	ft_parse_line(t_data *data, char *line)
{
	char	**params;
	int		i;

	if (line[0] == '\n' || line[0] == '#')
		return (free_s(line), 0);
	params = split(line, " \t\n#");
	free_s(line);
	if (!params)
		return (put(ERRM"split\n"), 2);
	i = -1;
	while (dico[++i].name)
	{
		if (same_str(params[0], dico[i].name))
		{
			if (dico[i].exe(data, params + 1))
				return (free_tab(params), put(ERR3"parsing error\n"), 1);
			break ;
		}
	}
	if (!dico[i].name)
		return (put(ERR4"Unknown object: "C_441"%.3t\n"RESET, params), free_tab(params), 1);
	free_tab(params);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// check is object vector is too big, create vector space while at it
int	h_parse_vect_space(t_obj *obj, t_vect *view)
{
	if (view)
	{
		if (view->dx < -1.0 || view->dx > 1.0
			|| view->dy < -1.0 || view->dy > 1.0
			|| view->dz < -1.0 || view->dz > 1.0)
			return (put(ERR1"Vector should be [-1.0,1.0]\n"), 1);
		ft_normalize_vect(view);
		create_vector_space(obj);
	}
	else
		*obj = (t_obj){obj->c0, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, {1.0, 0.0, 0.0}};
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
// 			AMBIENT LIGHT
// 		ambient lighting ratio [0.0,1.0] float
// 		ARGB [0, 255] int
int	parse_a(t_data *data, char **raw_split)
{
	t_ambient_light	*light;

	light = mem(0, sizeof(t_ambient_light));
	if (!light)
		return (put(ERRM), 2);
	data->bg = (t_ambient_light **)expand_tab((void **)data->bg, light);
	if (tab_size(raw_split) < 2 || tab_size(raw_split) > 3)
		return (put(ERR1"bad number of args (AMBIENT LIGHT)\n"), 1);
	if (ft_atof(raw_split[0], &light->ratio)
		|| ato_argb(raw_split[1], &light->rgb)
		|| parse_bg_texture(data, raw_split[2], &light->texture))
		return (1);
	if (light->ratio < 0.0 || light->ratio > 1.0)
		return (put(ERR1"(%s) ambient lighting ratio should be [0.0,1.0]\n", raw_split[0]), 1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			LIGHT
// 		XYZ = float
// 		BRIGHTNESS [0.0,1.0] float
// 		ARGB [0, 255] int
int	parse_l(t_data *data, char **raw_split)
{
	t_light	*light;

	light = mem(0, sizeof(t_camera));
	if (!light)
		return (put(ERRM), 2);
	data->light_source = (t_light **)expand_tab((void **)data->light_source, light);
	if (tab_size(raw_split) != 3)
		return (put(ERR1"bad number of args (LIGHT SOURCE)\n"), 1);
	if (ato_coor(raw_split[0], &light->xyz)
		|| ft_atof(raw_split[1], &light->ratio)
		|| ato_argb(raw_split[2], &light->color))
		return (1);
	if (light->ratio < 0.0 || light->ratio > 1.0)
		return (put(ERR1"(%s) light source ratio should be [0.0,1.0]\n", raw_split[1]), 1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			CAMERA
// 		position XYZ = float
// 		vector camera [-1,1] float
// 		FOV [0, 180] int
int	parse_c(t_data *data, char **raw_split)
{
	t_camera	*camera;
	int			err;

	camera = mem(0, sizeof(t_camera));
	if (!camera)
		return (put(ERRM), 2);
	data->camera = (t_camera **)expand_tab((void **)data->camera, camera);
	if (tab_size(raw_split) != 3)
		return (put(ERR1"bad number of args (CAMERA)\n"), 1);
	err = 0;
	camera->fov = ft_atoi(raw_split[2], &err);
	if (err || ato_coor(raw_split[0], &camera->O.c0)
		|| ato_coor(raw_split[1], (t_coor*)&camera->O.view))
		return (1);
	if (camera->fov < 0 || camera->fov > 180)
		return (put(ERR1"(%s) camera fov should be [0, 180]\n", raw_split[0]), 1);
	if (h_parse_vect_space(&camera->O, &camera->O.view))
		return (1);
	camera->fov_cst = tan(camera->fov * PI / 360) / (SZ_X / 2.0);
	return (0);
}
