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

void	calculate_pixel_color(t_data *data, t_calcul *c);
void	calculate_pixel_color_simple(t_data *data, t_calcul *c);
int	ft_find_pixel_colision(t_data *data, t_calcul *c);

///////////////////////////////////////////////////////////////////////////////]
void	calculate_pixel_color(t_data *data, t_calcul *c)
{
	if (!ft_find_pixel_colision(data, c))
	{
		c->px_color.r = (int)(round(data->light[0]->color.r * data->light[0]->ratio));
		c->px_color.g = (int)(round(data->light[0]->color.g * data->light[0]->ratio));
		c->px_color.b = (int)(round(data->light[0]->color.b * data->light[0]->ratio));
	}
	else
		ft_handle_shadows(data, c);
}

///////////////////////////////////////////////////////////////////////////////]
void	calculate_pixel_color_simple(t_data *data, t_calcul *c)
{
	if (!ft_find_pixel_colision(data, c))
	{
		c->px_color.r = (int)(round(data->light[0]->color.r * data->light[0]->ratio));
		c->px_color.g = (int)(round(data->light[0]->color.g * data->light[0]->ratio));
		c->px_color.b = (int)(round(data->light[0]->color.b * data->light[0]->ratio));
	}
	else
	{
		// calculate rebond to light source
		double cos_angle;
		cos_angle = calculate_light_angle(&c->inter_point, &data->light_source[0]->xyz, &c->vect_norm);

		// light is behind the surface
		if (cos_angle < 0.0)
		{
			c->px_color.r = (int)fmin(255, round(c->px_color.r * data->light[0]->color.r / 255.0 * data->light[0]->ratio));
			c->px_color.g = (int)fmin(255, round(c->px_color.g * data->light[0]->color.g / 255.0 * data->light[0]->ratio));
			c->px_color.b = (int)fmin(255, round(c->px_color.b * data->light[0]->color.b / 255.0 * data->light[0]->ratio));
			return ;
		}

		double adjusted_intensity = data->light_source[0]->ratio * cos_angle;

		// light ambient + light source (color*l_color/255*scalar)
		c->px_color.r = fmin(255, round(c->px_color.r * data->light_source[0]->color.r / 255.0 * adjusted_intensity) + c->px_color.r * data->light[0]->color.r / 255.0 * data->light[0]->ratio);
		c->px_color.g = fmin(255, round(c->px_color.g * data->light_source[0]->color.g / 255.0 * adjusted_intensity) + c->px_color.g * data->light[0]->color.g / 255.0 * data->light[0]->ratio);
		c->px_color.b = fmin(255, round(c->px_color.b * data->light_source[0]->color.b / 255.0 * adjusted_intensity) + c->px_color.b * data->light[0]->color.b / 255.0 * data->light[0]->ratio);

	}
}

///////////////////////////////////////////////////////////////////////////////]
// 	find the pixel color of the closest object
// fills:
// position xyz of the colision (default 0,0,0)
// abc vector normal to the surface
// color of object in view
int	ft_find_pixel_colision(t_data *data, t_calcul *c)
{
	t_sphere	**sphere_ptr;
	t_plane		**plane_ptr;
	t_cylinder	**cyl_ptr;

	c->dist = -1.0;
	sphere_ptr = data->spheres - 1;
	while (++sphere_ptr && *sphere_ptr)
		distance_from_sphere(data, c, *sphere_ptr);

	plane_ptr = data->planes - 1;
	while (++plane_ptr && *plane_ptr)
		distance_from_plane(data, c, *plane_ptr);

	cyl_ptr = data->cylinders - 1;
	while (++cyl_ptr && *cyl_ptr)
		distance_from_cylinder(data, c, *cyl_ptr);
	
	return (c->dist != -1.0);
}



