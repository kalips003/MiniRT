/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_general_struct.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/04/06 12:04:30 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_GENERAL_STRUCT_H
# define MINIRT_GENERAL_STRUCT_H

///////////////////////////////////////////////////////////////////////////////]
// 
// 			█▀▀▄ █▀▀█ ▀▀█▀▀ █▀▀█ 　 █▀▀ ▀▀█▀▀ █▀▀█ █░░█ █▀▀ ▀▀█▀▀ 
// 			█░░█ █▄▄█ ░░█░░ █▄▄█ 　 ▀▀█ ░░█░░ █▄▄▀ █░░█ █░░ ░░█░░ 
// 			▀▀▀░ ▀░░▀ ░░▀░░ ▀░░▀ 　 ▀▀▀ ░░▀░░ ▀░▀▀ ░▀▀▀ ▀▀▀ ░░▀░░
//
///////////////////////////////////////////////////////////////////////////////]

typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
}	t_rgb;

typedef struct s_argb
{
	int	a;
	int	r;
	int	g;
	int	b;
}	t_argb;

typedef struct s_coor
{
	double	x;
	double	y;
	double	z;
}	t_coor;

typedef struct s_vect
{
	double	dx;
	double	dy;
	double	dz;
}	t_vect;

typedef struct s_img
{
	void	*img;
	char	*addr;
	char	*path;

	int		bpp;
	int		ll;
	int		end;
	int		sz_x;
	int		sz_y;
}	t_img;

typedef union u_vec3 {
    double arr[3];
    t_coor coor;
    t_vect vect;
} t_dbl3;

////////////////////////////////////////////]
// param for each object
typedef struct s_param
{
	t_argb	argb;
	
	double	specular;
	double	shiny;
	double	mirror;
	double	transparence;
	double	gamma;
	double	light;
	double	ao;
	
	t_rgb	c2;
	t_img	*txt;
	t_img	*n_map;
	t_img	*a_map;
	t_img	*ao_map;
	t_img	*s_map;
	t_img	*rough_map;

}	t_param;

// material of .obj & in calcul_px
// sp = specular
// sh = shiny
// mi = mirror
// tr = transparence
// gamma = ior
// ao = ambient oclusion
typedef struct s_mat
{
	t_argb	argb;
	
	double	sp;
	double	sh;
	double	mi;
	double	tr;
	double	gamma;
	double	light;
	double	ao;

	char	*name;
	t_img	*txt;
	t_img	*n_map;
	t_img	*a_map;
	t_img	*ao_map;
	t_img	*s_map;
	t_img	*rough_map;

}	t_mat;

typedef struct s_mat2
{
	t_argb	argb;
	
	double	sp;
	double	sh;
	double	mi;
	double	tr;
	double	gamma;
	double	light;
	double	ao;
}	t_mat2;

typedef struct s_obj
{
	t_coor		c0;
	t_vect		view;
	t_vect		up;
	t_vect		right;

}	t_obj;

//	default object
typedef struct s_obj2
{
	int		type;
	t_obj	O;
	t_param	param;

}	t_obj2;


////////////////////////////////////////////]
// MODELS
////////////////////////////////////////////]
// .obj structs
typedef struct s_vt
{
	double	u;
	double	v;
}	t_vt;

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

	t_tri	*f;
	int		how_many_f;

	struct s_tree1	*r;
	struct s_tree1	*l;
}	t_bbox;

typedef struct s_model
{
	t_coor	**v;
	t_vect	**vn;
	t_vt	**vt;
	t_mat	**mat;
	t_bbox	tree;
	char	*path;
}	t_model;

#endif

