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
// void	f_rotation_mouse_slide_the_buffer(t_calcul_px *calcul, )


// c->v = calc_angle_view(data->eye.c, x - SIZE_SCREEN_X / 2, y - SIZE_SCREEN_Y / 2);
// x between [-screenX/2, screenX/2] 
t_vect	calc_angle_view(t_camera *cam, int x, int y)
{
	t_vect	rtrn;
	double	cx;
	double	cy;

	cx = x * cam->fov_cst;
	cy = -y * cam->fov_cst;
	rtrn = cam->O.view;
	rtrn = (t_vect){
		rtrn.dx + cx * cam->O.right.dx + cy * cam->O.up.dx,
		rtrn.dy + cx * cam->O.right.dy + cy * cam->O.up.dy,
		rtrn.dz + cx * cam->O.right.dz + cy * cam->O.up.dz,
	};
	ft_normalize_vect(&rtrn);
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	wrap_v_cam_quaternion(t_data *data, int x, int y)
{
	double		angle_a;
	double		angle_b;

	angle_b = atan((y - SZ_Y / 2) * data->eye.c->fov_cst);
	angle_a = atan((x - SZ_X / 2) * data->eye.c->fov_cst);
	return (combined_quaternion_rotation(&data->eye.c->O, angle_a, angle_b));
}

///////////////////////////////////////////////////////////////////////////////]
t_img	*parse_img(t_data *data, char *path)
{
	t_img	*texture;

	if (access(path, F_OK))
		return (printf(C_421"I cant open that: (%s)\n", path), NULL);
	if (txt_already_exist(data, path, &texture))
		return (texture);
	texture = mem(0, sizeof(t_img));
	data->textures = (t_img **)expand_tab((void **)data->textures, texture);
	if (!texture || !data->textures)
		return (put(ERRM"parse_texture\n"), NULL);
	if (path)
		texture->img = mlx_xpm_file_to_image(data->mlx, path, &texture->sz_x, &texture->sz_y);
	if (!texture->img)
		return (put(ERR8"Cant open sprite: %s\n", path), perror(RED"mlx_xpm_file_to_image"), NULL);
	texture->addr = mlx_get_data_addr(texture->img, &texture->bpp, &texture->ll, &texture->end);
	texture->path = str("%1s", path);
	return (texture);
}