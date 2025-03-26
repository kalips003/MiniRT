/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TT_tools_math2_vect_mouv.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/02/09 17:44:35 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	move_point(t_coor *p, t_vect *v, double incre);
t_coor	new_moved_point(t_coor *p, t_vect *v, double dist);
t_vect	vect_ab(t_coor *a, t_coor *b);
t_vect	vect_ab_norm(t_coor *a, t_coor *b);

///////////////////////////////////////////////////////////////////////////////]
void	move_point(t_coor *p, t_vect *v, double incre)
{
	p->x += v->dx * incre;
	p->y += v->dy * incre;
	p->z += v->dz * incre;
}

///////////////////////////////////////////////////////////////////////////////]
t_coor	new_moved_point(t_coor *p, t_vect *v, double dist)
{
	t_coor	point;

	point.x = p->x + v->dx * dist;
	point.y = p->y + v->dy * dist;
	point.z = p->z + v->dz * dist;
	return (point);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	vect_ab(t_coor *a, t_coor *b)
{
	t_vect	vect;

	vect.dx = b->x - a->x;
	vect.dy = b->y - a->y;
	vect.dz = b->z - a->z;
	return (vect);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	vect_ab_norm(t_coor *a, t_coor *b)
{
	t_vect	vect;

	vect.dx = b->x - a->x;
	vect.dy = b->y - a->y;
	vect.dz = b->z - a->z;
	ft_normalize_vect(&vect);
	return (vect);
}
