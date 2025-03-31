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

t_vect	dbl_quaternion_rota(t_obj *obj, double angle_a, double angle_b);
t_vect	quaternion_rota(t_vect *v, t_vect *axis_rota, double angle, int sign);
void	rotation_obj(t_obj *obj, t_vect *axis_rota, double angle, int sign);

///////////////////////////////////////////////////////////////////////////////]
// return the rotated vector view object, around
// 		angle_a ~ UP then around angle_b ~ RIGHT
// Q rotation combined = qb.qa
t_vect	dbl_quaternion_rota(t_obj *obj, double angle_a, double angle_b)
{
	t_camera_calc	c;
	t_vect			rotated;

	c.cA = cos(angle_a / 2);
	c.sA = sin(-angle_a / 2);
	c.cB = cos(angle_b / 2);
	c.sB = sin(-angle_b / 2);
	c.Qw = c.cA * c.cB - c.sA * c.sB * (obj->right.dx * obj->up.dx + \
		obj->right.dy * obj->up.dy + obj->right.dz * obj->up.dz);
	c.Qi = c.cB * c.sA * obj->up.dx + c.cA * c.sB * obj->right.dx + \
		c.sA * c.sB * (obj->right.dy * obj->up.dz - obj->right.dz * obj->up.dy);
	c.Qj = c.cB * c.sA * obj->up.dy + c.cA * c.sB * obj->right.dy + \
		c.sA * c.sB * (obj->right.dz * obj->up.dx - obj->right.dx * obj->up.dz);
	c.Qk = c.cB * c.sA * obj->up.dz + c.cA * c.sB * obj->right.dz + \
		c.sA * c.sB * (obj->right.dx * obj->up.dy - obj->right.dy * obj->up.dx);
	rotated.dx = obj->view.dx * (c.Qw * c.Qw + c.Qi * c.Qi - c.Qj * c.Qj - \
		c.Qk * c.Qk) + 2 * obj->view.dy * (c.Qi * c.Qj - c.Qw * c.Qk) + 2 * \
		obj->view.dz * (c.Qi * c.Qk + c.Qw * c.Qj);
	rotated.dy = obj->view.dy * (c.Qw * c.Qw + c.Qj * c.Qj - c.Qi * c.Qi - \
		c.Qk * c.Qk) + 2 * obj->view.dz * (c.Qj * c.Qk - c.Qw * c.Qi) + 2 * \
		obj->view.dx * (c.Qi * c.Qj + c.Qw * c.Qk);
	rotated.dz = obj->view.dz * (c.Qw * c.Qw + c.Qk * c.Qk - c.Qi * c.Qi - \
		c.Qj * c.Qj) + 2 * obj->view.dx * (c.Qi * c.Qk - c.Qw * c.Qj) + 2 * \
		obj->view.dy * (c.Qj * c.Qk + c.Qw * c.Qi);
	return (ft_normalize_vect(&rotated), rotated);
}

///////////////////////////////////////////////////////////////////////////////]
// rotate the vector of fixed angle around axis
t_vect	quaternion_rota(t_vect *v, t_vect *axis_rota, double angle, int sign)
{
	t_vect		rtrn;
	t_q_rota	c;

	c.cosa = cos(angle / 2);
	c.sina = sign * sin(angle / 2);
	c.qw = c.cosa;
	c.qi = c.sina * axis_rota->dx;
	c.qj = c.sina * axis_rota->dy;
	c.qk = c.sina * axis_rota->dz;
	c.w = -(c.qi * v->dx + c.qj * v->dy + c.qk * v->dz);
	c.qx = c.qw * v->dx + c.qj * v->dz - c.qk * v->dy;
	c.qy = c.qw * v->dy + c.qk * v->dx - c.qi * v->dz;
	c.qz = c.qw * v->dz + c.qi * v->dy - c.qj * v->dx;
	rtrn.dx = -c.qi * c.w + c.qx * c.qw - c.qy * c.qk + c.qz * c.qj;
	rtrn.dy = -c.qj * c.w + c.qy * c.qw - c.qz * c.qi + c.qx * c.qk;
	rtrn.dz = -c.qk * c.w + c.qz * c.qw - c.qx * c.qj + c.qy * c.qi;
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// rotate object of angle around axis
void	rotation_obj(t_obj *obj, t_vect *axis_rota, double angle, int sign)
{
	t_vect	new_view;
	t_vect	new_up;
	t_vect	new_right;

	new_right = quaternion_rota(&obj->right, axis_rota, angle, sign);
	new_up = quaternion_rota(&obj->up, axis_rota, angle, sign);
	new_view = quaternion_rota(&obj->view, axis_rota, angle, sign);
	obj->view = new_view;
	obj->up = new_up;
	obj->right = new_right;
}
