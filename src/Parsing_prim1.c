/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing_prim1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 13:02:34 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	parse_ci(t_data *data, char **raw_split);
int	parse_pl(t_data *data, char **raw_split);
int	parse_sp(t_data *data, char **raw_split);
int	parse_cy(t_data *data, char **raw_split);
int	parse_co(t_data *data, char **raw_split);

///////////////////////////////////////////////////////////////////////////////]
// 			CIRCLE
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		RGB [0, 255] int
// 		radius	float
// 
int	parse_ci(t_data *data, char **raw_split)
{
	t_circle	*circle;

	circle = mem(0, sizeof(t_circle));
	if (!circle)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, circle);
	if (tab_size(raw_split) < 4)
		return (put(ERR1"bad number of args (CIRCLE OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[4], &circle->param))
		return (1);
	circle->type = CIRCLE;
	if (ato_coor(raw_split[0], &circle->O.c0)
		|| ato_coor(raw_split[1], (t_coor*)&circle->O.view)
		|| ato_argb(raw_split[2], &circle->param.argb)
		|| ft_atof(raw_split[3], &circle->radius))
		return (1);
	if (circle->radius < EPSILON)
		return (put(ERR1"(CIRCLE OBJECT) too small\n"), 1);
	printf("radius: %.3f\n", circle->radius);
	if (h_parse_vect_space(&circle->O, &circle->O.view))
		return (1);
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
	data->objects = expand_tab(data->objects, plane);
	if (tab_size(raw_split) < 3)
		return (put(ERR1"bad number of args (PLANES OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[3], &plane->param))
		return (1);
	plane->type = PLANE;
	if (ato_coor(raw_split[0], &plane->O.c0)
		|| ato_coor(raw_split[1], (t_coor*)&plane->O.view)
		|| ato_argb(raw_split[2], &plane->param.argb))
		return (1);
	if (h_parse_vect_space(&plane->O, &plane->O.view))
		return (1);
	plane->d = -ft_dot_p(&plane->O.view, &plane->O.c0);
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

	sphere = mem(0, sizeof(t_sphere));
	if (!sphere)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, sphere);
	if (tab_size(raw_split) < 3)
		return (put(ERR1"bad number of args (SPHERE OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[3], &sphere->param))
		return (1);
	sphere->type = SPHERE;
	if (ato_coor(raw_split[0], &sphere->O.c0)
		|| ft_atof(raw_split[1], &sphere->r)
		|| ato_argb(raw_split[2], &sphere->param.argb))
		return (1);
	if (sphere->r < EPSILON)
		return (put(ERR1"(SPHERE OBJECT) too small\n"), 1);
	sphere->r /= 2;
	if (h_parse_vect_space(&sphere->O, NULL))
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

	cylinder = mem(0, sizeof(t_cylinder));
	if (!cylinder)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, cylinder);
	if (tab_size(raw_split) < 5)
		return (put(ERR1"bad number of args (CYLINDER OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[5], &cylinder->param))
		return (1);
	cylinder->type = CYLINDER;
	if (ato_coor(raw_split[0], &cylinder->O.c0)
		|| ato_coor(raw_split[1], (t_coor *)&cylinder->O.view)
		|| ft_atof(raw_split[2], &cylinder->radius)
		|| ft_atof(raw_split[3], &cylinder->height)
		|| ato_argb(raw_split[4], &cylinder->param.argb))
		return (1);
	if (cylinder->radius < EPSILON || cylinder->height < EPSILON)
		return (put(ERR1"(CYLINDER OBJECT) too small\n"), 1);
	cylinder->radius /= 2;
	if (h_parse_vect_space(&cylinder->O, &cylinder->O.view))
		return (1);
	cylinder->xyz_other = new_moved_point(&cylinder->O.c0, \
		&cylinder->O.view, cylinder->height);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 			CONE
// 		XYZ = float
// 		xyz vector [-1,1] float
// 		RADIUS = float
// 		HEIGHT = float
// 		RGB [0, 255] int
int	parse_co(t_data *data, char **raw_split)
{
	t_cone	*cone;

	cone = mem(0, sizeof(t_cone));
	if (!cone)
		return (put(ERRM), 2);
	data->objects = expand_tab(data->objects, cone);
	if (tab_size(raw_split) < 5)
		return (put(ERR1"bad number of args (CONE OBJECT)\n"), 1);
	if (parse_reste(data, &raw_split[5], &cone->param))
		return (1);
	cone->type = CONE;
	if (ato_coor(raw_split[0], &cone->O.c0)
		|| ato_coor(raw_split[1], (t_coor *)&cone->O.view)
		|| ft_atof(raw_split[2], &cone->radius)
		|| ft_atof(raw_split[3], &cone->height)
		|| ato_argb(raw_split[4], &cone->param.argb))
		return (1);
	if (cone->radius < EPSILON || cone->height < EPSILON)
		return (put(ERR1"(CONE OBJECT) too small\n"), 1);
	if (h_parse_vect_space(&cone->O, &cone->O.view))
		return (1);
	cone->apex = new_moved_point(&cone->O.c0, &cone->O.view, cone->height);
	cone->slope = (cone->radius * cone->radius) / (cone->height * cone->height);
	cone->angle = atan(cone->radius / cone->height);
	return (0);
}
