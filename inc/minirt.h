/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/01/17 15:34:53 by kalipso          ###   ########.fr       */
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
# include "minirt_struct.h"

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
# define EPSILON 1e-6
# define SCALAR_LIGHT_DIST 1000.0
# define SCALAR_REFLECTION 0.4
# define SCALAR_SHINY 10.0

# define SIZE_SCREEN_Y 500
# define SIZE_SCREEN_X 600


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
typedef struct s_data			t_data;


///////////////////////////////////////////////////////////////////////////////]
/********************************
		A
********************************/
void	ft_handle_shadows_simple(t_data *data, t_calcul_px *c);
double	distance_from_cylinder_v2(t_calcul_px *calcul, t_cylinder *cy);
void	h_dist_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
t_rgb	ft_textures_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
/********************************
		B
********************************/
int	ft_loop_empty_v2(t_data *data);
int	ft_loop_empty(t_data *data);
int	ft_render_frame(t_data *data);
int	ft_render_frame_plus(t_data *data);
void	calculate_pixel_color_simple(t_data *data, t_calcul_px *c);
void	calculate_pixel_color_plus(t_data *data, t_calcul_px *c);
/********************************
		C
********************************/
int	ft_find_pixel_colision(t_data *data, t_calcul_px *c);

/********************************
		D
********************************/
double	distance_from_plane(t_calcul_px *calcul, t_plane *p);
double	distance_from_cicle(t_calcul_px *calcul, t_circle circle);
double	distance_from_cylinder(t_calcul_px *calcul, t_cylinder *cy);

/********************************
		E
********************************/
void	ft_handle_shadows_plus(t_data *data, t_calcul_px *c);
void	ft_handle_shadows_v2(t_data *data, t_calcul_px *c);
t_coor	ft_ambient(t_data *data, t_calcul_px *c);
void	ft_diffuse_and_reflected(t_data *data, t_calcul_px *c, t_light *light);
t_coor	ft_diffuse(t_data *data, t_calcul_px *c);
t_coor	ft_reflected(t_data *data, t_calcul_px *c);
// 
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
int something_block_the_light(t_data *data, t_calcul_px *c, t_light *light);
// 
int	in_shadow_of_sphere(t_calcul_px *calcul, t_sphere *sphere);
int	in_shadow_of_plane(t_calcul_px *calcul, t_plane *p);
int	in_shadow_of_cicle(t_calcul_px *calcul, t_circle circle);
int	in_shadow_of_cylinder(t_calcul_px *calcul, t_cylinder *cy);
/********************************
		K
********************************/
int	key_press(int keysym, t_data *data);
int	key_release(int keysym, t_data *data);
int	mouse_clic(int button, int x, int y, void *data);
void	print_clic(t_data *data, int x, int y);
/********************************
		O	Objects
********************************/
// 	SPHERE
double	distance_from_sphere(t_calcul_px *calcul, t_sphere *sphere);
double	distance_from_sphere_v2(t_calcul_px *calcul, t_sphere *sphere);
void	h_dist_sphere(t_calcul_px *calcul, t_sphere *sphere, double dist, int inside);
t_rgb	ft_txt_sphere(t_calcul_px *calcul);
t_vect	ft_nmap_sphere(t_calcul_px *calcul);
// 
double	distance_from_cylinder_v2(t_calcul_px *calcul, t_cylinder *cy);
void	h_dist_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
t_rgb	ft_textures_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
t_vect	ft_nmap_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
/********************************
		T	Tools
********************************/
// atof
double	ft_atof(char *string, int *error);
int		ato_coor(char *str, t_coor *xyz);
int		ato_rgb(char *str, t_rgb *rgb);
// camera
void	h_camera_calc_up_right_vect(t_camera *camera);
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg);
// math
int	ft_normalize_vect(t_vect *vect);
double	ft_vect_dot_product(t_vect *a, t_vect *b);
t_vect	ft_vect_cross_product(t_vect *u, t_vect *v);
double	dist_two_points(t_coor *a, t_coor *b);
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn);
double h_smalest_Δ(double a, double b);

/********************************
		Y
********************************/
void	initialization(int ac, char **av, t_data *data);
void	read_file(int ac, char **av, t_data *data);
// 
int	parse_A(t_data *data, char **raw_split);
int	parse_C(t_data *data, char **raw_split);
int	parse_L(t_data *data, char **raw_split);
int	parse_pl(t_data *data, char **raw_split);
int	parse_sp(t_data *data, char **raw_split);
int	parse_cy(t_data *data, char **raw_split);
// 
int	parse_reste(t_data *data, char **raw_split, void *obj);
/********************************
		Z
********************************/
void	end(t_data *data, int exit_code, int full_clean);
int	end2(t_data *data);


#endif
