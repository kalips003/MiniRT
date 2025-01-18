/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_shadows_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/18 00:11:53 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
int something_block_the_light(t_data *data, t_calcul_px *c, t_light *light);

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

	// ft_normalize_vect(&l);
	// ft_normalize_vect(normal);
	if(ft_normalize_vect(&l) || ft_normalize_vect(normal))
		return (put(ERR9 "vector NULL?\n"), -1.0);
	// if(ft_normalize_vect(&l))
	// 	return (put(ERR9 "vector NULL?\n"), -1.0);
	// if(ft_normalize_vect(normal))
	// 	return (put(ERR9 "vector NULL2?\n"), -1.0);

// Dot product of Light Direction and Normal
	cos_theta = l.dx * normal->dx + l.dy * normal->dy + l.dz * normal->dz;
	cos_theta = fmax(-1.0, fmin(1.0, cos_theta));

// Return the angle in radians
	return (cos_theta);
}

///////////////////////////////////////////////////////////////////////////////]
int something_block_the_light(t_data *data, t_calcul_px *c, t_light *light)
{
	t_sphere	**sphere_ptr;
	t_plane		**plane_ptr;
	t_cylinder	**cyl_ptr;
	t_calcul_px	calcul;

	calcul.c0 = c->inter;
	calcul.inter = light->xyz;
	calcul.v_view = (t_vect){calcul.inter.x - calcul.c0.x, calcul.inter.y - calcul.c0.y, calcul.inter.z - calcul.c0.z};
	calcul.dist = sqrt(calcul.v_view.dx * calcul.v_view.dx + calcul.v_view.dy * calcul.v_view.dy + calcul.v_view.dz * calcul.v_view.dz);
	
	c->dist_light = calcul.dist;
	
	ft_normalize_vect(&calcul.v_view);
	c->v_light = calcul.v_view;

	sphere_ptr = data->spheres - 1;
	while (++sphere_ptr && *sphere_ptr)
	{
		if (*sphere_ptr == c->object)
			continue;
		if (in_shadow_of_sphere(&calcul, *sphere_ptr))
			return (1);
	}

	plane_ptr = data->planes - 1;
	while (++plane_ptr && *plane_ptr)
	{
		if (*plane_ptr == c->object)
			continue;
		if (in_shadow_of_plane(&calcul, *plane_ptr))
			return (1);
	}

	cyl_ptr = data->cylinders - 1;
	while (++cyl_ptr && *cyl_ptr)
	{
		if (*cyl_ptr == c->object)
			continue;
		if (in_shadow_of_cylinder(&calcul, *cyl_ptr) ||
				in_shadow_of_cicle(c, (t_circle){(*cyl_ptr)->c0, (*cyl_ptr)->color,
					(*cyl_ptr)->shiny, (*cyl_ptr)->mirror, (*cyl_ptr)->transparence, (*cyl_ptr)->gamma,
					(*cyl_ptr)->texture, (*cyl_ptr)->normal_map, (*cyl_ptr)->radius, (*cyl_ptr)->v}) ||
				in_shadow_of_cicle(c, (t_circle){(*cyl_ptr)->xyz_other, (*cyl_ptr)->color,
					(*cyl_ptr)->shiny, (*cyl_ptr)->mirror, (*cyl_ptr)->transparence, (*cyl_ptr)->gamma,
					(*cyl_ptr)->texture, (*cyl_ptr)->normal_map, (*cyl_ptr)->radius, (*cyl_ptr)->v}))
			return (1);
	}
	return (0);
}
