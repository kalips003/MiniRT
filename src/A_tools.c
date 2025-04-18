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

t_img	*parse_img(t_data *data, char *path);
int		txt_already_exist(t_data *data, char *path, t_img **txt);
int		clamp(int value, int min_v, int max_v);
t_mat	*find_mat(char *mat_name, t_model *model);

///////////////////////////////////////////////////////////////////////////////]
t_img	*parse_img(t_data *data, char *path)
{
	t_img	*txt;

	if (access(path, F_OK))
		return (printf(C_421"I cant open that: (%s)\n", path), NULL);
	if (txt_already_exist(data, path, &txt))
		return (txt);
	txt = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, txt);
	if (!txt || !data->textures)
		return (put(ERRM"parse_texture\n"), NULL);
	if (path)
		txt->img = mlx_xpm_file_to_image(data->mlx, path, \
			&txt->sz_x, &txt->sz_y);
	if (!txt->img)
		return (put(ERR8"Cant open sprite: %s\n", path), \
			perror(RED"mlx"), NULL);
	txt->addr = mlx_get_data_addr(txt->img, &txt->bpp, &txt->ll, &txt->end);
	txt->path = str("%1s", path);
	return (txt);
}

int	txt_already_exist(t_data *data, char *path, t_img **txt)
{
	int	i;

	i = -1;
	while (data->textures && data->textures[++i])
	{
		if (same_str(data->textures[i]->path, path))
		{
			*txt = data->textures[i];
			return (1);
		}
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	clamp(int value, int min_v, int max_v)
{
	value = min(max_v, max(min_v, value));
	return (value);
}

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
