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

t_model			*rtrn_obj(t_data *data, char *path, t_object *obj);
static int		model_already_exist(t_data *data, char *path, t_model **model);
t_tri			*h_parse_face(char **spl, t_parse_model *p, int *err);
static t_tri	*add_tri(t_tri *prev, char **spl, t_parse_model *p, int *err);
static int		h_vvtvn(char *v, t_tri *tri, int point, t_parse_model *p);

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

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
// handle: faces: "f 45/20/17 55/21/17 56/22/17 12/13/17 11/12/17 46/23/17"
t_tri	*h_parse_face(char **spl, t_parse_model *p, int *err)
{
	t_tri	*new_tri;
	int		poly_size;
	int		i;

	poly_size = tab_size(spl) - 1;
	if (poly_size < 3)
		return (p->current_tri);
	new_tri = add_tri(p->current_tri, \
		(char *[]){spl[1], spl[2], spl[3]}, p, err);
	if (!p->model->tree.f)
		p->model->tree.f = new_tri;
	i = 2;
	while (++i < poly_size)
		new_tri = add_tri(new_tri, \
			(char *[]){spl[1], spl[i], spl[i + 1]}, p, err);
	return (new_tri);
}

///////////////////////////////////////////////////////////////////////////////]
// "45/20/17"
static t_tri	*add_tri(t_tri *prev, char **spl, t_parse_model *p, int *err)
{
	t_tri	*new_tri;

	new_tri = mem(0, sizeof(t_tri));
	if (!new_tri)
		return (put(ERRM"ft: expand_tab_coor\n"), ++(*err), prev);
	*err = h_vvtvn(*spl, new_tri, 0, p);
	*err |= h_vvtvn(*(spl + 1), new_tri, 1, p);
	*err |= h_vvtvn(*(spl + 2), new_tri, 2, p);
	if (new_tri->p[0] < 0 || new_tri->p[1] < 0 || new_tri->p[2] < 0)
		return (printf(ERR"bad polygon\n"), free_s(new_tri), ++(*err), prev);
	new_tri->mat = p->current_mat;
	if (prev)
		prev->next = new_tri;
	return (new_tri);
}

// static void	h_vvtvn(char *v, t_tri *tri, int point, t_parse_model *p)
// {
// 	int	i;
// 	int	err;

// 	i = 0;
// 	err = 0;
// 	tri->p[point] = ft_atoi_v2(v, &i, &err) - 1;
// 	i += (v[i] == '/');
// 	tri->vt[point] = ft_atoi_v2(v, &i, &err) - 1;
// 	i += (v[i] == '/');
// 	tri->vn[point] = ft_atoi_v2(v, &i, &err) - 1;
// 	return (0);
// }

///////////////////////////////////////////////////////////////////////////////]
static int	h_vvtvn(char *v, t_tri *tri, int point, t_parse_model *p)
{
	int	i;
	int	err;
	int	num;

	i = 0;
	err = 0;
	num = ft_atoi_v2(v, &i, &err);
	if (num > p->num_v)
		return (printf(ERR8"f has v too big: %d (num v: %d)\n", \
			num, p->num_v), 1);
	tri->p[point] = num - 1;
	i += (v[i] == '/');
	num = ft_atoi_v2(v, &i, &err);
	if (num > p->num_vt)
		return (printf(ERR8"f has vt too big: %d (num vt: %d)\n", \
			num, p->num_vt), 1);
	tri->vt[point] = num - 1;
	i += (v[i] == '/');
	num = ft_atoi_v2(v, &i, &err);
	if (num > p->num_vn)
		return (printf(ERR8"f has vn too big: %d (num vn: %d)\n", \
			num, p->num_vn), 1);
	tri->vn[point] = num - 1;
	return (0);
}
