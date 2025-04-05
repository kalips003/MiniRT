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

void	f_progressive_rt(t_data *data, t_obj2 *obj, int k_or_loop);

///////////////////////////////////////////////////////////////////////////////]
// progressive RT
void	f_progressive_rt(t_data *data, t_obj2 *obj, int k_or_loop)
{
	(void)obj;
	if (k_or_loop != 1)
	{
		if (!k_or_loop)
			printf(C_451"Starting Progressive RT (Press Space)\n");
		clean_buffer(data);
		*(int *)&data->ram = 0;
		return ;
	}
	ft_render_frame_multi_prog(data, RENDERING_LVL);
	(*(int *)&data->ram)++;
	printf("loop: %d\n", *(int *)&data->ram);
}
