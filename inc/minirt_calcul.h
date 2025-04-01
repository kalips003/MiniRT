/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_calcul.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 16:55:43 by marvin            #+#    #+#             */
/*   Updated: 2025/04/01 15:18:20 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_CALCUL_H
# define MINIRT_CALCUL_H

# include "minirt_general_struct.h"

///////////////////////////////////////////////////////////////////////////////]
//
// 				░█▀▀█ ─█▀▀█ ░█─── ░█▀▀█ ░█─░█ ░█─── ░█▀▀▀█ 
// 				░█─── ░█▄▄█ ░█─── ░█─── ░█─░█ ░█─── ─▀▀▀▄▄ 
// 				░█▄▄█ ░█─░█ ░█▄▄█ ░█▄▄█ ─▀▄▄▀ ░█▄▄█ ░█▄▄▄█
//
///////////////////////////////////////////////////////////////////////////////]


///////////////////////////////////////////////////////////////////////////////]

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
// 	CIRCLE
typedef struct s_circle_calc {
	double	top;
	double	bot;

	double	pl_cst;
	t_coor	inter_temp;

	double	dist;
	double	dist_center;
} t_circle_calc;

///////////////////////////////////////////////////////////////////////////////]
// 	SPHERE
typedef struct s_sphere_calc {
	// t_sphere	*sphere;
	
	double	x0;
	double	y0;
	double	z0;

	double 	a;
	double 	b;
	double 	c;

	double	delta;

	double	det1;
	double	det2;

	double 	dist;
	int		inside;
	double	u;
	double	v;
} t_c_sphere;


///////////////////////////////////////////////////////////////////////////////]
// 	PLANE
typedef struct s_plane_calc {
	t_vect	o_to_inter;
	double	top;
	double	bot;

	double	dist;

	double	u;
	double	v;
} t_c_plane;


///////////////////////////////////////////////////////////////////////////////]
// 	CYLINDER
typedef struct s_cylinder_calc_v1 {

	double	A;
	double	B;
	
	double	x0;
	double	y0;
	double	z0;
	double	x1;
	double	y1;
	double	z1;

	double	a;
	double	b;
	double	c;

	double	delta;
} t_c_cyl2;

typedef struct s_cylinder_calc_v2 {
	double	det1;
	double	det2;

	double	dist_h;
	double	dist;
	int		in;
	t_coor	projec_p;
	t_circle	circle;
} t_c_cy;

///////////////////////////////////////////////////////////////////////////////]
// 	CONE
typedef struct s_cone_calc_v1 {
	double	p;
	double	q;

	double	a1;
	double	a2;
	double	a3;

	double	b1;
	double	b2;
	double	b3;

	double	A;
	double	B;
	double	C;

	double	delta;
	
	double	det1;
	double	det2;
	
	double	d_a1;
	double	d_a2;
} t_cone_calc_v1;

typedef struct s_cone_calc_v2 {
	double	dist_apex;
	double	dist;

	double	c_height;
	int		inside;
	t_coor	projec_point;
	t_vect	apex_to_camera;
	t_circle	circle;
} t_cone_calc_v2;

///////////////////////////////////////////////////////////////////////////////]
// 	HYPERBOLOID / PARABOLOID
typedef struct s_hyper_calc {
	double	a2;
	double	b2;
	double	c2;

	double	a;
	double	b;
	double	c;

	double	delta;

	double	det1;
	double	det2;
	double	dist;
	
	t_coor	new_o;
	t_vect	rot_v;

	int		inside;

}	t_hyper_calc;

///////////////////////////////////////////////////////////////////////////////]
// 	ARROW
typedef struct s_arrow_calc {
	double	det1;
	double	det2;

	double	dist_h;
	double	dist;
	t_coor	projec_point;
	t_cone		co;
	t_cylinder	cy;
} t_arrow_calc;

///////////////////////////////////////////////////////////////////////////////]
// 	OBJECT
typedef struct s_obj_calc {
	t_coor	new_o;
	t_vect	v_rotate;
	double	dist;
	double	t_enter;
	double	t_exit;
	t_tri	*t;

	t_vect	e1;
	t_vect	e2;
	t_coor	inter2;
	double	det;
	t_bbox	*bbox;
	double	size;
	t_coor	uvw;
	double	u;
	double	v;	

} t_c_obj;

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
typedef struct s_camera_calc {
	double cA;
	double sA;
	double cB;
	double sB;

	double Qw;
	double Qi;
	double Qj;
	double Qk;
} t_camera_calc;

typedef struct s_q_rota {
	double	cosa;
	double	sina;
	double	qw;
	double	qi;
	double	qj;
	double	qk;
	double	w;
	double	qx;
	double	qy;
	double	qz;
} t_q_rota;

// temp linked list to fill the stack inside
typedef	struct s_stack_inside {
	double	dist;
	void	*obj;
	struct s_stack_inside	*next;
}	t_ini_stk;

// temp struct to create the binary tree of .obj
typedef struct s_make_tree
{
	double	avg;
	t_tri	*ptr;
	t_tri	*ptr_r;
	t_tri	*ptr_l;
}	t_mtree;

// distance calcul for a single triangle
typedef struct s_calc_dist_tri {
	t_coor	p1;
	t_coor	p2;
	t_coor	p3;
	t_vect	h;
	t_vect	s;
	double	u;
	t_vect	q;
	double	v;
	double	t;
}	t_calc_dist_tri;

// for parsing .obj
typedef struct	s_parse_model
{
	t_model		*model;
	t_object	*obj;
	t_tri		*current_tri;
	t_mat		*current_mat;
	int			num_v;
	int			num_vt;
	int			num_vn;
}	t_parse_model;

#endif