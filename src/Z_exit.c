/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Z_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2024/10/30 13:28:24 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	end(t_data *data, int exit_code);

///////////////////////////////////////////////////////////////////////////////]
void	end(t_data *data, int exit_code)
{
	
	exit(exit_code);
}

void	free_list(void **first_node)
{
	if (!first_node)
		return (NULL);
	void	*next;
	next = *first_node;
	

}
