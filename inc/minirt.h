/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2024/10/30 16:00:39 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

// # include <pthread.h>
// # include <sys/stat.h>
// # include <signal.h>
// # include <dirent.h>
// # include <termios.h>
// # include <termcap.h>
// # include <sys/ioctl.h>
// # include <readline/readline.h>
// # include <readline/history.h>
// # include <signal.h>
// # include <time.h>
// # include <sys/types.h>

# include "libft.h"

///////////////////////////////////////////////////////////////////////////////]
//
//			▒█▀▄▀█ ░█▀▀█ ▒█▄░▒█ ▒█▀▀▄ ░█▀▀█ ▀▀█▀▀ ▒█▀▀▀█ ▒█▀▀█ ▒█░░▒█
//			▒█▒█▒█ ▒█▄▄█ ▒█▒█▒█ ▒█░▒█ ▒█▄▄█ ░▒█░░ ▒█░░▒█ ▒█▄▄▀ ▒█▄▄▄█
//			▒█░░▒█ ▒█░▒█ ▒█░░▀█ ▒█▄▄▀ ▒█░▒█ ░▒█░░ ▒█▄▄▄█ ▒█░▒█ ░░▒█░░
//
///////////////////////////////////////////////////////////////////////////////]

# define STDIN 0
# define STDOUT 1
# define STDERR 2

// typedef struct s_data			t_data;
// typedef struct s_cmd			t_cmd;
// typedef int						(*t_builtin)(t_data *data, t_cmd *cmd);

typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
}	t_rgb;

typedef struct s_coor
{
	double	x;
	double	y;
	double	z;
}	t_coor;

// range [-1, 1]
typedef struct s_norm_vect
{
	double	x;
	double	y;
	double	z;
}	t_norm_vect;

////////////////////////////////////////////]
typedef struct s_data
{
	t_ambient_light	**light;
	t_camera	**camera;
	t_light		**light_source;
	
	t_sphere	**spheres;
	t_plane		**planes;
	t_cylinder	**cylinders;
}	t_data;


////////////////////////////////////////////]
typedef struct s_ambient_light
{
	double	ratio;
	t_rgb	color;
}	t_ambient_light;//		A

typedef struct s_camera
{
	t_coor		xyz;
	t_norm_vect	direction;
	int			fov;
}	t_camera;//			C

typedef struct s_light
{
	t_coor	xyz;
	double	ratio;
	t_rgb	color;
}	t_light;//		L

typedef struct s_sphere
{
	t_coor	xyz;
	double	radius;
	t_rgb	color;
}	t_sphere;//		sp

typedef struct s_plane
{
	t_coor	xyz;
	t_norm_vect	direction;
	t_rgb	color;
}	t_plane;//		pl


typedef struct s_cylinder
{
	t_coor	xyz;
	t_norm_vect	direction;
	double	diameter;
	double	height;
	t_rgb	color;
}	t_cylinder;//		pl


typedef struct s_square
{
	t_coor	center;
	t_norm_vect	direction;
	double	diameter;
	double	height;
	t_rgb	color;
}	t_square;//		sq
///////////////////////////////////////////////////////////////////////////////]
/********************************
		T	Tools
********************************/
double	ft_atof(char *string, int *error);
int		ato_coor(char *str, t_coor *xyz);
int		ato_rgb(char *str, t_rgb *rgb);
/********************************
		Y
********************************/
int			initialization(int ac, char **av, char **env, t_data *data);
// 

int	parse_A(t_data *data, char **raw_split);
int	parse_C(t_data *data, char **raw_split);
int	parse_L(t_data *data, char **raw_split);
int	parse_pl(t_data *data, char **raw_split);
int	parse_sp(t_data *data, char **raw_split);
int	parse_cy(t_data *data, char **raw_split);
/********************************
		Z
********************************/
void		end(t_data *data, int exit_code);


#endif
