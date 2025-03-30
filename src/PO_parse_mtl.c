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

t_mat			*find_mat(char *mat_name, t_model *model);
void			f_mtl_file(t_data *data, t_object *obj, char **spl);
static t_mat	*h_ini_mat(t_data *data, char *mat_name, t_object *obj, int fd);
static int		h_ini_mat2(t_data *data, int fd, t_mat *mat);
static int		h_ini_mat3(t_data *data, char **spl, t_mat *mat);

///////////////////////////////////////////////////////////////////////////////]
t_mat	*find_mat(char *mat_name, t_model *model)
{
	t_mat	**ptr;

	ptr = model->mat - 1;
	while (++ptr && *ptr)
	{
		if (same_str(mat_name, (*ptr)->name))
			return (*ptr);
	}
	return (NULL);
}

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
	printf("OPENING (%s)\n", path);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (put(ERR8"(%s)\n", path), perror("Cant open file"), free_s(path), (void)0);
	free_s(path);
	while (1)
	{
		line = gnl(fd);
		if (!line)
			break ;
		if (wii(line[0], "#\n\r ") == -1)
			if (!find_str(line, "newmtl "))
				obj->model->mat = (t_mat **)expand_tab((void **)obj->model->mat, h_ini_mat(data, &line[7], obj, fd));
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
	if (h_ini_mat2(data, fd, mat))
		put(ERR1"???\n");
	return (mat);
}

// recieve the fd pointing at: "newmtl Material__49"
// keep reading until nno more lines in the current newmtl
static int	h_ini_mat2(t_data *data, int fd, t_mat *mat)
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
		if (h_ini_mat3(data, spl, mat))
			put(ERR1"????\n");
		spl = free_tab(spl);
		line = free_s(line);
	}
	line = free_s(line);
	return (0);
}

// read one line of the newmtl: "	Kd 0.5880 0.5880 0.5880"
// recieve the split of that line
static int	h_ini_mat3(t_data *data, char **spl, t_mat *mat)
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
	else if (same_str(spl[0], "map_Kd"))
	{
		
		char *path = str("model/%1s", spl[1]);
		mat->txt = parse_img(data, path);
		if (!mat->txt)
		{
			printf(C_421"no txt?: %s\n", path);
			err++;
		}
		free_s(path);
	}
	return (err);
}

