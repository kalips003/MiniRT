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

	angle = calculate_light_angle(&c->inter_point, &data->light_source[0]->xyz, &c->vect_norm);

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

// ///////////////////////////////////////////////////////////////////////////////]
// void	ft_handle_shadows(t_data *data, t_calcul *c)
// {
// 	double angle;

// 	angle = calculate_light_angle(&c->inter_point, &data->light_source[0]->xyz, &c->vect_norm);



// // double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal)

// 	t_vect	l;
// 	double cos_theta;

// // Calculate Light Direction vector
// 	l.dx = data->light_source[0]->xyz.x - c->inter_point.x;
// 	l.dy = data->light_source[0]->xyz.y - c->inter_point.y;
// 	l.dz = data->light_source[0]->xyz.z - c->inter_point.z;

// 	if(ft_normalize_vect(&l) || ft_normalize_vect(&c->vect_norm))
// 		return (put(ERR9 "i dont know? vector normal?\n"), (void)0);

// // Dot product of Light Direction and Normal
// 	cos_theta = l.dx * c->vect_norm.dx + l.dy * c->vect_norm.dy + l.dz * c->vect_norm.dz;
// 	cos_theta = fmax(-1.0, fmin(1.0, cos_theta));

// // Return the angle in radians
// 	// return acos(cos_theta);



// 	// light is behind the surface
// 	if (cos_theta < 0.0)
// 	{
// 		ft_handle_sky(data, c);
// 		return ;
// 	}

// // <<< calculate if in shadow of something HERE

// 	double adjusted_intensity = data->light_source[0]->ratio * cos_theta;

// 	c->px_color.r = fmin(255, round(c->px_color.r * data->light_source[0]->color.r / 255.0 * adjusted_intensity));
// 	c->px_color.g = fmin(255, round(c->px_color.g * data->light_source[0]->color.g / 255.0 * adjusted_intensity));
// 	c->px_color.b = fmin(255, round(c->px_color.b * data->light_source[0]->color.b / 255.0 * adjusted_intensity));

// }

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
