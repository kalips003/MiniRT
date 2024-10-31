/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Z_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2024/10/31 12:19:23 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	end(t_data *data, int exit_code);

///////////////////////////////////////////////////////////////////////////////]
void	end(t_data *data, int exit_code)
{
	free_tab((char **)data->light);
	free_tab((char **)data->camera);
	free_tab((char **)data->light_source);
	free_tab((char **)data->spheres);
	free_tab((char **)data->planes);
	free_tab((char **)data->cylinders);
	exit(exit_code);
}
