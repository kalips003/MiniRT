/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_objects.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/03/30 10:20:09 by kalipso          ###   ########.fr       */
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
	t_argb	color;
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

	double	radius;
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

	double	height;
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

////////////////////////////////////////////]
// MODELS
////////////////////////////////////////////]
typedef struct s_vt
{
	double	u;
	double	v;
}	t_vt;

// typedef struct s_tri
// {
// 	int				p1;
// 	int				vt1;
// 	int				vn1;
// 	int				p2;
// 	int				vt2;
// 	int				vn2;
// 	int				p3;
// 	int				vt3;
// 	int				vn3;
	
// 	t_mat			*mat;
// 	t_coor			centroid;
// 	struct s_tri	*next;
// }	t_tri;


typedef struct s_tri
{
	int				p[3];
	int				vt[3];
	int				vn[3];
	
	t_mat			*mat;
	t_coor			centroid;
	struct s_tri	*next;
}	t_tri;

typedef struct s_tree1
{
	t_coor	min;
	t_coor	max;

//	linked list first
	t_tri	*f;
	int		how_many_f;

	struct s_tree1	*r;
	struct s_tree1	*l;
}	t_bbox;

// typedef struct s_model
// {
// 	t_coor	**v;
// 	t_vect	**vn;
// 	t_vt	**vt;
// 	t_mat	**mat;
// 	t_tri	*f;
// 	t_tri	*first;
// 	t_coor	min;
// 	t_coor	max;
// 	char	*path;
// }	t_model;
typedef struct s_model
{
	t_coor	**v;
	t_vect	**vn;
	t_vt	**vt;
	t_mat	**mat;
	t_bbox	tree;
	char	*path;
}	t_model;

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