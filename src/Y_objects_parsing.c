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
// 			AMBIENT LIGHT
// 		ambient lighting ratio [0.0,1.0] float
// 		RGB [0, 255] int
int	parse_A(t_data *data, char **raw_split)
{
	t_ambient_light	*light = mem(0, sizeof(t_ambient_light));
	if (!light)
		return (put(ERRM), 2);
	data->light = (t_ambient_light **)expand_tab((void **)data->light, light);
	
	if (tab_size(raw_split) != 2)
		return (put(ERR1"bad number of args (AMBIENT LIGHT)\n"), 1);
	int	err = 0;
	light->ratio = ft_atof(raw_split[0], &err);
	if (err || ato_rgb(raw_split[1], &(light->color)))
		return (1);
	if (light->ratio < 0.0 || light->ratio > 1.0)
		return (put(ERR1"(%s) ambient lighting ratio should be [0.0,1.0]\n", raw_split[0]), 1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			CAMERA
// 		position XYZ = float
// 		vector camera [-1,1] float							//			bug cast ato_coor?
// 		FOV [0, 180] int
int	parse_C(t_data *data, char **raw_split)
{
	t_camera	*camera;
	int	err;
	
	camera = mem(0, sizeof(t_camera));
	if (!camera)
		return (put(ERRM), 2);
	data->camera = (t_camera **)expand_tab((void **)data->camera, camera);
	
	if (tab_size(raw_split) != 3)
		return (put(ERR1"bad number of args (CAMERA)\n"), 1);
	
	err = 0;
	camera->fov = ft_atoi(raw_split[2], &err);
	if (err || ato_coor(raw_split[0], &(camera->xyz)) || ato_coor(raw_split[1], (t_coor *)&camera->view))
		return (1);

	if (camera->fov < 0 || camera->fov > 180)
		return (put(ERR1"(%s) camera fov should be [0, 180]\n", raw_split[0]), 1);

	if (camera->view.dx < -1.0 || camera->view.dx > 1.0 || 
			camera->view.dy < -1.0 || camera->view.dy > 1.0 || 
			camera->view.dz < -1.0 || camera->view.dz > 1.0)
		return (put(ERR1"(%s) vector should be [-1.0,1.0]\n", raw_split[0]), 1);
	ft_normalize_vect(&camera->view);
	h_camera(camera);

	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			LIGHT
// 		XYZ = float
// 		BRIGHTNESS [0.0,1.0] float
// 		RGB [0, 255] int
int	parse_L(t_data *data, char **raw_split)
{
	t_light	*light = mem(0, sizeof(t_camera));
	if (!light)
		return (put(ERRM), 2);
	data->light_source = (t_light **)expand_tab((void **)data->light_source, light);
	
	if (tab_size(raw_split) != 3)
		return (put(ERR1"bad number of args (LIGHT SOURCE)\n"), 1);

	int	err = 0;
	light->ratio = ft_atof(raw_split[1], &err);

	if (err || ato_coor(raw_split[0], &(light->xyz)) || ato_rgb(raw_split[2], &(light->color)))
		return (1);
	if (light->ratio < 0.0 || light->ratio > 1.0)
		return (put(ERR1"(%s) light source ratio should be [0.0,1.0]\n", raw_split[0]), 1);

	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			PLANES
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		RGB [0, 255] int
// 
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
int	parse_pl(t_data *data, char **raw_split)
{
	t_plane	*plane = mem(0, sizeof(t_plane));
	if (!plane)
		return (put(ERRM), 2);
	data->planes = (t_plane **)expand_tab((void **)data->planes, plane);

	if (tab_size(raw_split) != 3)
		return (put(ERR1"bad number of args (PLANES OBJECT)\n"), 1);

	if (ato_coor(raw_split[0], &(plane->xyz)) || ato_coor(raw_split[1], (t_coor *)&plane->abc) || ato_rgb(raw_split[2], &(plane->color)))
		return (1);

	if (plane->abc.dx < -1.0 || plane->abc.dx > 1.0 || 
			plane->abc.dy < -1.0 || plane->abc.dy > 1.0 || 
			plane->abc.dz < -1.0 || plane->abc.dz > 1.0)
		return (put(ERR1"(%s) vector should be [-1.0,1.0]\n", raw_split[0]), 1);
	plane->d = -(plane->abc.dx * plane->xyz.x + plane->abc.dy * plane->xyz.y + plane->abc.dz * plane->xyz.z);
	ft_normalize_vect(&plane->abc);
	return (0);
}


///////////////////////////////////////////////////////////////////////////////]
// 			SPHERE
// 		XYZ = float
// 		DIAMETER = float
// 		RGB [0, 255] int
int	parse_sp(t_data *data, char **raw_split)
{
	t_sphere	*sphere = mem(0, sizeof(t_sphere));
	if (!sphere)
		return (put(ERRM), 2);
	data->spheres = (t_sphere **)expand_tab((void **)data->spheres, sphere);

	if (tab_size(raw_split) != 3)
		return (put(ERR1"bad number of args (SPHERE OBJECT)\n"), 1);

	int	err = 0;
	sphere->diameter = ft_atof(raw_split[1], &err);
	sphere->radius = sphere->diameter / 2;
	if (err || ato_coor(raw_split[0], &(sphere->xyz)) || ato_rgb(raw_split[2], &(sphere->color)))
		return (1);

	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			CYLINDER
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		DIAMETER = float
// 		HEIGHT = float
// 		RGB [0, 255] int
int	parse_cy(t_data *data, char **raw_split)
{
	t_cylinder	*cylinder = mem(0, sizeof(t_cylinder));
	if (!cylinder)
		return (put(ERRM), 2);
	data->cylinders = (t_cylinder **)expand_tab((void **)data->cylinders, cylinder);

	if (tab_size(raw_split) != 5)
		return (put(ERR1"bad number of args (CYLINDER OBJECT)\n"), 1);

	int	err = 0;
	cylinder->diameter = ft_atof(raw_split[2], &err);
	cylinder->height = ft_atof(raw_split[3], &err);

	if (err || ato_coor(raw_split[0], &(cylinder->xyz)) || ato_coor(raw_split[1], (t_coor *)&cylinder->abc) || ato_rgb(raw_split[4], &(cylinder->color)))
		return (1);

	if (cylinder->abc.dx < -1.0 || cylinder->abc.dx > 1.0 || 
			cylinder->abc.dy < -1.0 || cylinder->abc.dy > 1.0 || 
			cylinder->abc.dz < -1.0 || cylinder->abc.dz > 1.0)
		return (put(ERR1"(%s) vector should be [-1.0,1.0]\n", raw_split[0]), 1);
	ft_normalize_vect(&cylinder->abc);
	return (0);
}
