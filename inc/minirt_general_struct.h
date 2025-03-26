/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_general_struct.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/03/25 01:29:52 by kalipso          ###   ########.fr       */
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

////////////////////////////////////////////]
typedef struct s_param
{
	t_argb	argb;
	
	double	specular;
	double	shiny;
	double	mirror;
	double	transparence;
	double	gamma;
	double	light;
	
	t_rgb	color2;
	t_img	*texture;
	t_img	*normal_map;
	t_img	*alpha_map;

}	t_param;//		objects param

typedef struct s_param2
{
	t_argb	argb;
	
	double	sp;
	double	sh;
	double	mi;
	double	tr;
	double	gamma;
	double	light;

	char	*name;
	t_img	*txt;

}	t_mat;//		objects param

typedef struct s_obj
{
	t_coor		c0;
	t_vect		view;
	t_vect		up;
	t_vect		right;

}	t_obj;

typedef struct s_obj2
{
	int		type;
	t_obj	O;
	t_param	param;
	// t_model	*model;

}	t_obj2;//		default object

#endif
