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

int				h_read_obj(t_data *data, t_model *model, int fd, t_object *obj);
static int		parse_obj_line(t_data *data, t_parse_model *p, char *line);
static t_coor	*h_v(char **spl, t_parse_model *p);
static t_vt		*h_vt(char **spl, t_parse_model *p);
static t_vect	*h_vn(char **spl, t_parse_model *p);

///////////////////////////////////////////////////////////////////////////////]
// loop over the .obj file, skip empty or commented lines
// expand the model with each line
int	h_read_obj(t_data *data, t_model *model, int fd, t_object *obj)
{
	t_parse_model	p;
	char			*line;

	ft_memset(&p, 0, sizeof(t_parse_model));
	p.obj = obj;
	p.model = model;
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
		if (parse_obj_line(data, &p, line))
			return (free_s(line), 1);
		line = free_s(line);
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// Parse ONE line of the .obj file: "v  21.9717 97.5478 -18.1949"
// update the model.struct with the content of 1 line
static int	parse_obj_line(t_data *data, t_parse_model *p, char *line)
{
	char	**spl;
	int		err;

	spl = split(line, " \n\r\t");
	if (!spl)
		return (put(ERRM), 1);
	err = 0;
	if (same_str(spl[0], "v"))
		p->model->v = (t_coor **)expand_tab((void **)p->model->v, h_v(spl, p));
	else if (same_str(spl[0], "vn"))
		p->model->vn = (t_vect **)expand_tab((void **)p->model->vn, \
			h_vn(spl, p));
	else if (same_str(spl[0], "vt"))
		p->model->vt = (t_vt **)expand_tab((void **)p->model->vt, h_vt(spl, p));
	else if (same_str(spl[0], "f"))
		p->current_tri = h_parse_face(spl, p, &err);
	else if (same_str(spl[0], "mtllib"))
		f_mtl_file(data, p->obj, spl);
	else if (same_str(spl[0], "usemtl"))
		p->current_mat = find_mat(spl[1], p->model);
	free_tab(spl);
	return (err);
}

///////////////////////////////////////////////////////////////////////////////]
// v->x = atof(spl[1]);
// v->y = atof(spl[2]);
// v->z = atof(spl[3]);
// if (ft_atof(spl[1], &v->x) || ft_atof(spl[2], &v->y) 
// 		|| ft_atof(spl[3], &v->z))
// 	return (put(ERR5"v ???\n"), NULL);
static t_coor	*h_v(char **spl, t_parse_model *p)
{
	t_coor	*v;

	v = mem(0, sizeof(t_coor));
	if (!v)
		return (put(ERRM"ft: h_v\n"), NULL);
	v->x = atof(spl[1]);
	v->y = atof(spl[2]);
	v->z = atof(spl[3]);
	p->num_v++;
	return (v);
}

///////////////////////////////////////////////////////////////////////////////]
// vt->u = atof(spl[1]);
// vt->v = atof(spl[2]);
// if (ft_atof(spl[1], &vt->u) || ft_atof(spl[2], &vt->v))
// 	return (put(ERR5"vt ???\n"), NULL);
static t_vt	*h_vt(char **spl, t_parse_model *p)
{
	t_vt	*vt;

	vt = mem(0, sizeof(t_vt));
	if (!vt)
		return (put(ERRM"ft: h_vt\n"), NULL);
	vt->u = atof(spl[1]);
	vt->v = atof(spl[2]);
	vt->u = fmin(1.0, fmax(0.0, vt->u));
	vt->v = fmin(1.0, fmax(0.0, vt->v));
	p->num_vt++;
	return (vt);
}

///////////////////////////////////////////////////////////////////////////////]
// vn->dx = atof(spl[1]);
// vn->dy = atof(spl[2]);
// vn->dz = atof(spl[3]);
// if (ft_atof(spl[1], &vn->dx) || ft_atof(spl[2], &vn->dy) 
// 		|| ft_atof(spl[3], &vn->dz))
// 	return (put(ERR5"vn ???\n"), NULL);
static t_vect	*h_vn(char **spl, t_parse_model *p)
{
	t_vect	*vn;

	vn = mem(0, sizeof(t_vect));
	if (!vn)
		return (put(ERRM"ft: h_vn\n"), NULL);
	vn->dx = atof(spl[1]);
	vn->dy = atof(spl[2]);
	vn->dz = atof(spl[3]);
	ft_normalize_vect(vn);
	p->num_vn++;
	return (vn);
}
