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

int	parse_texture(t_data *data, char *path, t_param *obj);
int	parse_nmap(t_data *data, char *path, t_param *obj);
int	parse_amap(t_data *data, char *path, t_param *obj);
int	parse_ao(t_data *data, char *path, t_param *obj);
int	parse_smap(t_data *data, char *path, t_param *obj);
int	parse_rmap(t_data *data, char *path, t_param *obj);

///////////////////////////////////////////////////////////////////////////////]
// (Texture) X=sphere_texture.xpm
int	parse_texture(t_data *data, char *path, t_param *obj)
{
	obj->txt = parse_img(data, path);
	if (!obj->txt)
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Normal Map) N=sphere_normal.xpm
int	parse_nmap(t_data *data, char *path, t_param *obj)
{	
	obj->n_map = parse_img(data, path);
	if (!obj->n_map)
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Alpha Map) A=sphere_normal.xpm
int	parse_amap(t_data *data, char *path, t_param *obj)
{	
	obj->a_map = parse_img(data, path);
	if (!obj->a_map)
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Ambient Occlusion) O=ao_normal.xpm
int	parse_ao(t_data *data, char *path, t_param *obj)
{	
	obj->ao_map = parse_img(data, path);
	if (!obj->ao_map)
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Specular Map) s=sp_map.xpm
int	parse_smap(t_data *data, char *path, t_param *obj)
{	
	obj->s_map = parse_img(data, path);
	if (!obj->s_map)
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Roughness Map) R=r_map.xpm
int	parse_rmap(t_data *data, char *path, t_param *obj)
{	
	obj->rough_map = parse_img(data, path);
	if (!obj->rough_map)
		return (1);
	return (0);
}

