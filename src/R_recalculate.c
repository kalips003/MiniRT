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

void	recalculate_obj_const(t_obj2 *obj);

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
void	recalculate_obj_const(t_obj2 *obj)
{
	if (!obj)
		return ;
	if (obj->type == CONE)
	{
		((t_cone *)obj)->apex = new_moved_point(&obj->O.c0, &obj->O.view, ((t_cone *)obj)->height);
		((t_cone *)obj)->slope = pow(((t_cone *)obj)->radius, 2.0) / pow(((t_cone *)obj)->height, 2.0);
		((t_cone *)obj)->angle = atan(((t_cone *)obj)->radius / ((t_cone *)obj)->height);
	}
	else if (obj->type == CYLINDER)
		((t_cylinder *)obj)->xyz_other = new_moved_point(&obj->O.c0, &obj->O.view, ((t_cylinder *)obj)->height);
	else if (obj->type == PLANE || obj->type == SPRITE)
		((t_plane *)obj)->d = -(obj->O.view.dx * obj->O.c0.x + obj->O.view.dy * obj->O.c0.y + obj->O.view.dz * obj->O.c0.z);
	else if (obj->type == ARROW)
	{
		((t_arrow *)obj)->xyz_other = new_moved_point(&obj->O.c0, &obj->O.view, ((t_arrow *)obj)->height * 2 / 3);
		((t_arrow *)obj)->apex = new_moved_point(&obj->O.c0, &obj->O.view, ((t_arrow *)obj)->height);
		((t_arrow *)obj)->slope = (9.0 * pow(((t_arrow *)obj)->radius, 2.0)) / pow(((t_arrow *)obj)->height, 2.0);
	}
}
