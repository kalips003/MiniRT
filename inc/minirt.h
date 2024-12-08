/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2024/12/02 19:57:59 by kalipso          ###   ########.fr       */
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
# include <math.h>
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
# define COS_ROTA 0.99619469809174554520
# define COS_ROTA2 0.99240387650610406567
# define SIN_ROTA 0.08715574274765816587
# define SIN_ROTA2 0.00759612349389596903
# define COSSIN_ROTA 0.08682408883346516559

# define SIZE_SCREEN_Y 900
# define SIZE_SCREEN_X 1100


// typedef int	(*t_builtin)(t_data *data, t_cmd *cmd);

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
typedef struct s_calcul			t_calcul;
typedef struct s_img			t_img;
typedef struct s_eye			t_eye;

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

	int			current_camera;

}	t_eye;

typedef struct s_data
{
	void	*mlx;
	void	*win;

	// t_img	buffer; <---- buffer not used, all buffer lines commented

	t_ambient_light	**light;
	t_camera	**camera;
	t_light		**light_source;
	
	t_sphere	**spheres;
	t_plane		**planes;
	t_cylinder	**cylinders;

// shortcut to the vector camera
	t_eye		eye;
	int		is_not_moving;

}	t_data;
////////////////////////////////////////////]
////////////////////////////////////////////]

typedef struct s_calcul
{
	// ce quon voit:
	t_coor	inter_point;
	t_vect	vect_norm;
	
	t_coor	origin;
	
	t_rgb	px_color;
	t_rgb	tmp_color;

	double	dist;
	double	tmp_dist;

	t_vect	v_rotated;


	// t_sphere 	*sphere;
	// t_plane		*plane;
	// t_cylinder	*cylinder;

}	t_calcul;

typedef struct s_calcul2
{
	double	dist;
	t_rgb	px_color;

	t_coor	c0;
	t_vect	c_vect;
	
	t_coor	inter;
	t_vect	v_normal;

}	t_calcul2;

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
}	t_sphere;//		sphere

typedef struct s_plane
{
	t_coor	xyz;
	t_vect	abc;
	t_rgb	color;
	double	d;
}	t_plane;//		plane


typedef struct s_cylinder
{
	t_coor	xyz;
	t_vect	abc;
	double	diameter;
	double	height;
	t_rgb	color;
}	t_cylinder;//		cylinder

typedef struct s_cicle
{
	t_coor	c0;
	t_vect	v;
	double	radius;
	t_rgb	color;
}	t_cicle;//		circle


typedef struct s_square
{
	t_coor	center;
	t_vect	abc;
	double	diameter;
	double	height;
	t_rgb	color;
}	t_square;//		square
///////////////////////////////////////////////////////////////////////////////]
/********************************
		A
********************************/


/********************************
		B - Loop
********************************/
int 	ft_loop_empty(t_data *data);
int		ft_render_frame(t_data *data);
int		ft_render_frame_plus(t_data *data);
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn);

/********************************
		K - key press
********************************/
int		key_press(int keysym, t_data *data);
int		key_release(int keysym, t_data *data);
int mouse_clic(int button, int x, int y, void *data);
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg);
/********************************
		R	ray-tracing
********************************/
void	calculate_pixel_color(t_data *data, t_calcul *c);
void	calculate_pixel_color_simple(t_data *data, t_calcul *c);
int	ft_find_pixel_colision(t_data *data, t_calcul *c);

/********************************
		S	shadows
********************************/
void	ft_handle_shadows(t_data *data, t_calcul *c);
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
/********************************
		T	Tools
********************************/
// - camera
void	h_camera_calc_up_right_vect(t_camera *camera);
void	f_recalculate_camera(t_data *data);
// - colision
double	distance_from_sphere(t_data *data, t_calcul *calc, t_sphere *sphere);
double	distance_from_plane(t_data *data, t_calcul *calc, t_plane *p);
double	distance_from_cylinder(t_data *data, t_calcul *calc, t_cylinder *cy);
// - vector operations
int	ft_normalize_vect(t_vect *vect);
double	ft_vect_dot_product(t_vect *a, t_vect *b);
// - atof, parsing
double	ft_atof(char *string, int *error);
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
