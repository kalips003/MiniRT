/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Key_functions_tools.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/04 19:54:22 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void		render_normal(t_data *data, t_c_px *calcul);
void		h_render_v_space(t_data *data, t_obj *obj);
void		render_temp_added_obj(t_data *data, t_obj2 *obj_to_add);
static int	h_set_arrow(t_arrow *arrow, t_coor o, t_vect v, t_rgb c);

///////////////////////////////////////////////////////////////////////////////]
void	render_normal(t_data *data, t_c_px *calcul)
{
	t_arrow	arrow;

	h_set_arrow(&arrow, calcul->inter, calcul->vn, (t_rgb){255, 223, 0});
	arrow.h = 1.0;
	arrow.radius = 0.1;
	render_temp_added_obj(data, (t_obj2 *)&arrow);
}

///////////////////////////////////////////////////////////////////////////////]
void	h_render_v_space(t_data *data, t_obj *obj)
{
	t_arrow	arrow_x;
	t_arrow	arrow_y;
	t_arrow	arrow_z;
	int		i;

	if (h_set_arrow(&arrow_x, obj->c0, obj->right, (t_rgb){255, 0, 0}) || \
		h_set_arrow(&arrow_y, obj->c0, obj->up, (t_rgb){0, 255, 0}) || \
		h_set_arrow(&arrow_z, obj->c0, obj->view, (t_rgb){0, 0, 255}))
		return (printf(ERR4"obj?: %p\n", obj), (void)0);
	data->objects = expand_tab(data->objects, &arrow_x);
	data->objects = expand_tab(data->objects, &arrow_y);
	data->objects = expand_tab(data->objects, &arrow_z);
	i = -1;
	while (data->objects[++i])
		if (data->objects[i] == &arrow_x)
			break ;
	if (!data->objects[i])
		return (printf(ERR7"??\n"), (void)0);
	ft_render_frame_multi(data, RENDERING_LVL);
	data->objects[i] = NULL;
	data->objects[i + 1] = NULL;
	data->objects[i + 2] = NULL;
}

///////////////////////////////////////////////////////////////////////////////]
void	render_temp_added_obj(t_data *data, t_obj2 *obj_to_add)
{
	int	i;

	data->objects = expand_tab(data->objects, obj_to_add);
	i = -1;
	while (data->objects[++i])
		if (data->objects[i] == obj_to_add)
			break ;
	if (!data->objects[i])
		return (printf(ERR7"??\n"), (void)0);
	ft_render_frame_multi(data, RENDERING_LVL);
	data->objects[i] = NULL;
}

static int	h_set_arrow(t_arrow *arrow, t_coor o, t_vect v, t_rgb c)
{
	ft_memset(arrow, 0, sizeof(t_arrow));
	arrow->h = 3.0;
	arrow->radius = 0.3;
	arrow->type = ARROW;
	arrow->O.view = v;
	arrow->O.c0 = o;
	arrow->param.argb = (t_argb){0, c.r, c.g, c.b};
	arrow->param.light = 1.0;
	arrow->param.c2.r = -1;
	if (h_parse_vect_space(&arrow->O, &arrow->O.view))
		return (1);
	recalculate_obj_const((t_obj2 *)arrow);
	return (0);
}
