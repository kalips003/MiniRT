/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_find_colision.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/12/15 09:27:57 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	ft_find_pixel_colision(t_data *data, t_calcul_px *c);

///////////////////////////////////////////////////////////////////////////////]
// 	find the pixel color of the closest object
// fills:
// position xyz of the colision (default 0,0,0)
// abc vector normal to the surface
// color of object in view
int	ft_find_pixel_colision(t_data *data, t_calcul_px *c)
{
	t_sphere	**sphere_ptr;
	t_plane		**plane_ptr;
	t_cylinder	**cyl_ptr;

	c->dist = -1.0;
	sphere_ptr = data->spheres - 1;
	while (++sphere_ptr && *sphere_ptr)
		distance_from_sphere(c, *sphere_ptr);

	plane_ptr = data->planes - 1;
	while (++plane_ptr && *plane_ptr)
		distance_from_plane(c, *plane_ptr);


	cyl_ptr = data->cylinders - 1;
	while (++cyl_ptr && *cyl_ptr)
	{
		distance_from_cylinder(c, *cyl_ptr);
		distance_from_cicle(c, (t_circle){
			(*cyl_ptr)->c0, (*cyl_ptr)->color,
			(*cyl_ptr)->shiny, (*cyl_ptr)->mirror, (*cyl_ptr)->transparence, (*cyl_ptr)->gamma,
			(*cyl_ptr)->texture, (*cyl_ptr)->normal_map, (*cyl_ptr)->radius, (*cyl_ptr)->v});
		distance_from_cicle(c, (t_circle){
			(*cyl_ptr)->xyz_other, (*cyl_ptr)->color,
			(*cyl_ptr)->shiny, (*cyl_ptr)->mirror, (*cyl_ptr)->transparence, (*cyl_ptr)->gamma,
			(*cyl_ptr)->texture, (*cyl_ptr)->normal_map, (*cyl_ptr)->radius, (*cyl_ptr)->v});
	}
	return (c->dist != -1.0);
}
