/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P3_param_parsing1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/30 10:52:31 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	parse_reste(t_data *data, char **raw_split, t_param *obj);
int	parse_bg_texture(t_data *data, char *path, t_img **bg_txt);

#define PARAM_DICO "STMXNACLOH"

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// (Shininess) S=[1.0, +]
// (Transparence, gamma) T=[0,1],[1.0, 2.5+]
// (Mirroir) M=[0,1]
// (Texture) X=sphere_texture.xpm
// (Normal Map) N=sphere_normal.xpm
// (Alpha Map) A=sphere_alpha.xpm
// (Color 2) C=255,0,0
// (Light) L=[0,1]
static const t_ft_param_parsing	g_ft_param_parsing[] = {
	parse_shininess,
	parse_transparence,
	parse_mirror,
	parse_texture,
	parse_nmap,
	parse_amap,
	parse_color2,
	parse_light,
	parse_ao,
	parse_hmap
};

int	parse_reste(t_data *data, char **raw_split, t_param *obj)
{
	int	index;

	obj->gamma = 1.0;
	obj->specular = 0.5;
	obj->shiny = 32.0;
	obj->c2.r = -1;
	obj->ao = 1.0;
	while (raw_split && *raw_split)
	{
		index = wii(**raw_split, PARAM_DICO);
		if (index < 0)
			return (put(ERR7"UNKNNOWN PARAMETER (%s)\n", *raw_split), 1);
		if ((*raw_split)[1] && (*(raw_split))[2])
			if (g_ft_param_parsing[index](data, &(*raw_split)[2], obj))
				return (1);
		raw_split++;
	}
	return (0);
}

int	parse_bg_texture(t_data *data, char *path, t_img **bg_txt)
{
	t_img	*texture;

	if (!path || !path[0])
		return (0);
	if (path[0] != 'X' || !path[1] || !path[2])
		return (put("UNKNOWN BACKGROUND\n"), 1);
	texture = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, texture);
	if (!texture || !data->textures)
		return (put(ERRM"parse_bg_texture\n"), 1);
	texture->img = mlx_xpm_file_to_image(data->mlx, &path[2], &texture->sz_x, &texture->sz_y);
	if (!texture->img)
		return (put(ERR8"Cant open sprite: %s\n", &path[2]), perror(RED"mlx_xpm_file_to_image"), 1);
	texture->addr = mlx_get_data_addr(texture->img, &texture->bpp, &texture->ll, &texture->end);
	*bg_txt = texture;
	return (0);
}
