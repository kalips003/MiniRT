/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_struct.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/03/16 14:59:48 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_STRUCT_H
# define MINIRT_STRUCT_H

# include <time.h>

# include "minirt_objects.h"
# include "minirt_general_struct.h"
# include "minirt_const.h"

///////////////////////////////////////////////////////////////////////////////]
//
// 		▒█▀▀▀█ ▀▀█▀▀ ▒█▀▀█ ▒█░▒█ ▒█▀▀█ ▀▀█▀▀ ▒█░▒█ ▒█▀▀█ ▒█▀▀▀ ▒█▀▀▀█ 
// 		░▀▀▀▄▄ ░▒█░░ ▒█▄▄▀ ▒█░▒█ ▒█░░░ ░▒█░░ ▒█░▒█ ▒█▄▄▀ ▒█▀▀▀ ░▀▀▀▄▄ 
// 		▒█▄▄▄█ ░▒█░░ ▒█░▒█ ░▀▄▄▀ ▒█▄▄█ ░▒█░░ ░▀▄▄▀ ▒█░▒█ ▒█▄▄▄ ▒█▄▄▄█
//
///////////////////////////////////////////////////////////////////////////////]
typedef struct s_data t_data;

// 	GLOBAL PX CALCUL STRUCT
typedef struct s_calcul_px
{
	// camera origin + vector
	void	*object;
	t_coor	c0;
	t_vect	v;

	// point intersection + vector normal to the surface
	t_coor	inter;
	t_vect	vn;
	
	// t_rgb	px_color;
	// t_argb	argb;
	double	dist;

// lighting values
	double dist_light;
	t_vect	v_light;
	// double	cos_angle;

// 
	int	reflected_depth;
	int	transparence_depth;
// transparence

	void	*inside[MAX_MIRROR_DEPTH];
	int		stack_top;

	t_mat	mat;
	t_light	eff_light;
	// double	eff_trans;
	// double	ratio;
	double	r_diff;
// 
	t_coor	diffuse;
	t_coor	specular;
	t_coor	reflected;
	t_coor	behind;

	// bit for printing info
	int	print;

}	t_calcul_px;

////////////////////////////////////////////]
////////////////////////////////////////////]
typedef struct s_eye
{
	t_camera	*c;
	
	int			current_camera;
	int			clic_px_x;
	int			clic_px_y;
	
}	t_eye;


////////////////////////////////////////////]
typedef void t_ft_change(t_data *data, t_obj2 *obj, int key_press_or_loop_call);


typedef struct s_data
{
	void	*mlx;
	void	*win;
	
	// t_img	buffer; <---- buffer not used, all buffer lines commented
	
	t_ambient_light	**bg;
	t_camera		**camera;
	t_light			**light_source;
	
	void		**objects;
	t_img		**textures;
	t_model		**models;
	
	// pthread_mutex_t buffer_mutex;
	t_img		buffer;
	t_img		buff_aa;
	// shortcut to the vector camera
	t_eye	eye;
	int		current_camera;
	
	char	**av;
	time_t	last_modif_time;
	
	
	t_obj2	*change_obj;
	t_ft_change	*change_function;
	double	ram[10];
	
	int		change;
	double	zoom;
	
	
	
	int		in_movement;
	// 
	t_calcul_px	calc;
}	t_data;
////////////////////////////////////////////]
////////////////////////////////////////////]
////////////////////////////////////////////]

typedef int (*t_ft_parse_object)(t_data *data, char **split);

typedef struct s_parsing_dico_pair
{
	char				*name;
	t_ft_parse_object	exe;
}	t_dico_pair;

typedef int (*t_ft_param_parsing)(t_data*, char*, t_param*);

#endif