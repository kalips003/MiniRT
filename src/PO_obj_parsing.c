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

///////////////////////////////////////////////////////////////////////////////]
// take the "model/tea.obj", expand model tab, return ptr to model
t_model	*rtrn_obj(t_data *data, char *path, t_object *obj)
{
	int		dot;
	int		fd;
	t_model	*model;

	dot = wii('.', path);
	if (dot == -1 || !same_str(".obj", &path[dot]))
		(put(ERR1"Objects require .obj argument\n"), end(data, 1, 1));
	if (model_already_exist(data, path, &model))
		return (model);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		(perror(ERR8"Cant open file"), end(data, 1, 1));
	model = mem(0, sizeof(t_model));
	if (!model)
		return (put(ERRM"?\n"), end(data, 1, 1), close(fd), NULL);
	data->models = (t_model **)expand_tab((void **)data->models, model);
	obj->model = model;
	printf(C_431"LOADING FILE... (%s)\n", path);
	if (h_read_obj(data, model, fd, obj))
		(close(fd), end(data, 1, 1));
	close(fd);
	do_the_tree_splitting(model);
	printf(C_341"FINISHED LOADING FILE!\n");
	model->path = str("%1s", path);
	return (model);
}

///////////////////////////////////////////////////////////////////////////////]
// skip all the openinng of .obj and such if the model already is in memory
static int	model_already_exist(t_data *data, char *path, t_model **model)
{
	int	i;

	i = -1;
	while (data->models && data->models[++i])
	{
		if (same_str(data->models[i]->path, path))
		{
			*model = data->models[i];
			return (1);
		}
	}
	return (0);
}

typedef struct	s_parse_model
{
	char		*line;

	t_model		*model;
	t_object	*obj;
	t_tri		*current_tri;
	t_mat		*current_mat;
}	t_parse_model;

///////////////////////////////////////////////////////////////////////////////]
// loop over the .obj file, skip empty or commented lines
// expand the model with each line
static int	h_read_obj(t_data *data, t_model *model, int fd, t_object *obj)
{
	
	
	
	char	*line;
	t_tri	*current_tri;
	t_mat	*current_mat;

	current_mat = NULL;
	current_tri = NULL;
	while (1)
	{
		line = gnl(fd);
		if (!line)
			break ;
		if (wii(line[0], "#\n\t\r ") >= 0)
		{
			line = free_s(line);
			continue ;
		}
		if (parse_obj_line(data, line, model, &current_tri, obj, &current_mat))
			return (free_s(line), 1);
		line = free_s(line);
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// Parse ONE line of the .obj file: "v  21.9717 97.5478 -18.1949"
// update the model.struct with the content of 1 line
static int	parse_obj_line(t_data *data, char *line, t_model *model, t_tri **current_model, t_object *obj, t_mat **current_mat)
{
	char	**spl;

	spl = split(line, " \n\r\t");
	if (!spl)
		return (put(ERRM), 1);
	if (same_str(spl[0], "v"))
		model->v = (t_coor **)expand_tab((void **)model->v, h_v(spl, obj->size));
	else if (same_str(spl[0], "vn"))
		model->vn = (t_vect **)expand_tab((void **)model->vn, h_vn(spl));
	else if (same_str(spl[0], "vt"))
		model->vt = (t_vt **)expand_tab((void **)model->vt, h_vt(spl));
	else if (same_str(spl[0], "f"))
		*current_model = h_parse_face(spl, model, *current_model, *current_mat);
	else if (same_str(spl[0], "mtllib"))
		f_mtl_file(data, obj, spl);
	else if (same_str(spl[0], "usemtl"))
		*current_mat = find_mat(spl[1], model);
	free_tab(spl);
	return (0);
}
