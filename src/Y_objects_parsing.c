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
	int	err;
	t_ambient_light	*light;

	light = mem(0, sizeof(t_ambient_light));
	if (!light)
		return (put(ERRM), 2);
	data->bg_light = (t_ambient_light **)expand_tab((void **)data->bg_light, light);
	
	if (tab_size(raw_split) != 2)
		return (put(ERR1"bad number of args (AMBIENT LIGHT)\n"), 1);
	err = 0;
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
	h_camera_calc_up_right_vect(camera);
	camera->fov_cst_x = tan(camera->fov / 2) / SIZE_SCREEN_X;
	camera->fov_cst_y = tan(camera->fov / 2) / SIZE_SCREEN_Y;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			LIGHT
// 		XYZ = float
// 		BRIGHTNESS [0.0,1.0] float
// 		RGB [0, 255] int
int	parse_L(t_data *data, char **raw_split)
{
	t_light	*light;
	int	err;
	
	light = mem(0, sizeof(t_camera));
	if (!light)
		return (put(ERRM), 2);
	data->light_source = (t_light **)expand_tab((void **)data->light_source, light);
	
	if (tab_size(raw_split) != 3)
		return (put(ERR1"bad number of args (LIGHT SOURCE)\n"), 1);

	err = 0;
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
	t_plane	*plane;
	
	plane = mem(0, sizeof(t_plane));
	if (!plane)
		return (put(ERRM), 2);
	data->planes = (t_plane **)expand_tab((void **)data->planes, plane);

	if (tab_size(raw_split) != 3)
		return (put(ERR1"bad number of args (PLANES OBJECT)\n"), 1);

	if (ato_coor(raw_split[0], &(plane->c0)) || ato_coor(raw_split[1], (t_coor *)&plane->abc) || ato_rgb(raw_split[2], &(plane->color)))
		return (1);

	if (plane->abc.dx < -1.0 || plane->abc.dx > 1.0 || 
			plane->abc.dy < -1.0 || plane->abc.dy > 1.0 || 
			plane->abc.dz < -1.0 || plane->abc.dz > 1.0)
		return (put(ERR1"(%s) vector should be [-1.0,1.0]\n", raw_split[1]), 1);
	plane->d = -(plane->abc.dx * plane->c0.x + plane->abc.dy * plane->c0.y + plane->abc.dz * plane->c0.z);
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
	t_sphere	*sphere;
	int	err;
	
	sphere = mem(0, sizeof(t_sphere));
	if (!sphere)
		return (put(ERRM), 2);
	data->spheres = (t_sphere **)expand_tab((void **)data->spheres, sphere);

	// if (tab_size(raw_split) != 3)
	// 	return (put(ERR1"bad number of args (SPHERE OBJECT)\n"), 1);
	if (tab_size(raw_split) < 3)
		return (put(ERR1"bad number of args (SPHERE OBJECT)\n"), 1);
	else if (raw_split[3])
		parse_reste(data, &raw_split[3], (void*)sphere);

	err = 0;
	sphere->diameter = ft_atof(raw_split[1], &err);
	sphere->radius = sphere->diameter / 2;
	if (err || ato_coor(raw_split[0], &(sphere->c0)) || ato_rgb(raw_split[2], &(sphere->color)))
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
	t_cylinder	*cylinder;
	int	err;
	
	cylinder = mem(0, sizeof(t_cylinder));
	if (!cylinder)
		return (put(ERRM), 2);
	data->cylinders = (t_cylinder **)expand_tab((void **)data->cylinders, cylinder);

	// if (tab_size(raw_split) != 5)
	// 	return (put(ERR1"bad number of args (CYLINDER OBJECT)\n"), 1);
	if (tab_size(raw_split) < 5)
		return (put(ERR1"bad number of args (CYLINDER OBJECT)\n"), 1);
	else if (raw_split[5])
		parse_reste(data, &raw_split[5], (void*)cylinder);
// 
	err = 0;
	cylinder->diameter = ft_atof(raw_split[2], &err);
	cylinder->radius = cylinder->diameter / 2;
	cylinder->height = ft_atof(raw_split[3], &err);

	if (err || ato_coor(raw_split[0], &(cylinder->c0)) || ato_coor(raw_split[1], (t_coor *)&cylinder->v) || ato_rgb(raw_split[4], &(cylinder->color)))
		return (1);

	if (cylinder->v.dx < -1.0 || cylinder->v.dx > 1.0 || 
			cylinder->v.dy < -1.0 || cylinder->v.dy > 1.0 || 
			cylinder->v.dz < -1.0 || cylinder->v.dz > 1.0)
		return (put(ERR1"(%s) vector should be [-1.0,1.0]\n", raw_split[0]), 1);
	cylinder->xyz_other = (t_coor){
		cylinder->c0.x + cylinder->height * cylinder->v.dx,
		cylinder->c0.y + cylinder->height * cylinder->v.dy,
		cylinder->c0.z + cylinder->height * cylinder->v.dz};
	ft_normalize_vect(&cylinder->v);
	return (0);
}
