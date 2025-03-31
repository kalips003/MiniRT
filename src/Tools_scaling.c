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

t_vect	scale_vect(t_vect v, double scalar);
t_coor	scale_point(t_coor p, double scalar);
t_argb	scale_argb(t_argb argb, double scalar);

///////////////////////////////////////////////////////////////////////////////]
t_vect	scale_vect(t_vect v, double scalar)
{
	v.dx = v.dx * scalar;
	v.dy = v.dy * scalar;
	v.dz = v.dz * scalar;
	return (v);
}

///////////////////////////////////////////////////////////////////////////////]
t_coor	scale_point(t_coor p, double scalar)
{
	p.x = p.x * scalar;
	p.y = p.y * scalar;
	p.z = p.z * scalar;
	return (p);
}

///////////////////////////////////////////////////////////////////////////////]
t_argb	scale_argb(t_argb argb, double scalar)
{
	argb.r = fmax(0, fmin(255, round(argb.r * scalar)));
	argb.g = fmax(0, fmin(255, round(argb.g * scalar)));
	argb.b = fmax(0, fmin(255, round(argb.b * scalar)));
	return (argb);
}
