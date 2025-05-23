/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_builtin_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	create_vector_space(t_obj *obj);
double	h_smalest_delta(double a, double b);
t_argb	dual_color(t_argb *color1, t_rgb *color2, double dist);
void	ini_new_calcul_struct(t_c_px *calcul, t_c_px *to_ini, int bit);
void	recalculate_obj_const(t_obj2 *obj);

///////////////////////////////////////////////////////////////////////////////]
// takes an obj with a view vector filled, create the vector space
// 
// Right = Y x Camera = {-Cz, 0, Cx}
// Up = Right x Camera = {-CxCy, Cx²+Cz², -CyCz}
void	create_vector_space(t_obj *obj)
{
	if (fabs(obj->view.dx) < EPSILON && fabs(obj->view.dz) < EPSILON)
	{
		if (obj->view.dy > 0)
			*obj = (t_obj){obj->c0, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}, \
				{1.0, 0.0, 0.0}};
		else
			*obj = (t_obj){obj->c0, {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}, \
				{1.0, 0.0, 0.0}};
		return ;
	}
	obj->right.dx = -obj->view.dz;
	obj->right.dy = 0;
	obj->right.dz = obj->view.dx;
	ft_normalize_vect(&obj->right);
	obj->up.dx = -obj->view.dx * obj->view.dy;
	obj->up.dy = obj->view.dx * obj->view.dx + obj->view.dz * obj->view.dz;
	obj->up.dz = -obj->view.dy * obj->view.dz;
	ft_normalize_vect(&obj->up);
}

///////////////////////////////////////////////////////////////////////////////]
// 	return smalest positive
double	h_smalest_delta(double a, double b)
{
	if (a < EPSILON && b < EPSILON)
		return (-1.0);
	if (a < EPSILON)
		return (b);
	if (b < EPSILON)
		return (a);
	return (a * (a < b) + b * (b < a));
}

///////////////////////////////////////////////////////////////////////////////]
// scale color based on a [0,1] distance
t_argb	dual_color(t_argb *color1, t_rgb *color2, double dist)
{
	t_argb	rtrn;

	rtrn.a = color1->a;
	rtrn.r = (int)((color2->r - color1->r) * dist + color1->r);
	rtrn.g = (int)((color2->g - color1->g) * dist + color1->g);
	rtrn.b = (int)((color2->b - color1->b) * dist + color1->b);
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// bit control depth, 0b1 = transparence_depth; 0b10 = reflected_depth
void	ini_new_calcul_struct(t_c_px *calcul, t_c_px *to_ini, int bit)
{
	to_ini->print = calcul->print + !!(calcul->print);
	to_ini->transparence_depth = calcul->transparence_depth + (bit == 0b1);
	to_ini->reflected_depth = calcul->reflected_depth + (bit == 0b10);
	ft_memcpy(to_ini->inside, calcul->inside, sizeof(calcul->inside));
	to_ini->stack_top = calcul->stack_top;
	to_ini->dist = -1.0;
}

///////////////////////////////////////////////////////////////////////////////]
void	recalculate_obj_const(t_obj2 *obj)
{
	t_cone	*c;
	t_arrow	*a;

	if (!obj)
		return ;
	if (obj->type == CONE)
	{
		c = (t_cone *)obj;
		c->apex = new_moved_point(&obj->O.c0, &obj->O.view, c->h);
		c->slope = pow(c->radius, 2.0) / pow(c->h, 2.0);
		c->angle = atan(c->radius / c->h);
	}
	else if (obj->type == CYLINDER)
		((t_cylinder *)obj)->xyz_other = new_moved_point(&obj->O.c0, \
			&obj->O.view, ((t_cylinder *)obj)->height);
	else if (obj->type == PLANE || obj->type == SPRITE)
		((t_plane *)obj)->d = -ft_dot_p(&obj->O.view, (t_vect *)&obj->O.c0);
	else if (obj->type == ARROW)
	{
		a = (t_arrow *)obj;
		a->xyz_other = new_moved_point(&obj->O.c0, &obj->O.view, a->h * 2 / 3);
		a->apex = new_moved_point(&obj->O.c0, &obj->O.view, a->h);
		a->slope = (9.0 * pow(a->radius, 2.0)) / pow(a->h, 2.0);
	}
}
