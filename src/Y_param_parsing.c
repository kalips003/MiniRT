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

int	parse_reste(t_data *data, char **raw_split, void *obj);
static double	ft_parse_shininess(char *raw);
static double	ft_parse_transparence(char *raw, double *gamma);
static t_img	*helper_texture(t_data *data, char *path);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// (Shininess) S=20.0
// (Transparence, gamma) T=[0,1],[-PI/2, PI/2]
// (Mirroir) M=[0,1]
// (Texture) X="sphere_texture.xpm"
// (Normal Map) N="sphere_normal.xpm"
int	parse_reste(t_data *data, char **raw_split, void *obj)
{
	while (raw_split && *raw_split)
	{
		if (**raw_split == 'S')
			((t_sphere *)obj)->shiny = ft_parse_shininess(&(*raw_split)[2]);
		else if (**raw_split == 'T')
			((t_sphere *)obj)->transparence = ft_parse_transparence(&(*raw_split)[2], &((t_obj *)obj)->gamma);
		else if (**raw_split == 'M')
			((t_sphere *)obj)->mirror = ft_parse_shininess(&(*raw_split)[2]);
		else if (**raw_split == 'X')
			((t_sphere *)obj)->texture = helper_texture(data, &(*raw_split)[2]);
		else if (**raw_split == 'N')
			((t_sphere *)obj)->normal_map = helper_texture(data, &(*raw_split)[2]);
		else
			return (put(ERR7"(%s) UNKNNOWN PARAMETER\n", *raw_split), 1);
		raw_split++;
	}
	return (0);
}

static double	ft_parse_shininess(char *raw)
{
	double	shiny;
	int		err;

	err = 0;
	shiny = ft_atof(raw, &err);
	if (err)
		shiny = 0.0;
	return (shiny);
}

static double	ft_parse_transparence(char *raw, double *gamma)
{
	double	transparence;
	int		err;
	char	**split_tg;

	err = 0;
	split_tg = split(raw, ",");
	if (split_tg)
	{
		transparence = ft_atof(split_tg[0], &err);
		*gamma = ft_atof(split_tg[1], &err);
	}
	if (err)
		transparence = 0.0;

	transparence = fmax(0.0, fmin(1.0, transparence));
	*gamma = fmax(-PI / 2, fmin(PI / 2, *gamma));
	free_tab(split_tg);
	return (transparence);
}


static t_img	*helper_texture(t_data *data, char *path)
{
	
	t_img	*texture;
	int	err;

	texture = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, texture);
	if (!texture || !data->textures)
		return (put(ERRM"helper_texture\n"), NULL);

	if (path)
		texture->img = mlx_xpm_file_to_image(data->mlx, path, &texture->sz_x, &texture->sz_y);
	if (!texture->img)
		return(put(ERR8"Cant open sprite: %s\n", path), perror(RED"mlx_xpm_file_to_image"), NULL);

	texture->addr = mlx_get_data_addr(texture->img, &texture->bpp, &texture->ll, &texture->end);
	return (texture);
}
