/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/04/04 20:14:46 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <pthread.h>
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

# include "minirt_const.h"
# include "minirt_struct.h"
# include "minirt_calcul.h"
# include "minirt_objects.h"
# include "minirt_general_struct.h"


///////////////////////////////////////////////////////////////////////////////]
//
//			▒█▀▄▀█ ░█▀▀█ ▒█▄░▒█ ▒█▀▀▄ ░█▀▀█ ▀▀█▀▀ ▒█▀▀▀█ ▒█▀▀█ ▒█░░▒█
//			▒█▒█▒█ ▒█▄▄█ ▒█▒█▒█ ▒█░▒█ ▒█▄▄█ ░▒█░░ ▒█░░▒█ ▒█▄▄▀ ▒█▄▄▄█
//			▒█░░▒█ ▒█░▒█ ▒█░░▀█ ▒█▄▄▀ ▒█░▒█ ░▒█░░ ▒█▄▄▄█ ▒█░▒█ ░░▒█░░
//
///////////////////////////////////////////////////////////////////////////////]

/********************************
		A
********************************/
// A - Tools
t_img	*parse_img(t_data *data, char *path);
int		txt_already_exist(t_data *data, char *path, t_img **txt);
int		clamp(int value, int min_v, int max_v);
t_mat	*find_mat(char *mat_name, t_model *model);
// A - Screen Control
t_vect	v_cam(t_data *data, int x, int y, int aa);
t_vect	cam_vector(t_camera *cam, int x, int y, int aa);
t_vect	cam_quaternion(t_data *data, int x, int y, int aa);
// A - 1 - ini stack inside what object?
void		ini_stack(t_data *data, t_c_px *calcul);
t_ini_stk	*add_link(t_ini_stk *first, t_ini_stk *to_add);
t_ini_stk	*create_node(t_c_px *calcul);
// A - 1 - Stack operations
int		rtrn_top_stack_gamma(t_c_px *c, t_obj2 *coli, double *i_ior, double *r_ior);
void	push_stack(void **stack, void *colli, int *ptr_stack, int max_stack_index);
// AB - Progressive RT
int		ft_render_frame_multi_prog(t_data *data, int sublim);
// AB - Progressive RT - Loop
t_coor	calculate_random_ray(t_data *data, t_c_px *calcul, int num_bounce);
void	clean_buffer(t_data *data);
// 	(C)	anti aliasing
int		ft_render_frame_aa(t_data *data, int sublim);
void	w_px_buff(t_img *buff, int x, int y, unsigned int color);
/********************************
		B
********************************/
int		ft_loop(t_data *data);
int		ft_render_frame(t_data *data, int sublim);
unsigned int	calc_px_color(t_data *data, t_c_px *c, int sublim);
// 	(2)	multi-threads render
int		ft_render_frame_multi(t_data *data, int sublim);
/********************************
		C
********************************/
int			find_coli(t_data *data, t_c_px *c, int shadow, int set_dist);
t_ini_stk	*ft_fill_stack_inside(t_data *data, t_c_px *c, t_ini_stk **top_list, t_ini_stk **ptr_list);
int			something_block_the_light(t_data *data, t_c_px *c);
/********************************
		D	Objects
********************************/
// CIRCLE
int	distance_from_circle(t_c_px *calcul, void *obj, int simple);
// 	SPHERE
int	distance_from_sphere(t_c_px *calcul, void *obj, int simple);
// PLANE
int	distance_from_plane(t_c_px *calcul, void *obj, int simple);
// CYLINDER
int	distance_from_cylinder(t_c_px *calcul, void *obj, int simple);
// 	CONE
int	distance_from_cone(t_c_px *calcul, void *obj, int simple);
// 	HYPER / PARA
int	distance_from_hyper(t_c_px *calcul, void *obj, int simple);
int	distance_from_para(t_c_px *calcul, void *obj, int simple);
// 	ARROW
int	distance_from_arrow(t_c_px *calcul, void *obj, int simple);
// 	SPRITE
int	distance_from_sprite(t_c_px *calcul, void *obj, int simple);
// OBJECT
int	distance_from_object(t_c_px *calcul, void *object, int simple);
// 
void	ft_rotate_around_obj(t_c_px *calcul, t_obj2 *o, t_obj *c);
double	h_dist_triangle(t_tri *tri, t_model *o, t_c_obj *c);
void	f_return_obj_normal(t_c_px *ca, t_c_obj *c, t_object *obj);
t_coor	h_uvw(t_c_obj *c, t_model *m);
// 
void	find_inter_tri(t_bbox *node, t_model *model, t_c_obj *c, t_c_px *calcul);
/********************************
		E
********************************/
// E0
void	ft_lighting(t_data *data, t_c_px *c, int (*f_shadow)(t_data*, t_c_px*));
int		shadow_tracing(t_data *data, t_c_px *calcul);
// simple
void	ft_lighting_simple(t_data *data, t_c_px *c);
// PHONG MODEL
t_coor	ft_ambient(t_data *data, t_c_px *c);
int		ft_diffuse(t_data *data, t_c_px *c, t_light *light, int (*f_shadow)(t_data*, t_c_px*));
void	ft_reflected(t_data *data, t_c_px *c);
void	ft_refracted(t_data *data, t_c_px *c);
void	ft_specular(t_c_px *c);
// REFLECTED REFRACTED TOOLS
t_argb	what_is_reflected(t_data *data, t_c_px *calcul);
t_argb	what_is_behind(t_data *data, t_c_px *calcul);
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
t_vect	ft_vect_reflected(t_vect *incident, t_vect *normal);
t_vect	ft_vect_refracted(t_vect *incident, t_vect *normal, double η, int *neg);
/********************************
		K
********************************/
int		key_press(int keysym, t_data *data);
int		mouse_clic(int button, int x, int y, void *data);
int		mouse_move(int x, int y, void *d);
int		mouse_release(int button, int x, int y, void *d);
// extra keys
int		ft_cat_timing(t_data *data, int sublim);
int		direction_pad(int keysym, t_data *data);
int		keys_wasd(int keysym, t_data *data);
int		extra_keys(int keysym, t_data *data);
// print
void	print_clic(t_data *data, int x, int y);
// FUCTIONS 0123456789
int		fuctions_number_pad(int keysym, t_data *data);
void	ft_print_help(void);
void	f_anti_aliasing(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_change_transp(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_loop_light_ratio(t_data *data, t_obj2 *obj, int k_or_loop);
// 
void	f_set_color(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_move_obj(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_toogle_cam(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_render_normal_arrow(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_render_v_space(t_data *data, t_obj2 *obj, int k_or_loop);
void	f_progressive_rt(t_data *data, t_obj2 *obj, int k_or_loop);
// tools
void	render_normal(t_data *data, t_c_px *calcul);
void	h_render_v_space(t_data *data, t_obj *obj);
void	render_temp_added_obj(t_data *data, t_obj2 *obj_to_add);
/********************************
		P	Parsing
********************************/
// OBJ - mtl
void	f_mtl_file(t_data *data, t_object *obj, char **spl);
// OBJ - bounding box split
void	do_the_tree_splitting(t_model *model);
// 
void	ft_split_by_xyz(t_model *model, t_bbox *node, int xyz);
// OBJ - Parsing
t_model	*rtrn_obj(t_data *data, char *path, t_object *obj);
t_tri	*h_parse_face(char **spl, t_parse_model *p, int *err);
// 
int		h_read_obj(t_data *data, t_model *model, int fd, t_object *obj);
// Param
int		parse_reste(t_data *data, char **raw_split, t_param *obj);
int		parse_texture(t_data *data, char *path, t_param *obj);
// 
int		parse_shininess(t_data *data, char *raw, t_param *obj);
int		parse_transparence(t_data *data, char *raw, t_param *obj);
int		parse_mirror(t_data *data, char *raw, t_param *obj);
int		parse_color2(t_data *data, char *raw, t_param *obj);
int		parse_light(t_data *data, char *raw, t_param *obj);
// 
int		parse_nmap(t_data *data, char *path, t_param *obj);
int		parse_amap(t_data *data, char *path, t_param *obj);
int		parse_ao(t_data *data, char *path, t_param *obj);
int		parse_smap(t_data *data, char *path, t_param *obj);
int		parse_rmap(t_data *data, char *path, t_param *obj);
// Prims
int		ft_parse_line(t_data *data, char *line);
int		h_parse_vect_space(t_obj *obj, t_vect *view);
int		parse_a(t_data *data, char **raw_split);
int		parse_l(t_data *data, char **raw_split);
int		parse_c(t_data *data, char **raw_split);
// 
int		parse_ci(t_data *data, char **raw_split);
int		parse_pl(t_data *data, char **raw_split);
int		parse_sp(t_data *data, char **raw_split);
int		parse_cy(t_data *data, char **raw_split);
int		parse_co(t_data *data, char **raw_split);
// 
int		parse_hy(t_data *data, char **raw_split);
int		parse_pa(t_data *data, char **raw_split);
int		parse_ar(t_data *data, char **raw_split);
int		parse_xi(t_data *data, char **raw_split);
int		parse_obj(t_data *data, char **raw_split);

t_mat	*find_mat(char *mat_name, t_model *model);
/********************************
		T	Tools
********************************/
// atof
int		ft_atof(char *string, double *rtrn);
int		ato_coor(char *str, t_coor *xyz);
int		ato_rgb(char *str, t_rgb *rgb);
int		ato_argb(char *str, t_argb *argb);
// other
void	create_vector_space(t_obj *obj);
double	h_smalest_delta(double a, double b);
t_argb	dual_color(t_argb *color1, t_rgb *color2, double dist);
void	ini_new_calcul_struct(t_c_px *calcul, t_c_px *to_ini, int bit);
void	recalculate_obj_const(t_obj2 *obj);
// rotation
t_vect	dbl_quaternion_rota(t_obj *obj, double angle_α, double angle_β);
t_vect	quaternion_rota(t_vect *v, t_vect *axis_rota, double angle_α, int posi_neg);
void	rotation_obj(t_obj *obj, t_vect *axis_rota, double angle_α, int posi_neg);
t_vect	mult_3x3_vect(t_obj *o3, t_vect *v);
// vector mouv
void	move_point(t_coor* p, t_vect *v, double incre);
t_coor	new_moved_point(t_coor* p, t_vect *v, double dist);
t_vect	vect_ab(t_coor* a, t_coor* b);
t_vect	vect_ab_norm(t_coor* a, t_coor* b);
// vector ope
int		ft_normalize_vect(t_vect *vect);
double	ft_dot_p(t_vect *a, t_vect *b);
t_vect	ft_cross_product(t_vect *u, t_vect *v);
t_vect	ft_cross_product_norm(t_vect *u, t_vect *v);
double	dist_two_points(t_coor *a, t_coor *b);
// scaling
t_vect	scale_vect(t_vect v, double scalar);
t_coor	scale_point(t_coor p, double scalar);
t_argb	scale_argb(t_argb argb, double scalar);
// 	textures
void	update_mat_w_txt(t_c_px *calcul, t_obj2 *obj, double u, double v);
int		is_there_txt(t_param *p);
t_argb	return_px_img(t_img *img, double x, double y);
t_vect	return_vect_img(t_img *img, double x, double y);
int		return_alpha_img(t_img *img, double x, double y);
/********************************
		Y
********************************/
void	initialization(int ac, char **av, t_data *data);
void	read_file(int ac, char **av, t_data *data);
/********************************
		Z
********************************/
void	end(t_data *data, int exit_code, int full_clean);
int		end2(t_data *data);
// 
void	destroy_models(t_data *data);

#endif
