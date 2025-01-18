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

void	ft_handle_shadows_simple(t_data *data, t_calcul_px *c);



///////////////////////////////////////////////////////////////////////////////]
void	ft_handle_shadows_simple(t_data *data, t_calcul_px *c)
{
	t_coor ambient;
	t_coor	diffuse;
	t_light	**lights;
	double adjusted_intensity;
	
	ambient = ft_ambient(data, c);
	memset(&diffuse, 0 , sizeof(t_coor));

	lights = data->light_source - 1;
	while (lights && *(++lights))
	{
		c->cos_angle = calculate_light_angle(&c->inter, &(*lights)->xyz, &c->v_normal);
		if (c->cos_angle < 0.0 || something_block_the_light(data, c, *lights))
			continue ;
		adjusted_intensity = (*lights)->ratio * c->cos_angle;
		adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
		diffuse.x += c->px_color.r * (*lights)->color.r / 255.0 * adjusted_intensity;
		diffuse.y += c->px_color.g * (*lights)->color.g / 255.0 * adjusted_intensity;
		diffuse.z += c->px_color.b * (*lights)->color.b / 255.0 * adjusted_intensity;
	}

	c->px_color.r = fmax(0, fmin(255, round(ambient.x + diffuse.x)));
	c->px_color.g = fmax(0, fmin(255, round(ambient.y + diffuse.y)));
	c->px_color.b = fmax(0, fmin(255, round(ambient.z + diffuse.z)));

}
