/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P3_param_parsing3_txt.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/03 14:25:13 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	txt_already_exist(t_data *data, char *path, t_img **txt);
int	parse_texture(t_data *data, char *path, t_param *obj);
int	parse_nmap(t_data *data, char *path, t_param *obj);
int	parse_amap(t_data *data, char *path, t_param *obj);
int	parse_ao(t_data *data, char *path, t_param *obj);

///////////////////////////////////////////////////////////////////////////////]
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
// (Texture) X=sphere_texture.xpm
int	parse_texture(t_data *data, char *path, t_param *obj)
{
	t_img	*texture;

	if (access(path, F_OK))
		return (printf(C_421"I cant open that: (%s)\n", path), 0);
	if (txt_already_exist(data, path, &texture))
	{
		obj->texture = texture;
		return (0);
	}
	texture = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, texture);
	if (!texture || !data->textures)
		return (put(ERRM"parse_texture\n"), 1);
	if (path)
		texture->img = mlx_xpm_file_to_image(data->mlx, path, &texture->sz_x, &texture->sz_y);
	if (!texture->img)
		return (put(ERR8"Cant open sprite: %s\n", path), perror(RED"mlx_xpm_file_to_image"), 1);
	texture->addr = mlx_get_data_addr(texture->img, &texture->bpp, &texture->ll, &texture->end);
	texture->path = str("%1s", path);
	obj->texture = texture;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Normal Map) N=sphere_normal.xpm
int	parse_nmap(t_data *data, char *path, t_param *obj)
{
	t_img	*nmap;

	if (access(path, F_OK))
		return (printf(C_421"I cant open that: (%s)\n", path), 0);
	if (txt_already_exist(data, path, &nmap))
	{
		obj->normal_map = nmap;
		return (0);
	}
	nmap = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, nmap);
	if (!nmap || !data->textures)
		return (put(ERRM"parse_texture\n"), 1);
	if (path)
		nmap->img = mlx_xpm_file_to_image(data->mlx, path, &nmap->sz_x, &nmap->sz_y);
	if (!nmap->img)
		return (put(ERR8"Cant open sprite: %s\n", path), perror(RED"mlx_xpm_file_to_image"), 1);
	nmap->addr = mlx_get_data_addr(nmap->img, &nmap->bpp, &nmap->ll, &nmap->end);
	nmap->path = str("%1s", path);
	obj->normal_map = nmap;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Alpha Map) A=sphere_normal.xpm
int	parse_amap(t_data *data, char *path, t_param *obj)
{
	t_img	*amap;

	if (access(path, F_OK))
		return (printf(C_421"I cant open that: (%s)\n", path), 0);
	if (txt_already_exist(data, path, &amap))
	{
		obj->alpha_map = amap;
		return (0);
	}
	amap = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, amap);
	if (!amap || !data->textures)
		return (put(ERRM"parse_texture\n"), 1);
	if (path)
		amap->img = mlx_xpm_file_to_image(data->mlx, path, &amap->sz_x, &amap->sz_y);
	if (!amap->img)
		return (put(ERR8"Cant open sprite: %s\n", path), perror(RED"mlx_xpm_file_to_image"), 1);
	amap->addr = mlx_get_data_addr(amap->img, &amap->bpp, &amap->ll, &amap->end);
	amap->path = str("%1s", path);
	obj->alpha_map = amap;
	return (0);
}

// (Ambient Occlusion) O=ao_normal.xpm
int	parse_ao(t_data *data, char *path, t_param *obj)
{
	t_img	*aomap;

	if (access(path, F_OK))
		return (printf(C_421"I cant open that: (%s)\n", path), 0);
	if (txt_already_exist(data, path, &aomap))
	{
		obj->ao_map = aomap;
		return (0);
	}
	aomap = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, aomap);
	if (!aomap || !data->textures)
		return (put(ERRM"parse_texture\n"), 1);
	if (path)
		aomap->img = mlx_xpm_file_to_image(data->mlx, path, &aomap->sz_x, &aomap->sz_y);
	if (!aomap->img)
		return (put(ERR8"Cant open sprite: %s\n", path), perror(RED"mlx_xpm_file_to_image"), 1);
	aomap->addr = mlx_get_data_addr(aomap->img, &aomap->bpp, &aomap->ll, &aomap->end);
	aomap->path = str("%1s", path);
	obj->ao_map = aomap;
	return (0);
}

