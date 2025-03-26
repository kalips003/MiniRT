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

t_coor	*h_v(char **spl, double size);
t_vect	*h_vn(char **spl);
t_vt	*h_vt(char **spl);

///////////////////////////////////////////////////////////////////////////////]
// v->x = atof(spl[1]);
// v->y = atof(spl[2]);
// v->z = atof(spl[3]);
t_coor	*h_v(char **spl, double size)
{
	t_coor	*v;

	v = mem(0, sizeof(t_coor));
	if (!v)
		return (put(ERRM"ft: h_v\n"), NULL);
	v->x = atof(spl[1]);
	v->y = atof(spl[2]);
	v->z = atof(spl[3]);
	// if (ft_atof(spl[1], &v->x) || ft_atof(spl[2], &v->y) || ft_atof(spl[3], &v->z))
	// 	return (put(ERR5"v ???\n"), NULL);
	v->x *= size;
	v->y *= size;
	v->z *= size;
	return (v);
}

///////////////////////////////////////////////////////////////////////////////]
// vn->dx = atof(spl[1]);
// vn->dy = atof(spl[2]);
// vn->dz = atof(spl[3]);
t_vect	*h_vn(char **spl)
{
	t_vect	*vn;

	vn = mem(0, sizeof(t_vect));
	if (!vn)
		return (put(ERRM"ft: h_vn\n"), NULL);
	vn->dx = atof(spl[1]);
	vn->dy = atof(spl[2]);
	vn->dz = atof(spl[3]);
	// if (ft_atof(spl[1], &vn->dx) || ft_atof(spl[2], &vn->dy) || ft_atof(spl[3], &vn->dz))
	// 	return (put(ERR5"vn ???\n"), NULL);
	ft_normalize_vect(vn);
	return (vn);
}

t_vt	*h_vt(char **spl)
{
	t_vt	*vt;

	vt = mem(0, sizeof(t_vt));
	if (!vt)
		return (put(ERRM"ft: h_vt\n"), NULL);
	if (ft_atof(spl[1], &vt->u) || ft_atof(spl[2], &vt->v))
		return (put(ERR5"vt ???\n"), NULL);
	vt->u = fmin(1.0, fmax(0.0, vt->u));
	vt->v = fmin(1.0, fmax(0.0, vt->v));
	return (vt);
}
