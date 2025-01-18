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

int	ft_normalize_vect(t_vect *vect);
double	ft_vect_dot_product(t_vect *a, t_vect *b);
t_vect	ft_vect_cross_product(t_vect *u, t_vect *v);
double	dist_two_points(t_coor *a, t_coor *b);
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn);
double h_smalest_Δ(double a, double b);

///////////////////////////////////////////////////////////////////////////////]
// Normalize vector
int	ft_normalize_vect(t_vect *vect)
{
	double normal_magnitude;

	normal_magnitude = sqrt(vect->dx * vect->dx + vect->dy * vect->dy + vect->dz * vect->dz);
	if (normal_magnitude == 0.0)
		return (put("VECTOR = [%f, %f, %f]\n", vect->dx, vect->dy, vect->dz), 1);
	vect->dx = vect->dx / normal_magnitude;
	vect->dy = vect->dy / normal_magnitude;
	vect->dz = vect->dz / normal_magnitude;
	return (0);
}


///////////////////////////////////////////////////////////////////////////////]
double	ft_vect_dot_product(t_vect *a, t_vect *b)
{
	return (a->dx * b->dx + a->dy * b->dy + a->dz * b->dz);
}

t_vect	ft_vect_cross_product(t_vect *u, t_vect *v)
{
	t_vect result;

	result.dx = u->dy * v->dz - u->dz * v->dy;
	result.dy = u->dz * v->dx - u->dx * v->dz;
	result.dz = u->dx * v->dy - u->dy * v->dx;

	return (result);
}

///////////////////////////////////////////////////////////////////////////////]
double	dist_two_points(t_coor *a, t_coor *b)
{
	t_vect c = (t_vect){b->x - a->x, b->y - a->y, b->z - a->z};
	return (sqrt(c.dx * c.dx + c.dy * c.dy + c.dz * c.dz));
}

///////////////////////////////////////////////////////////////////////////////]
// fills rtrnn with the ccoordonates of the rotated vector camera for [x,y]pixel
// Q rotation combined = qβ.qα
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn)
{
	t_camera *c = data->eye.c;
	
	double cosA = cos(angle_α / 2);
	double sinA = sin(angle_α / 2);
	double cosB = cos(-angle_β / 2);
	double sinB = sin(-angle_β / 2);

	double Qw = cosA*cosB - sinA*sinB * (c->right.dx * c->up.dx + c->right.dy * c->up.dy + c->right.dz * c->up.dz);
	double Qi = cosB*sinA * c->up.dx + cosA*sinB * c->right.dx + sinA*sinB*(c->right.dy * c->up.dz - c->right.dz * c->up.dy);
	double Qj = cosB*sinA * c->up.dy + cosA*sinB * c->right.dy + sinA*sinB*(c->right.dz * c->up.dx - c->right.dx * c->up.dz);
	double Qk = cosB*sinA * c->up.dz + cosA*sinB * c->right.dz + sinA*sinB*(c->right.dx * c->up.dy - c->right.dy * c->up.dx);

	rtrn->dx = c->view.dx * (Qw * Qw + Qi * Qi - Qj * Qj - Qk * Qk) + 2*c->view.dy * (Qi*Qj - Qw*Qk) + 2*c->view.dz * (Qi*Qk + Qw*Qj);
	rtrn->dy = c->view.dy * (Qw * Qw + Qj * Qj - Qi * Qi - Qk * Qk) + 2*c->view.dz * (Qj*Qk - Qw*Qi) + 2*c->view.dx * (Qi*Qj + Qw*Qk);
	rtrn->dz = c->view.dz * (Qw * Qw + Qk * Qk - Qi * Qi - Qj * Qj) + 2*c->view.dx * (Qi*Qk - Qw*Qj) + 2*c->view.dy * (Qj*Qk + Qw*Qi);

	ft_normalize_vect(rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// 	return smalest positive
double h_smalest_Δ(double a, double b)
{
	if (a < EPSILON && b < EPSILON)
		return (-1.0);
	if (a < EPSILON)
		return (b);
	if (b < EPSILON)
		return (a);
	return (a * (a < b) + b * (b < a));
}
