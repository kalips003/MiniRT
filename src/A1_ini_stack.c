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

void		ini_stack(t_data *data, t_c_px *calcul);
t_ini_stk	*add_link(t_ini_stk *first, t_ini_stk *to_add);
t_ini_stk	*create_node(t_c_px *calcul);
static void	destroy_list(t_ini_stk *first);

///////////////////////////////////////////////////////////////////////////////]
void	ini_stack(t_data *data, t_c_px *calcul)
{
	t_c_px		c;
	t_ini_stk	*top_list;
	t_ini_stk	*ptr_list;

	c.c0 = data->eye.c->O.c0;
	c.v = data->eye.c->O.view;
	top_list = NULL;
	ptr_list = NULL;
	ft_fill_stack_inside(data, &c, &top_list, &ptr_list);
	ptr_list = top_list;
	calcul->stack_top = -1;
	while (ptr_list)
	{
		push_stack(calcul->inside, ptr_list->obj, &calcul->stack_top, \
			MAX_MI_DEPTH - 1);
		ptr_list = ptr_list->next;
	}
	destroy_list(top_list);
}

///////////////////////////////////////////////////////////////////////////////]
// add new dist/obj node to the list, closest distance last
t_ini_stk	*add_link(t_ini_stk *first, t_ini_stk *to_add)
{
	t_ini_stk	*ptr;
	t_ini_stk	*ptr_prev;

	if (!first)
		return (to_add);
	ptr = first;
	ptr_prev = NULL;
	while (ptr)
	{
		if (to_add->dist > ptr->dist)
		{
			to_add->next = ptr;
			if (ptr_prev)
				ptr_prev->next = to_add;
			else
				first = to_add;
			return (first);
		}
		ptr_prev = ptr;
		ptr = ptr->next;
	}
	ptr_prev->next = to_add;
	return (first);
}

t_ini_stk	*create_node(t_c_px *calcul)
{
	t_ini_stk	*next;

	next = mem(0, sizeof(t_ini_stk));
	if (!next)
		return (NULL);
	next->dist = calcul->dist;
	next->obj = calcul->object;
	return (next);
}

static void	destroy_list(t_ini_stk *first)
{
	t_ini_stk	*ptr;

	ptr = first;
	while (ptr)
	{
		first = ptr->next;
		free_s(ptr);
		ptr = first;
	}
}
