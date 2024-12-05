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


///////////////////////////////////////////////////////////////////////////////]
// Normalize vector
int	ft_normalize_vect(t_vect *vect)
{
	double normal_magnitude;

	normal_magnitude = sqrt(vect->dx * vect->dx + vect->dy * vect->dy + vect->dz * vect->dz);
	if (normal_magnitude == 0.0)
		return (1);
	vect->dx = vect->dx / normal_magnitude;
	vect->dy = vect->dy / normal_magnitude;
	vect->dz = vect->dz / normal_magnitude;
	return (0);
}


double	ft_vect_dot_product(t_vect *a, t_vect *b)
{
	return (a->dx * b->dx + a->dy * b->dy + a->dz * b->dz);
}

