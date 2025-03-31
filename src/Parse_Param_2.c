/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P3_param_parsing2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/03 14:00:51 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	parse_shininess(t_data *data, char *raw, t_param *obj);
int	parse_transparence(t_data *data, char *raw, t_param *obj);
int	parse_mirror(t_data *data, char *raw, t_param *obj);
int	parse_color2(t_data *data, char *raw, t_param *obj);
int	parse_light(t_data *data, char *raw, t_param *obj);

///////////////////////////////////////////////////////////////////////////////]
// (Shininess) S=20.0
int	parse_shininess(t_data *data, char *raw, t_param *obj)
{
	char	**split_tg;

	(void)data;
	split_tg = split(raw, ",");
	if (tab_size(split_tg) != 2)
		return (put(ERR1"bad number of args (SPECULAR - SHINY)\n"), \
			free_tab(split_tg), 1);
	if (ft_atof(split_tg[0], &obj->specular)
		|| ft_atof(split_tg[1], &obj->shiny))
		return (1);
	if (obj->specular < 0.0 || obj->specular > 1.0)
		return (put(ERR1"(%s) specular should be [0.0,1.0]\n", split_tg[0]), \
			free_tab(split_tg), 1);
	if (obj->shiny < 10.0 || obj->shiny > 1000.0)
		return (put(ERR1"(%s) material shininess should be [10,500+]\n", \
			split_tg[0]), free_tab(split_tg), 1);
	free_tab(split_tg);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Transparence, gamma) T=[0,1],[1.0, 2.5+]
int	parse_transparence(t_data *data, char *raw, t_param *obj)
{
	char	**split_tg;

	(void)data;
	split_tg = split(raw, ",");
	if (tab_size(split_tg) != 2)
		return (put(ERR1"bad number of args (TRANSPARENCE - GAMMA) (%.3t)\n", \
			split_tg), free_tab(split_tg), 1);
	if (ft_atof(split_tg[0], &obj->transparence)
		|| ft_atof(split_tg[1], &obj->gamma))
		return (1);
	if (obj->transparence < 0.0 || obj->transparence > 1.0)
		return (put(ERR1"(%s) transparence should be [0.0,1.0]\n", \
			split_tg[0]), free_tab(split_tg), 1);
	if (obj->gamma < 0.0)
		return (put(ERR1"(%s) refraction gamma should be [1.0, 2.5+]\n", \
			split_tg[1]), free_tab(split_tg), 1);
	free_tab(split_tg);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Mirroir) M=[0,1]
int	parse_mirror(t_data *data, char *raw, t_param *obj)
{
	(void)data;
	if (ft_atof(raw, &obj->mirror))
		return (1);
	if (obj->mirror < 0.0 || obj->mirror > 1.0)
		return (put(ERR1"(%s) reflection should be [0.0,1.0]\n", raw), 1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Color2) C=255,0,112
int	parse_color2(t_data *data, char *raw, t_param *obj)
{
	(void)data;
	if (ato_rgb(raw, &obj->c2))
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// (Light) L=[0,1]
int	parse_light(t_data *data, char *raw, t_param *obj)
{
	(void)data;
	if (ft_atof(raw, &obj->light))
		return (1);
	if (obj->mirror < 0.0 || obj->mirror > 1.0)
		return (put(ERR1"(%s) Object light should be [0.0,1.0]\n", raw), 1);
	return (0);
}
