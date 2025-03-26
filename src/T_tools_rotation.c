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

t_vect	combined_quaternion_rotation(t_obj *obj, double angle_a, double angle_b);
t_vect	quaternion_rotation(t_vect *v, t_vect *axis_rota, double angle_a, int posi_neg);
void	rotation_obj(t_obj *obj, t_vect *axis_rota, double angle_a, int posi_neg);

///////////////////////////////////////////////////////////////////////////////]
// return the rotated vector view object, around
// 		angle_a ~ UP then around angle_b ~ RIGHT
// Q rotation combined = qb.qa
t_vect	combined_quaternion_rotation(t_obj *obj, double angle_a, double angle_b)
{
	t_camera_calc	c;
	t_vect			rotated;

	c.cosA = cos(angle_a / 2);
	c.sinA = sin(-angle_a / 2);
	c.cosB = cos(angle_b / 2);
	c.sinB = sin(-angle_b / 2);
	c.Qw = c.cosA * c.cosB - c.sinA * c.sinB * (obj->right.dx * obj->up.dx + obj->right.dy * obj->up.dy + obj->right.dz * obj->up.dz);
	c.Qi = c.cosB * c.sinA * obj->up.dx + c.cosA * c.sinB * obj->right.dx + c.sinA * c.sinB * (obj->right.dy * obj->up.dz - obj->right.dz * obj->up.dy);
	c.Qj = c.cosB * c.sinA * obj->up.dy + c.cosA * c.sinB * obj->right.dy + c.sinA * c.sinB * (obj->right.dz * obj->up.dx - obj->right.dx * obj->up.dz);
	c.Qk = c.cosB * c.sinA * obj->up.dz + c.cosA * c.sinB * obj->right.dz + c.sinA * c.sinB * (obj->right.dx * obj->up.dy - obj->right.dy * obj->up.dx);
	rotated.dx = obj->view.dx * (c.Qw * c.Qw + c.Qi * c.Qi - c.Qj * c.Qj - c.Qk * c.Qk) + 2 * obj->view.dy * (c.Qi * c.Qj - c.Qw * c.Qk) + 2 * obj->view.dz * (c.Qi * c.Qk + c.Qw * c.Qj);
	rotated.dy = obj->view.dy * (c.Qw * c.Qw + c.Qj * c.Qj - c.Qi * c.Qi - c.Qk * c.Qk) + 2 * obj->view.dz * (c.Qj * c.Qk - c.Qw * c.Qi) + 2 * obj->view.dx * (c.Qi * c.Qj + c.Qw * c.Qk);
	rotated.dz = obj->view.dz * (c.Qw * c.Qw + c.Qk * c.Qk - c.Qi * c.Qi - c.Qj * c.Qj) + 2 * obj->view.dx * (c.Qi * c.Qk - c.Qw * c.Qj) + 2 * obj->view.dy * (c.Qj * c.Qk + c.Qw * c.Qi);
	ft_normalize_vect(&rotated);
	return (rotated);
}

///////////////////////////////////////////////////////////////////////////////]
// rotate the vector of fixed angle around axis
t_vect	quaternion_rotation(t_vect *v, t_vect *axis_rota, double angle_a, int posi_neg)
{
	t_vect	rtrn;
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

	cosa = cos(angle_a / 2);
	sina = posi_neg * sin(angle_a / 2);
	qw = cosa;
	qi = sina * axis_rota->dx;
	qj = sina * axis_rota->dy;
	qk = sina * axis_rota->dz;
	w = -(qi * v->dx + qj * v->dy + qk * v->dz);
	qx = qw * v->dx + qj * v->dz - qk * v->dy;
	qy = qw * v->dy + qk * v->dx - qi * v->dz;
	qz = qw * v->dz + qi * v->dy - qj * v->dx;
	rtrn.dx = -qi * w + qx * qw - qy * qk + qz * qj;
	rtrn.dy = -qj * w + qy * qw - qz * qi + qx * qk;
	rtrn.dz = -qk * w + qz * qw - qx * qj + qy * qi;
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// rotate object of angle around axis
void	rotation_obj(t_obj *obj, t_vect *axis_rota, double angle_a, int posi_neg)
{
	t_vect	new_view;
	t_vect	new_up;
	t_vect	new_right;

	new_right = quaternion_rotation(&obj->right, axis_rota, angle_a, posi_neg);
	new_up = quaternion_rotation(&obj->up, axis_rota, angle_a, posi_neg);
	new_view = quaternion_rotation(&obj->view, axis_rota, angle_a, posi_neg);
	obj->view = new_view;
	obj->up = new_up;
	obj->right = new_right;
}
