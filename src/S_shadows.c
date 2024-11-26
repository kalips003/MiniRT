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

void	ft_handle_shadows(t_data *data, t_calcul *c);
void	ft_handle_sky(t_data *data, t_calcul *c);


///////////////////////////////////////////////////////////////////////////////]
void	ft_handle_shadows(t_data *data, t_calcul *c)
{
	double angle;

	angle = calculate_angle(&c->inter_point, &data->light_source[0]->xyz, &c->vect_norm);

	// light is behind the surface
	if (angle < 0)
	{
		ft_handle_sky(data, c);
		return ;
	}

// <<< calculate if in shadow of something HERE

	double adjusted_intensity = data->light_source[0]->ratio * fmax(0.0, cos(angle));

	c->px_color.r = fmin(255, round(c->px_color.r * data->light_source[0]->color.r / 255.0 * adjusted_intensity));
	c->px_color.g = fmin(255, round(c->px_color.g * data->light_source[0]->color.g / 255.0 * adjusted_intensity));
	c->px_color.b = fmin(255, round(c->px_color.b * data->light_source[0]->color.b / 255.0 * adjusted_intensity));

}

///////////////////////////////////////////////////////////////////////////////]
void	ft_handle_sky(t_data *data, t_calcul *c)
{
	double	light_intensity;
	t_rgb	light_color;

	light_intensity = data->light[0]->ratio;
	light_color = data->light[0]->color;

// <<< calculate if in shadow of something HERE


	c->px_color = (t_rgb){255, 255, 255};

	c->px_color.r = fmin(255, round(c->px_color.r * light_color.r / 255.0 * light_intensity));
	c->px_color.g = fmin(255, round(c->px_color.g * light_color.g / 255.0 * light_intensity));
	c->px_color.b = fmin(255, round(c->px_color.b * light_color.b / 255.0 * light_intensity));

}
