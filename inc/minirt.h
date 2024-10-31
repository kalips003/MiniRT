/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2024/10/31 16:37:08 by kalipso          ###   ########.fr       */
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

# include "../mlx_linux/mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>

# include "libft.h"

///////////////////////////////////////////////////////////////////////////////]
//
//			▒█▀▄▀█ ░█▀▀█ ▒█▄░▒█ ▒█▀▀▄ ░█▀▀█ ▀▀█▀▀ ▒█▀▀▀█ ▒█▀▀█ ▒█░░▒█
//			▒█▒█▒█ ▒█▄▄█ ▒█▒█▒█ ▒█░▒█ ▒█▄▄█ ░▒█░░ ▒█░░▒█ ▒█▄▄▀ ▒█▄▄▄█
//			▒█░░▒█ ▒█░▒█ ▒█░░▀█ ▒█▄▄▀ ▒█░▒█ ░▒█░░ ▒█▄▄▄█ ▒█░▒█ ░░▒█░░
//
///////////////////////////////////////////////////////////////////////////////]

# define PI 3.14159265358979323846
# define SIZEX 500
# define SIZEY 500

// typedef int	(*t_builtin)(t_data *data, t_cmd *cmd);

typedef struct s_rgb			t_rgb;
typedef struct s_coor			t_coor;
typedef struct s_norm_vect		t_norm_vect;
typedef struct s_ambient_light	t_ambient_light;
typedef struct s_camera			t_camera;
typedef struct s_light			t_light;
typedef struct s_sphere			t_sphere;
typedef struct s_plane			t_plane;
typedef struct s_cylinder		t_cylinder;
typedef struct s_square			t_square;
typedef struct s_img			t_img;

typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
}	t_rgb;

typedef struct s_coor
{
	float	x;
	float	y;
	float	z;
}	t_coor;

// range [-1, 1]
typedef struct s_norm_vect
{
	float	dx;
	float	dy;
	float	dz;
}	t_norm_vect;

typedef struct s_img
{
	void	*img;
	char	*addr;

	int		bpp;
	int		ll;
	int		end;
	int		sz_x;
	int		sz_y;
}	t_img;
////////////////////////////////////////////]
typedef struct s_data
{
	void	*mlx;
	void	*win;

	t_img	buffer;

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
	float	ratio;
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
	float	ratio;
	t_rgb	color;
}	t_light;//		L

typedef struct s_sphere
{
	t_coor	xyz;
	float	diameter;
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
	float	diameter;
	float	height;
	t_rgb	color;
}	t_cylinder;//		pl


typedef struct s_square
{
	t_coor	center;
	t_norm_vect	direction;
	float	diameter;
	float	height;
	t_rgb	color;
}	t_square;//		sq
///////////////////////////////////////////////////////////////////////////////]

/********************************
		B
********************************/
int		ft_loop(t_data *data);
int		key_press(int keysym, t_data *data);
int		key_release(int keysym, t_data *data);
/********************************
		T	Tools
********************************/
float	ft_atof(char *string, int *error);
int		ato_coor(char *str, t_coor *xyz);
int		ato_rgb(char *str, t_rgb *rgb);
/********************************
		Y
********************************/
void	initialization(int ac, char **av, t_data *data);
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
void	end(t_data *data, int exit_code);
int	end2(t_data *data);


#endif
