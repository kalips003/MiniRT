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

int			rtrn_top_stack_gamma(t_calcul_px *calcul, t_obj2 *collision, double *incident_gamma, double *refracted_gamma);
static int	where_in_stack(void **stack, void *colli, int ptr_stack);
void		push_stack(void **stack, void *colli, int *ptr_stack, int max_stack_index);
static int	pop_stack(void **stack, int to_pop, int *ptr_stack);

///////////////////////////////////////////////////////////////////////////////]
// if the new collision stored in the previous calcul_struct:
// 		is NOT in the stack, push it
// 			if stack full, no need calcul, just return color obj
// 		is top of stack: (prev_gamma = topstack, new_gamma = topstack[-1] || AIR), pop obj
// 		is NOT top of stack: pop obj, ignore refraction
// 		is a plane or sprite, ignore refraction
// 	Return 0 if popped element isnt top of stack
int	rtrn_top_stack_gamma(t_calcul_px *calcul, t_obj2 *collision, double *incident_gamma, double *refracted_gamma)
{
	int	posi_colli;

	*refracted_gamma = collision->param.gamma;
	if (calcul->stack_top == -1)
	{
		*incident_gamma = 1.0;
		push_stack(calcul->inside, collision, &calcul->stack_top, MAX_MIRROR_DEPTH - 1);
		return (1);
	}
	else
		*incident_gamma = ((t_obj2 *)calcul->inside[calcul->stack_top])->param.gamma;
	posi_colli = where_in_stack(calcul->inside, collision, calcul->stack_top);
	if (posi_colli == -1)
		push_stack(calcul->inside, collision, &calcul->stack_top, MAX_MIRROR_DEPTH - 1);
	else if (pop_stack(calcul->inside, posi_colli, &calcul->stack_top))
	{
		if (calcul->stack_top == -1)
			*refracted_gamma = 1.0;
		else
			*refracted_gamma = ((t_obj2 *)calcul->inside[calcul->stack_top])->param.gamma;
	}
	else
		return (0);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
static int	where_in_stack(void **stack, void *colli, int ptr_stack)
{
	int	i;

	i = 0;
	while (stack && i <= ptr_stack)
	{
		if (colli == stack[i])
			return (i);
		i++;
	}
	return (-1);
}

void	push_stack(void **stack, void *colli, int *ptr_stack, int max_stack_index)
{
	int	i;

	if (*ptr_stack == max_stack_index)
	{
		i = -1;
		while (++i < max_stack_index)
			stack[i] = stack[i + 1];
		stack[i] = colli;
	}
	else
		stack[++*ptr_stack] = colli;
}

// if the popped element is the top of stack return 1, else 0
static int	pop_stack(void **stack, int to_pop, int *ptr_stack)
{
	int	i;

	i = to_pop - 1;
	while (++i < *ptr_stack)
		stack[i] = stack[i + 1];
	return (to_pop == (*ptr_stack)--);
}
