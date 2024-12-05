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
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
int something_block_the_light(t_data *data, t_calcul *c);


///////////////////////////////////////////////////////////////////////////////]
void	ft_handle_shadows(t_data *data, t_calcul *c)
{
	// calculate rebond to light source
	double cos_angle;
	cos_angle = calculate_light_angle(&c->inter_point, &data->light_source[0]->xyz, &c->vect_norm);

	// light is behind the surface
	if (cos_angle < 0.0 || something_block_the_light(data, c))
	{
		// <<< calculate the half shere view of the sky HERE
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

///////////////////////////////////////////////////////////////////////////////]
int something_block_the_light(t_data *data, t_calcul *c)
{
	t_sphere	**sphere_ptr;
	t_plane		**plane_ptr;
	t_cylinder	**cyl_ptr;
	t_calcul	calcul;

	calcul.origin = c->inter_point;

	calcul.v_rotated = (t_vect){data->light_source[0]->xyz.x - calcul.origin.x
		, data->light_source[0]->xyz.y - calcul.origin.y
		, data->light_source[0]->xyz.z - calcul.origin.z};

	double dist_from_light = sqrt(calcul.v_rotated.dx * calcul.v_rotated.dx + calcul.v_rotated.dy * calcul.v_rotated.dy + calcul.v_rotated.dz * calcul.v_rotated.dz);
	ft_normalize_vect(&calcul.v_rotated);

	calcul.dist = -1.0;
	sphere_ptr = data->spheres - 1;
	while (++sphere_ptr && *sphere_ptr)
		if (distance_from_sphere(data, &calcul, *sphere_ptr) >= 0.0 && calcul.dist < dist_from_light)
			return (calcul.dist);

	plane_ptr = data->planes - 1;
	while (++plane_ptr && *plane_ptr)
		if (distance_from_plane(data, &calcul, *plane_ptr) >= 0.0 && calcul.dist < dist_from_light)
			return (calcul.dist);

	// cyl_ptr = data->cylinders - 1;
	// while (++cyl_ptr && *cyl_ptr)
	// 	distance_from_cyl(data, &c, v, cyl_ptr);
	return (0);
}
///////////////////////////////////////////////////////////////////////////////]
// calculate angle between camera ray and light source at intersection
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal)
{
	t_vect	l;
	double cos_theta;

// Calculate Light Direction vector
	l.dx = light->x - intersection->x;
	l.dy = light->y - intersection->y;
	l.dz = light->z - intersection->z;

	if(ft_normalize_vect(&l) || ft_normalize_vect(normal))
		return (put(ERR9 "i dont know? vector normal?\n"), -1.0);

// Dot product of Light Direction and Normal
	cos_theta = l.dx * normal->dx + l.dy * normal->dy + l.dz * normal->dz;
	cos_theta = fmax(-1.0, fmin(1.0, cos_theta));

// Return the angle in radians
	return (cos_theta);
}
