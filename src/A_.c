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

///////////////////////////////////////////////////////////////////////////////]
void	update_mat_w_txt(t_c_px *calcul, t_obj2 *obj, double u, double v)
{
	// if (obj->param.shiny_map)
	// 	calcul->mat.sh = return_alpha_img(obj->param.alpha_map, u, v) / 255.0;
	if (obj->param.height_map)
	{
		double	height;
		height = return_alpha_img(obj->param.height_map, u, v) / 255.0 * HEIGHT_MAP_DISPLACEMENT;
		calcul->inter = new_moved_point(&calcul->inter, &calcul->vn, height);
	}
	if (obj->param.txt)
		calcul->mat.argb = return_px_img(obj->param.txt, u, v);
	if (obj->param.a_map)
		calcul->mat.argb.a = return_alpha_img(obj->param.a_map, u, v);
	if (obj->param.ao_map)
		calcul->ao = return_alpha_img(obj->param.ao_map, u, v) / 255.0;
}

///////////////////////////////////////////////////////////////////////////////]
t_img	*parse_img(t_data *data, char *path)
{
	t_img	*txt;

	if (access(path, F_OK))
		return (printf(C_421"I cant open that: (%s)\n", path), NULL);
	if (txt_already_exist(data, path, &txt))
		return (txt);
	txt = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, txt);
	if (!txt || !data->textures)
		return (put(ERRM"parse_texture\n"), NULL);
	if (path)
		txt->img = mlx_xpm_file_to_image(data->mlx, path, \
			&txt->sz_x, &txt->sz_y);
	if (!txt->img)
		return (put(ERR8"Cant open sprite: %s\n", path), perror(RED"mlx"), NULL);
	txt->addr = mlx_get_data_addr(txt->img, &txt->bpp, &txt->ll, &txt->end);
	txt->path = str("%1s", path);
	return (txt);
}

int	txt_already_exist(t_data *data, char *path, t_img **txt)
{
	int	i;

	i = -1;
	while (data->textures && data->textures[++i])
	{
		if (same_str(data->textures[i]->path, path))
		{
			*txt = data->textures[i];
			return (1);
		}
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	mult_3x3_vect(t_obj *o3, t_vect *v)
{
	t_vect	rtrn;

	rtrn = (t_vect){
		v->dx * o3->right.dx + v->dy * o3->up.dx + v->dz * o3->view.dx,
		v->dx * o3->right.dy + v->dy * o3->up.dy + v->dz * o3->view.dy,
		v->dx * o3->right.dz + v->dy * o3->up.dz + v->dz * o3->view.dz
	};
	return (rtrn);
}

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
	// data->objects[i] = free_s(obj_to_add);
}

void	render_normal(t_data *data, t_c_px *calcul)
{
	t_arrow arrow;

	ft_memset(&arrow, 0, sizeof(t_arrow));
	arrow.height = 1.0;
	arrow.radius = 0.1;
	arrow.param.argb = (t_argb){0, 255, 223, 0};
	arrow.type = ARROW;
	arrow.O.c0 = calcul->inter;
	arrow.O.view = calcul->vn;
	h_parse_vect_space(&arrow.O, &arrow.O.view);
	recalculate_obj_const(&arrow);
	arrow.param.light = 1.0;
	arrow.param.c2.r = -1;
	render_temp_added_obj(data, &arrow);
}