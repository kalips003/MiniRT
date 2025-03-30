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
double	calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
t_vect	ft_vect_reflected(t_vect *incident, t_vect *normal);
t_vect	ft_vect_refracted(t_vect *incident, t_vect *normal, double n, int *neg);

///////////////////////////////////////////////////////////////////////////////]
// Recursively call calculate_px_color, moving the ray forward
t_argb	what_is_behind(t_data *data, t_c_px *calcul)
{
	t_c_px	c;
	double	incident_gamma;
	double	refracted_gamma;
	int		neg;

	neg = 1;
	if (calcul->transparence_depth > MAX_REFRACTION_DEPTH)
		return (printf(ERR"transparence depth: %d [max:%d]\n", calcul->transparence_depth, MAX_REFRACTION_DEPTH), calcul->mat.argb);
	ini_new_calcul_struct(calcul, &c, 0b1);
	if (calcul->object->type == PLANE
		|| calcul->object->type == SPRITE
		|| calcul->object->type == PARABOLOID
		|| !rtrn_top_stack_gamma(&c, calcul->object, &incident_gamma, &refracted_gamma))
		c.v = calcul->v;
	else
		c.v = ft_vect_refracted(&calcul->v, &calcul->vn, incident_gamma / refracted_gamma, &neg);
	c.c0 = new_moved_point(&calcul->inter, &calcul->v, neg * EPSILON);
	if (neg == -1)
		push_stack(calcul->inside, calcul->object, &calcul->stack_top, MAX_MIRROR_DEPTH - 1);
	calc_px_color(data, &c, RENDER_LVL_DEPTH);
	return (c.mat.argb);
}

///////////////////////////////////////////////////////////////////////////////]
// Recursively call calculate_px_color, moving the ray forward
t_argb	what_is_reflected(t_data *data, t_c_px *calcul)
{
	t_c_px	c;

	if (calcul->reflected_depth > MAX_MIRROR_DEPTH)
		return (printf(ERR"mirror depth: %d [max:%d]\n", calcul->reflected_depth, MAX_REFRACTION_DEPTH), calcul->mat.argb);
	ini_new_calcul_struct(calcul, &c, 0b10);
	c.v = ft_vect_reflected(&calcul->v, &calcul->vn);
	c.c0 = new_moved_point(&calcul->inter, &calcul->vn, EPSILON);
	calc_px_color(data, &c, RENDER_LVL_DEPTH);
	return (c.mat.argb);
}

///////////////////////////////////////////////////////////////////////////////]
// calculate angle between camera ray and light source at intersection
double	calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal)
{
	t_vect	l;
	double	cos_theta;

	l = vect_ab_norm(intersection, light);
	cos_theta = ft_dot_product(&l, normal);
	return (cos_theta);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	ft_vect_reflected(t_vect *incident, t_vect *normal)
{
	t_vect	reflected;
	double	dot_pro;

	dot_pro = 2.0 * ft_dot_product(incident, normal);
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

	dot = ft_dot_product(incident, normal);
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
