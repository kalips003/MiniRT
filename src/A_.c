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
	if (obj->param.height_map)
	{
		double	height;
		height = return_alpha_img(obj->param.alpha_map, u, v) / 255.0 * HEIGHT_MAP_DISPLACEMENT;
		calcul->inter = new_moved_point(&calcul->inter, &calcul->vn, height);
	}
	if (obj->param.alpha_map)
		calcul->mat.argb.a = return_alpha_img(obj->param.alpha_map, u, v);
	if (obj->param.texture)
		calcul->mat.argb = return_px_img(obj->param.texture, u, v);
	if (obj->param.ao_map)
		calcul->ao = return_alpha_img(obj->param.alpha_map, u, v) / 255.0;
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