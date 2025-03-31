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
int	parse_texture(t_data *data, char *path, t_param *obj);

#define PARAM_DICO "STMXNACLOsR"

///////////////////////////////////////////////////////////////////////////////]
// S: (Shininess) S=[1.0, +]
// T: (Transparence, gamma) T=[0,1],[1.0, 2.5+]
// M: (Mirroir) M=[0,1]
// X: (Texture) X=t_sphere.xpm
// N: (Normal Map) N=n_sphere.xpm
// A: (Alpha Map) A=a_sphere.xpm
// C: (Color 2) C=255,0,0
// L: (Light) L=[0,1]
// O: (Ambient Occlusion) O=ao_sphere.xpm
// s: (Specular Map) s=sp_sphere.xpm
// R: (Roughness Map) s=r_sphere.xpm
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
	parse_smap,
	parse_rmap
};

///////////////////////////////////////////////////////////////////////////////]
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

///////////////////////////////////////////////////////////////////////////////]
// (Texture) X=sphere_texture.xpm
int	parse_texture(t_data *data, char *path, t_param *obj)
{
	obj->txt = parse_img(data, path);
	if (!obj->txt)
		return (1);
	return (0);
}
