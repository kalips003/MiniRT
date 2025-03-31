/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   T_tools_vect_ope.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/26 11:14:35 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		ft_normalize_vect(t_vect *vect);
double	ft_dot_p(t_vect *a, t_vect *b);
t_vect	ft_cross_product(t_vect *u, t_vect *v);
t_vect	ft_cross_product_norm(t_vect *u, t_vect *v);
double	dist_two_points(t_coor *a, t_coor *b);

///////////////////////////////////////////////////////////////////////////////]
// return (printf("VECTOR = [%.2f,%.2f,%.2f]\n",vect->dx,vect->dy,vect->dz),1);
// Normalize vector
int	ft_normalize_vect(t_vect *vect)
{
	double	normal_magnitude;

	normal_magnitude = sqrt(vect->dx * vect->dx + vect->dy * vect->dy + \
		vect->dz * vect->dz);
	if (normal_magnitude == 0.0)
		return (1);
	vect->dx = vect->dx / normal_magnitude;
	vect->dy = vect->dy / normal_magnitude;
	vect->dz = vect->dz / normal_magnitude;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
double	ft_dot_p(t_vect *a, t_vect *b)
{
	return (a->dx * b->dx + a->dy * b->dy + a->dz * b->dz);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	ft_cross_product(t_vect *u, t_vect *v)
{
	t_vect	result;

	result.dx = u->dy * v->dz - u->dz * v->dy;
	result.dy = u->dz * v->dx - u->dx * v->dz;
	result.dz = u->dx * v->dy - u->dy * v->dx;
	return (result);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	ft_cross_product_norm(t_vect *u, t_vect *v)
{
	t_vect	result;

	result.dx = u->dy * v->dz - u->dz * v->dy;
	result.dy = u->dz * v->dx - u->dx * v->dz;
	result.dz = u->dx * v->dy - u->dy * v->dx;
	ft_normalize_vect(&result);
	return (result);
}

///////////////////////////////////////////////////////////////////////////////]
double	dist_two_points(t_coor *a, t_coor *b)
{
	t_vect	c;

	c = (t_vect){b->x - a->x, b->y - a->y, b->z - a->z};
	return (sqrt(c.dx * c.dx + c.dy * c.dy + c.dz * c.dz));
}
