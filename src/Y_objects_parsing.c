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

int	parse_A(t_data *data, char **raw_split);
int	parse_C(t_data *data, char **raw_split);
int	parse_L(t_data *data, char **raw_split);
int	parse_pl(t_data *data, char **raw_split);
int	parse_sp(t_data *data, char **raw_split);
int	parse_cy(t_data *data, char **raw_split);

///////////////////////////////////////////////////////////////////////////////]
int	parse_A(t_data *data, char **raw_split)
{

	return (0);
}

int	parse_C(t_data *data, char **raw_split)
{
	t_camera	*camera;

	if (tab_size(raw_split) != 3)
		return (put(ERR0"wrong num args camera\n"), 2);
	camera = mem(0, sizeof(t_camera));
	if (!camera)
		return (put(ERRM"new camera\n"), 2);
	if (ato_coor(raw_split[0], &camera->xyz))
		return (1);
	return (0);
}

int	parse_L(t_data *data, char **raw_split)
{

	return (0);
}

int	parse_pl(t_data *data, char **raw_split)
{

	return (0);
}

int	parse_sp(t_data *data, char **raw_split)
{

	return (0);
}

int	parse_cy(t_data *data, char **raw_split)
{

	return (0);
}
