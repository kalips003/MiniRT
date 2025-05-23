/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_loop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 15:53:33 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int					ft_loop(t_data *data);
static void			h_refresh_input_file(t_data *data);
int					ft_render_frame(t_data *data, int sublim);
unsigned int		calc_px_color(t_data *data, t_c_px *c, int sublim);
static unsigned int	h_bg_texture(t_data *data, t_c_px *calcul);

///////////////////////////////////////////////////////////////////////////////]
// main loop refresh if file is changed
int	ft_loop(t_data *data)
{
	struct stat	file_stat;

	if (stat(data->av[1], &file_stat) == 0)
	{
		if (file_stat.st_mtime != data->last_modif_time)
		{
			printf(C_413"file changed... REFRESH\n");
			data->last_modif_time = file_stat.st_mtime;
			h_refresh_input_file(data);
			ft_render_frame_multi(data, RENDERING_LVL);
		}
	}
	else
		perror("stat");
	if (data->change_function && data->change)
		data->change_function(data, data->change_obj, 1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_refresh_input_file(t_data *data)
{
	t_camera	*old_camera;

	old_camera = mem(0, sizeof(t_camera));
	*old_camera = *data->eye.c;
	usleep(10000);
	end(data, 0, 0);
	data->camera = (t_camera **)expand_tab((void **)data->camera, old_camera);
	if (!data->camera)
		printf(ERRM"expand tab failed\n");
	read_file(2, data->av, data);
	if (!data->camera)
		printf(ERRM"read file failed\n");
	data->change_obj = NULL;
	data->current_camera = 0;
	data->eye.c = data->camera[0];
}

///////////////////////////////////////////////////////////////////////////////]
// render simple shadows, uses adjusted by tan angle
// used to render while moving
int	ft_render_frame(t_data *data, int sublim)
{
	t_c_px	c;
	int		x;
	int		y;

	ft_memset(&c, 0, sizeof(t_c_px));
	ini_stack(data, &c);
	c.c0 = data->eye.c->O.c0;
	y = -1;
	while (++y < SZ_Y)
	{
		x = -1;
		while (++x < SZ_X)
		{
			c.transparence_depth = 0;
			c.reflected_depth = 0;
			c.v = v_cam(data, x, y, NOT_AA);
			calc_px_color(data, &c, sublim);
			w_px_buff(&data->buffer, x, y, c.mat.argb.r << 16 \
				| c.mat.argb.g << 8 | c.mat.argb.b);
		}
	}
	mlx_put_image_to_window(data->mlx, data->win, data->buffer.img, 0, 0);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// require ray origin (c0), ray vector (v)
// fills calcul.argb with the pixel color shaded of the intersection
unsigned int	calc_px_color(t_data *data, t_c_px *c, int sublim)
{
	if (!find_coli(data, c, NOT_SHADOWS, SET_DIST))
	{
		if (data->bgl[0]->texture)
			return (h_bg_texture(data, c));
		c->mat.argb.r = (int)(round(data->bgl[0]->rgb.r * data->bgl[0]->ratio));
		c->mat.argb.g = (int)(round(data->bgl[0]->rgb.g * data->bgl[0]->ratio));
		c->mat.argb.b = (int)(round(data->bgl[0]->rgb.b * data->bgl[0]->ratio));
		return (c->mat.argb.r << 16 | c->mat.argb.g << 8 | c->mat.argb.b);
	}
	else if (sublim == 2)
		ft_lighting(data, c, shadow_tracing);
	else if (sublim == 1)
		ft_lighting(data, c, something_block_the_light);
	else if (sublim == 0)
		ft_lighting_simple(data, c);
	return (c->mat.argb.r << 16 | c->mat.argb.g << 8 | c->mat.argb.b);
}

///////////////////////////////////////////////////////////////////////////////]
// IF ambient light has a texture, return the correct pixel instead of 
// 		simple bg color
static unsigned int	h_bg_texture(t_data *data, t_c_px *c)
{
	double	l_x;
	double	l_y;

	l_x = fmin(1.0, fmax(0.0, atan2(c->v.dz, c->v.dx) / (2 * PI) + 0.5));
	l_y = fmin(1.0, fmax(0.0, acos(c->v.dy) / PI));
	c->mat.argb = return_px_img(data->bgl[0]->texture, l_x, l_y);
	return (c->mat.argb.r << 16 | c->mat.argb.g << 8 | c->mat.argb.b);
}
