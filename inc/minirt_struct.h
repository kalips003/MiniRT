/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_struct.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2024/12/14 15:27:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_STRUCT_H
# define MINIRT_STRUCT_H

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
# include <time.h>
// # include <sys/types.h>

// # include "../mlx_linux/mlx.h"
// # include <X11/X.h>
// # include <math.h>
// # include <X11/keysym.h>

// # include "libft.h"

///////////////////////////////////////////////////////////////////////////////]
//
//			▒█▀▄▀█ ░█▀▀█ ▒█▄░▒█ ▒█▀▀▄ ░█▀▀█ ▀▀█▀▀ ▒█▀▀▀█ ▒█▀▀█ ▒█░░▒█
//			▒█▒█▒█ ▒█▄▄█ ▒█▒█▒█ ▒█░▒█ ▒█▄▄█ ░▒█░░ ▒█░░▒█ ▒█▄▄▀ ▒█▄▄▄█
//			▒█░░▒█ ▒█░▒█ ▒█░░▀█ ▒█▄▄▀ ▒█░▒█ ░▒█░░ ▒█▄▄▄█ ▒█░▒█ ░░▒█░░
//
///////////////////////////////////////////////////////////////////////////////]

typedef struct s_rgb			t_rgb;
typedef struct s_coor			t_coor;
typedef struct s_vect		t_vect;
typedef struct s_ambient_light	t_ambient_light;
typedef struct s_camera			t_camera;
typedef struct s_light			t_light;
typedef struct s_sphere			t_sphere;
typedef struct s_plane			t_plane;
typedef struct s_cylinder		t_cylinder;
typedef struct s_square			t_square;
typedef struct s_calcul_px			t_calcul_px;
typedef struct s_img			t_img;
typedef struct s_eye			t_eye;
typedef struct s_eye_v2			t_eye_v2;
typedef struct s_data			t_data;

////////////////////////////////////////////]
////////////////////////////////////////////]
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

	int		bpp;
	int		ll;
	int		end;
	int		sz_x;
	int		sz_y;
}	t_img;
////////////////////////////////////////////]
////////////////////////////////////////////]

typedef struct s_eye
{
	t_camera	*c;
// unit angle in radians per pixel
	double		px;
// position of upper-left screen (rotation in radian)
	double		px0;
	double		py0;

// 6 Const value f(camera) to compute temp rotation vector V'
	// (X) = cos²Cx + sin²(Cx(Ux²-Uy²-Uz²)+2Ux(UyCy+UzCz)) + 2cossin(UyCz-UzCy) == cos²Cx + sin² * (A1x) + (B1x)
	// (Y) = cos²Cy + sin²(Cx(-Ux²+Uy²-Uz²)+2Uy(UxCx+UzCz)) + 2cossin(UzCx-UxCz) == cos²Cy + sin² * (A1y) + (B1y)
	// (Z) = cos²Cz + sin²(Cx(-Ux²-Uy²+Uz²)+2Uz(UxCx+UyCy)) + 2cossin(UxCy-UyCx) == cos²Cz + sin² * (A1z) + (B1z)
	double A1x;
	double A1y;
	double A1z;

	double B1x;
	double B1y;
	double B1z;

}	t_eye;

typedef struct s_eye_v2
{
	t_camera	*c;
// unit angle in radians per pixel
	double		px;
// position of upper-left screen (rotation in radian)
	double		px0;
	double		py0;

	int			current_camera;

}	t_eye_v2;

typedef struct s_data
{
	void	*mlx;
	void	*win;

	// t_img	buffer; <---- buffer not used, all buffer lines commented

	t_ambient_light	**bg_light;
	t_camera	**camera;
	t_light		**light_source;
	
	t_sphere	**spheres;
	t_plane		**planes;
	t_cylinder	**cylinders;

	
// shortcut to the vector camera
	t_eye_v2	eye;
	int			current_camera;
	int		is_not_moving;

	char **av;
	time_t last_modif_time;

}	t_data;
////////////////////////////////////////////]
////////////////////////////////////////////]

////////////////////////////////////////////]
typedef struct s_ambient_light
{
	double	ratio;
	t_rgb	color;
}	t_ambient_light;//		A

typedef struct s_camera
{
	t_coor		xyz;
	t_vect		view;
	t_vect		up;
	t_vect		right;
	int			fov;
	double		fov_cst_x;// = tan(fov/2) / screenX
	double		fov_cst_y;// = tan(fov/2) / screenY
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
	double	diameter;
	double	radius;
	t_rgb	color;
}	t_sphere;//		sp

typedef struct s_plane
{
	t_coor	xyz;
	t_vect	abc;
	t_rgb	color;
	double	d;
}	t_plane;//		pl


typedef struct s_cylinder
{
	t_coor	xyz;
	t_coor	xyz_other;
	t_vect	abc;
	double	diameter;
	double	height;
	double	radius;
	t_rgb	color;
}	t_cylinder;//		pl

typedef struct s_cicle
{
	t_coor	c0;
	t_vect	v;
	double	radius;
	t_rgb	color;
}	t_circle;//		circle


typedef struct s_square
{
	t_coor	center;
	t_vect	abc;
	double	diameter;
	double	height;
	t_rgb	color;
}	t_square;//		sq
///////////////////////////////////////////////////////////////////////////////]

typedef struct s_calcul_px
{
	// camera origin + vector
	t_coor	c0;
	t_vect	v_view;

	// point intersection + vector normal to the surface
	t_coor	inter;
	t_vect	v_normal;
	
	t_rgb	px_color;
	double	dist;

	t_coor	tmp_color;
	// double	tmp_dist;

	double dist_light;
	t_vect	v_light;
	void	*object;
	double	cos_angle;

}	t_calcul_px;


typedef struct s_sphere_calc {
	double x0;
	double y0;
	double z0;

	double a;
	double b;
	double c;

	double Δ;

	double	det1;
	double	det2;

	double dist;
} t_sphere_calc;

typedef struct s_plane_calc {
	double	top;
	double	bot;

	double	dist;
} t_plane_calc;

typedef struct s_cylinder_calc {
	double	radius;

	double	A;
	double	B;
	
	double x0;
	double y0;
	double z0;
	double x1;
	double y1;
	double z1;

	double a;
	double b;
	double c;

	double Δ;
	double	det1;
	double	det2;

	double	dist_h;
	double	dist;
	t_coor	projec_point;
} t_cylinder_calc;

#endif
