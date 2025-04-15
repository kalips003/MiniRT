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

void			f_mtl_file(t_data *data, t_object *obj, char **spl);
static t_mat	*h_ini_mat(t_data *data, char *mat_name, t_object *obj, int fd);
static int		h_read_newmtl(t_data *data, int fd, t_mat *mat);
static int		h_mtl_param1(t_data *data, char **spl, t_mat *mat);
static int		h_mtl_param2(t_data *data, char **spl, t_mat *mat);

///////////////////////////////////////////////////////////////////////////////]
// recieve "mtllib Tree_low.mtl" as a tab, open and read it
// expand the tab: obj->model->mat, with every materials in the .mtl file
void	f_mtl_file(t_data *data, t_object *obj, char **spl)
{
	int		fd;
	char	*line;
	char	*path;

	if (tab_size(spl) != 2 || find_str(spl[1], ".mtl") <= 0)
		return (put(ERR6"Bad .mtl file (%.1t)\n", spl), (void)0);
	path = str("model/%1s", spl[1]);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (put(ERR8"(%s)\n", path), perror("Cant open file"), \
			free_s(path), (void)0);
	free_s(path);
	while (1)
	{
		line = gnl(fd);
		if (!line)
			break ;
		if (wii(line[0], "#\n\r ") == -1)
			if (!find_str(line, "newmtl "))
				obj->model->mat = \
					(t_mat **)expand_tab((void **)obj->model->mat, \
					h_ini_mat(data, &line[7], obj, fd));
		line = free_s(line);
	}
	close(fd);
}

///////////////////////////////////////////////////////////////////////////////]
// copy the OBJECT param into the new malloc'ed mat
static t_mat	*h_ini_mat(t_data *data, char *mat_name, t_object *obj, int fd)
{
	t_mat	*mat;
	char	**spl;

	mat = mem(0, sizeof(t_mat));
	if (!mat)
		return (put(ERRM"\n"), NULL);
	spl = split(mat_name, "\n\r");
	mat->argb = obj->param.argb;
	mat->sp = obj->param.specular;
	mat->sh = obj->param.shiny;
	mat->mi = obj->param.mirror;
	mat->tr = obj->param.transparence;
	mat->gamma = obj->param.gamma;
	mat->light = obj->param.light;
	mat->name = str("%1s", spl[0]);
	free_tab(spl);
	if (h_read_newmtl(data, fd, mat))
		put(ERR1"???\n");
	return (mat);
}

///////////////////////////////////////////////////////////////////////////////]
// recieve the fd pointing at: "newmtl Material__49"
// keep reading until nno more lines in the current newmtl
static int	h_read_newmtl(t_data *data, int fd, t_mat *mat)
{
	char	*line;
	char	**spl;

	while (1)
	{
		line = gnl(fd);
		if (!line || wii(line[0], "#\n\r ") >= 0)
			break ;
		spl = split(line, "\t \n\r");
		if (!spl)
			return (put(ERRM"\n"), free_s(line), 1);
		if (h_mtl_param1(data, spl, mat))
			put(ERR1"h_mtl_param???\n");
		spl = free_tab(spl);
		line = free_s(line);
	}
	line = free_s(line);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// read one line of the newmtl: "	Kd 0.5880 0.5880 0.5880"
// recieve the split of that line
// 
// Ns → Specular exponent (shininess)
// Ni → Index of refraction
// Tr → Transparency (alternative to d)
// d → Dissolve (opacity, d = 1.0 - Tr)
// Ka → Ambient color (RGB)
// Kd → Diffuse color (RGB)
// Ks → Specular color (RGB)
// Ke → Emissive color (RGB, self-illumination)
// map_Ka → Texture map for ambient color
// map_Kd → Texture map for diffuse color
// map_Ks → Texture map for specular color
// map_Ns → Texture map for specular exponent
// map_d → Texture map for dissolve (opacity)
// map_bump / bump → Normal map (bump mapping)
static int	h_mtl_param1(t_data *data, char **spl, t_mat *mat)
{
	int		err;
	double	dbl;

	err = 0;
	if (same_str(spl[0], "Ns"))
		err = ft_atof(spl[1], &mat->sh);
	else if (same_str(spl[0], "Ni"))
		err = ft_atof(spl[1], &mat->gamma);
	else if (same_str(spl[0], "Tr"))
	{
		err = ft_atof(spl[1], &dbl);
		mat->argb.a = min(255, max(0, (int)(dbl * 255.0)));
	}
	else if (same_str(spl[0], "Kd"))
	{
		err = ft_atof(spl[1], &dbl);
		mat->argb.r = min(255, max(0, (int)(dbl * 255.0)));
		err |= ft_atof(spl[2], &dbl);
		mat->argb.g = min(255, max(0, (int)(dbl * 255.0)));
		err |= ft_atof(spl[3], &dbl);
		mat->argb.b = min(255, max(0, (int)(dbl * 255.0)));
	}
	else
		return (h_mtl_param2(data, spl, mat));
	return (err);
}

static int	h_mtl_param2(t_data *data, char **spl, t_mat *mat)
{
	int		err;
	char	*path;
	t_img	**img;

	err = 0;
	if (same_str(spl[0], "Ka"))
		return (err = ft_atof(spl[1], &mat->ao), err);
	if (same_str(spl[0], "map_Kd"))
		img = &mat->txt;
	else if (same_str(spl[0], "map_d"))
		img = &mat->a_map;
	else if (same_str(spl[0], "map_Ka"))
		img = &mat->ao_map;
	else if (same_str(spl[0], "map_Pr"))
		img = &mat->rough_map;
	else if (same_str(spl[0], "map_bump"))
		img = &mat->n_map;
	else
		return (err);
	path = str("model/%1s", spl[1]);
	*img = parse_img(data, path);
	if (!*img && ++err)
		printf(C_421"no txt?: %s\n", path);
	free_s(path);
	return (err);
}
