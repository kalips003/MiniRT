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

#define PX_MIDDLE 0.3
#define PX_EDGE 0.175
#define PX_CORNER 0.025

int					ft_render_frame_aa(t_data *data, int sublim);
static void			ft_anti_aliasing_aa(t_data *data);
static unsigned int	ft_average_pixel_buff(t_img *buff, int x, int y);
void				w_px_buff(t_img *buff, int x, int y, unsigned int color);
static void			h_weight_color(t_img *buffer, int *xy, t_coor *rtrn, double scalar);

///////////////////////////////////////////////////////////////////////////////]
int	ft_render_frame_aa(t_data *data, int sublim)
{
	int		xy[2];
	t_c_px	c;

	ft_memset(&c, 0, sizeof(t_c_px));
	ini_stack(data, &c);
	c.c0 = data->eye.c->O.c0;
	xy[Y] = -1;
	while (++xy[Y] < SZ_Y * 2)
	{
		if (xy[Y] % (SZ_Y * 2 / 10) == 0)
			printf(C_552"%d %% done\n", (xy[Y] * 100) / (SZ_Y * 2));
		xy[X] = -1;
		while (++xy[X] < SZ_X * 2)
		{
			if (xy[X] % 2 && xy[Y] % 2)
				continue ;
			c.transparence_depth = 0;
			c.reflected_depth = 0;
			c.v = v_cam(data, xy[X], xy[Y], AA);
			w_px_buff(&data->buff_aa, xy[X], xy[Y], \
				calc_px_color(data, &c, sublim));
		}
	}
	printf(C_552"100 %% done\n"C_515"PRESS ENTER TO SUBLIM!\n");
	return (free_s(gnl(0)), ft_anti_aliasing_aa(data), 0);
}

///////////////////////////////////////////////////////////////////////////////]
static void	ft_anti_aliasing_aa(t_data *data)
{
	int				x;
	int				y;
	unsigned int	color;

	y = 0;
	while (y < SZ_Y * 2 - 1)
	{
		x = 0;
		while (x < SZ_X * 2 - 1)
		{
			if (x == 0 || y == 0)
				color = *(unsigned int *)(data->buff_aa.addr + y * \
					data->buff_aa.ll + x * (data->buff_aa.bpp / 8));
			else
				color = ft_average_pixel_buff(&data->buff_aa, x, y);
			mlx_pixel_put(data->mlx, data->win, x / 2, y / 2, color);
			x += 2;
		}
		y += 2;
	}
}

///////////////////////////////////////////////////////////////////////////////]
// does the averaging
static unsigned int	ft_average_pixel_buff(t_img *buff, int x, int y)
{
	t_coor	px;
	t_rgb	color;

	px = (t_coor){0};
	h_weight_color(buff, (int []){x, y}, &px, PX_MIDDLE);
	h_weight_color(buff, (int []){x - 1, y}, &px, PX_EDGE);
	h_weight_color(buff, (int []){x + 1, y}, &px, PX_EDGE);
	h_weight_color(buff, (int []){x, y - 1}, &px, PX_EDGE);
	h_weight_color(buff, (int []){x, y + 1}, &px, PX_EDGE);
	color.r = min(255, max(0, (int)round(px.x)));
	color.g = min(255, max(0, (int)round(px.y)));
	color.b = min(255, max(0, (int)round(px.z)));
	return ((unsigned int)(color.r << 16 | color.g << 8 | color.b));
}

///////////////////////////////////////////////////////////////////////////////]
void	w_px_buff(t_img *buff, int x, int y, unsigned int color)
{
	char	*dst;
	int		offset;

	offset = (y * buff->ll + x * (buff->bpp / 8));
	dst = buff->addr + offset;
	*(unsigned int *)dst = color;
}

///////////////////////////////////////////////////////////////////////////////]
// read the pixel xy, weight it by scalar
static void	h_weight_color(t_img *buffer, int *xy, t_coor *rtrn, double scalar)
{
	unsigned int	color;

	color = *(unsigned int *)(buffer->addr + (xy[Y] * buffer->ll \
		+ xy[X] * (buffer->bpp / 8)));
	rtrn->x += ((color >> 16) & 0xFF) * scalar;
	rtrn->y += ((color >> 8) & 0xFF) * scalar;
	rtrn->z += (color & 0xFF) * scalar;
}
