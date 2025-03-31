/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E2_refrac_reflec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/11 15:06:41 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_argb	what_is_behind(t_data *data, t_c_px *calcul);
t_argb	what_is_reflected(t_data *data, t_c_px *calcul);
double	calculate_light_angle(t_coor *inter, t_coor *light, t_vect *normal);
t_vect	ft_vect_reflected(t_vect *incident, t_vect *normal);
t_vect	ft_vect_refracted(t_vect *incident, t_vect *normal, double n, int *neg);

///////////////////////////////////////////////////////////////////////////////]
// Recursively call calculate_px_color, moving the ray forward
t_argb	what_is_behind(t_data *data, t_c_px *ca)
{
	t_c_px	c;
	double	i_gamma;
	double	r_gamma;
	int		neg;

	neg = 1;
	if (ca->transparence_depth > MAX_TR_DEPTH)
		return (printf(ERR"transparence depth: %d [max:%d]\n", \
			ca->transparence_depth, MAX_TR_DEPTH), ca->mat.argb);
	ini_new_calcul_struct(ca, &c, 0b1);
	if (ca->object->type == PLANE
		|| ca->object->type == SPRITE
		|| ca->object->type == PARABOLOID
		|| !rtrn_top_stack_gamma(&c, ca->object, &i_gamma, &r_gamma))
		c.v = ca->v;
	else
		c.v = ft_vect_refracted(&ca->v, &ca->vn, i_gamma / r_gamma, &neg);
	c.c0 = new_moved_point(&ca->inter, &ca->v, neg * EPSILON);
	if (neg == -1)
		push_stack(ca->inside, ca->object, &ca->stack_top, MAX_MI_DEPTH - 1);
	calc_px_color(data, &c, RENDER_LVL_DEPTH);
	return (c.mat.argb);
}

///////////////////////////////////////////////////////////////////////////////]
// Recursively call calculate_px_color, moving the ray forward
t_argb	what_is_reflected(t_data *data, t_c_px *calcul)
{
	t_c_px	c;

	if (calcul->reflected_depth > MAX_MI_DEPTH)
		return (printf(ERR"mirror depth: %d [max:%d]\n", \
			calcul->reflected_depth, MAX_TR_DEPTH), calcul->mat.argb);
	ini_new_calcul_struct(calcul, &c, 0b10);
	c.v = ft_vect_reflected(&calcul->v, &calcul->vn);
	c.c0 = new_moved_point(&calcul->inter, &calcul->vn, EPSILON);
	calc_px_color(data, &c, RENDER_LVL_DEPTH);
	return (c.mat.argb);
}

///////////////////////////////////////////////////////////////////////////////]
// calculate angle between camera ray and light source at intersection
double	calculate_light_angle(t_coor *inter, t_coor *light, t_vect *normal)
{
	t_vect	l;
	double	cos_theta;

	l = vect_ab_norm(inter, light);
	cos_theta = ft_dot_p(&l, normal);
	return (cos_theta);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	ft_vect_reflected(t_vect *incident, t_vect *normal)
{
	t_vect	reflected;
	double	dot_pro;

	dot_pro = 2.0 * ft_dot_p(incident, normal);
	reflected.dx = incident->dx - dot_pro * normal->dx;
	reflected.dy = incident->dy - dot_pro * normal->dy;
	reflected.dz = incident->dz - dot_pro * normal->dz;
	ft_normalize_vect(&reflected);
	return (reflected);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	ft_vect_refracted(t_vect *incident, t_vect *normal, double n, int *neg)
{
	t_vect	refracted;
	double	dot;
	double	sin_theta2;
	double	s;

	dot = ft_dot_p(incident, normal);
	if (fabs(dot + 1.0) < EPSILON)
		return (*incident);
	sin_theta2 = n * sqrt(1 - dot * dot);
	if (sin_theta2 > 1.0)
	{
		*neg = -1;
		return (ft_vect_reflected(incident, normal));
	}
	s = -sqrt(1 - sin_theta2 * sin_theta2);
	refracted = (t_vect){
		n * (incident->dx - dot * normal->dx) + s * normal->dx,
		n * (incident->dy - dot * normal->dy) + s * normal->dy,
		n * (incident->dz - dot * normal->dz) + s * normal->dz
	};
	return (refracted);
}
