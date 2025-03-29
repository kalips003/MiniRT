/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D6_dist_arrow.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/11 10:54:59 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		distance_from_arrow(t_c_px *calcul, void *obj, int simple);

///////////////////////////////////////////////////////////////////////////////]
int	distance_from_arrow(t_c_px *calcul, void *obj, int simple)
{
	t_arrow_calc	c;
	t_arrow			*arrow;
	int				rtrn;

	arrow = (t_arrow *)obj;
	c.cy = (t_cylinder){CYLINDER, arrow->O, arrow->param, arrow->xyz_other, arrow->height * 2 / 3, arrow->radius / 2};
	c.co = (t_cone){CONE, arrow->O, arrow->param, arrow->height / 3, arrow->radius, arrow->apex, arrow->slope, 0};
	c.co.angle = atan(c.co.radius / c.co.height);
	c.co.O.c0 = new_moved_point(&c.co.O.c0, &arrow->O.view, arrow->height * 2 / 3);
	rtrn = distance_from_cylinder(calcul, &c.cy, simple);
	rtrn |= distance_from_cone(calcul, &c.co, simple);
	if (rtrn && simple)
		return (1);
	if (rtrn)
		calcul->object = obj;
	return (rtrn);
}
