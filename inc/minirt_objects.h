/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_objects.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/04/01 14:47:51 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_OBJECTS_H
# define MINIRT_OBJECTS_H

# include "minirt_general_struct.h"

///////////////////////////////////////////////////////////////////////////////]
// 
// 				░█▀▀▀█ ░█▀▀█ ───░█ ░█▀▀▀ ░█▀▀█ ▀▀█▀▀ ░█▀▀▀█ 
// 				░█──░█ ░█▀▀▄ ─▄─░█ ░█▀▀▀ ░█─── ─░█── ─▀▀▀▄▄ 
// 				░█▄▄▄█ ░█▄▄█ ░█▄▄█ ░█▄▄▄ ░█▄▄█ ─░█── ░█▄▄▄█
//
///////////////////////////////////////////////////////////////////////////////]

// AMBIENT LIGHT = A
typedef struct s_ambient_light
{
	double	ratio;
	t_argb	rgb;
	t_img	*texture;
}	t_ambient_light;

// CAMERA = C
// fov_cst = tan(fov/2) / screenX
typedef struct s_camera
{
	t_obj	O;
	int		fov;
	double	fov_cst;

}	t_camera;

// LIGHT SOURCE = L
typedef struct s_light
{
	t_coor	xyz;
	double	ratio;
	t_argb	rgb;
}	t_light;

////////////////////////////////////////////]
// CIRCLE = ci
typedef struct s_cicle
{
	int		type;
	t_obj	O;
	t_param	param;

	double	radius;
}	t_circle;

// SPHERE = sp
typedef struct s_sphere
{
	int		type;
	t_obj	O;
	t_param	param;

	double	r;
}	t_sphere;

// PLANE = pl
typedef struct s_plane
{
	int		type;
	t_obj	O;
	t_param	param;

	double	d;
}	t_plane;

// CYLINDER = cy
typedef struct s_cylinder
{
	int		type;
	t_obj	O;
	t_param	param;

	t_coor	xyz_other;
	double	height;
	double	radius;
}	t_cylinder;

// CONE = co
typedef struct s_cone
{
	int		type;
	t_obj	O;
	t_param	param;

	double	h;
	double	radius;

	t_coor	apex;
	double	slope;
	double	angle;
}	t_cone;

// HYPERBOLOID = hy
// PARABOLOID = pa
typedef struct s_hyperboloid
{
	int		type;
	t_obj	O;
	t_param	param;

	t_coor	abc;
	double	radius;
}	t_hyper;

// ARROW = ar
typedef struct s_arrow
{
	int		type;
	t_obj	O;
	t_param	param;

	double	h;
	double	radius;

	t_coor	xyz_other;
	t_coor	apex;
	double	slope;
}	t_arrow;

// SPRITE = xi
typedef struct s_sprite
{
	int		type;
	t_obj	O;
	t_param	param;

	double	d;
	double	size;
}	t_sprite;

// MODEL = ob
typedef struct s_object
{
	int		type;
	t_obj	O;
	t_param	param;
	
	t_model	*model;
	double	size;
}	t_object;

#endif