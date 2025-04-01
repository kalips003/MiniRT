/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   C_find_colision.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 10:25:32 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			find_coli(t_data *data, t_c_px *c, int shadow, int set_dist);
t_ini_stk	*ft_fill_stack_inside(t_data *data, t_c_px *c, t_ini_stk **top_list, t_ini_stk **ptr_list);
int			something_block_the_light(t_data *data, t_c_px *c);

///////////////////////////////////////////////////////////////////////////////]

static const t_dist_of	g_ft_dist_of[] = {
	distance_from_circle,
	distance_from_sphere,
	distance_from_plane,
	distance_from_cylinder,
	distance_from_cone,
	distance_from_hyper,
	distance_from_para,
	distance_from_arrow,
	distance_from_sprite,
	distance_from_object,
	NULL
};
///////////////////////////////////////////////////////////////////////////////]

///////////////////////////////////////////////////////////////////////////////]
// 	find the closest object colision from vector view
//	fills calcul data struct with:
// 		position xyz of the colision (default 0,0,0)
// 		vector normal to the surface ...
// return 1 if there is collision
// 
// if SHADOW: 1, dont fill calcul, return when anything is hit
// if SHADOW: 0, fill calcul, return when all object have been cycled through
// if SET_DIST: 1, dist is set to -1.0
// if SET_DIST: 0, dist is unchanged
int	find_coli(t_data *data, t_c_px *c, int shadow, int set_dist)
{
	void	**obj_ptr;
	int		rtrn;

	if (set_dist)
		c->dist = -1.0;
	rtrn = 0;
	obj_ptr = data->objects - 1;
	while (++obj_ptr && *obj_ptr)
	{
		rtrn |= g_ft_dist_of[((t_obj2 *)*obj_ptr)->type](c, *obj_ptr, shadow);
		if (rtrn && shadow)
			return (1);
	}
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// used to create a linked list of object the camera is INSIDE of
// created in order, furthest is first, closest last of the chain
t_ini_stk	*ft_fill_stack_inside(t_data *data, t_c_px *c, t_ini_stk **top_list, t_ini_stk **ptr_list)
{
	void	**obj_ptr;
	int		in;

	obj_ptr = data->objects - 1;
	while (++obj_ptr && *obj_ptr)
	{
		c->dist = -1.0;
		in = g_ft_dist_of[((t_obj2 *)*obj_ptr)->type](c, *obj_ptr, 0);
		if (in == 2)
		{
			*ptr_list = create_node(c);
			*top_list = add_link(*top_list, *ptr_list);
		}
	}
	return (*top_list);
}

///////////////////////////////////////////////////////////////////////////////]
int	something_block_the_light(t_data *data, t_c_px *c)
{
	t_c_px	calcul;
	void	**obj_ptr;
	double	tr;

	calcul.c0 = new_moved_point(&c->inter, &c->vn, EPSILON);
	calcul.v = c->v_light;
	calcul.dist = c->dist_light;
	calcul.print = c->print + !!(c->print);
	obj_ptr = data->objects - 1;
	while (++obj_ptr && *obj_ptr)
	{
		tr = ((t_obj2 *)*obj_ptr)->param.transparence;
		if (g_ft_dist_of[((t_obj2 *)*obj_ptr)->type](&calcul, *obj_ptr, 1))
		{
			if (tr < EPSILON)
				return (1);
			tr = 1.0 - tr;
			c->eff_l.ratio *= tr;
			c->eff_l.rgb.r *= ((t_obj2 *)*obj_ptr)->param.argb.r / 255.0 * tr;
			c->eff_l.rgb.g *= ((t_obj2 *)*obj_ptr)->param.argb.g / 255.0 * tr;
			c->eff_l.rgb.b *= ((t_obj2 *)*obj_ptr)->param.argb.b / 255.0 * tr;
		}
	}
	return (0);
}
