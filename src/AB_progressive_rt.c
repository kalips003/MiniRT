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

t_coor			calculate_random_ray(t_data *data, t_c_px *calcul, \
	int num_bounce);
static t_vect	random_ray(t_c_px *calcul);
static t_coor	h_bg_texture(t_data *data, t_c_px *c);
void			clean_buffer(t_data *data);

///////////////////////////////////////////////////////////////////////////////]
t_coor	calculate_random_ray(t_data *data, t_c_px *calcul, int num_bounce)
{
	t_c_px	c;
	t_coor	rtrn;

	if (num_bounce > MAX_RAY_BOUNCE)
		return ((t_coor){0});
	if (!find_coli(data, calcul, NOT_SHADOWS, SET_DIST))
	{
		if (!num_bounce && data->bgl[0]->texture)
			return (h_bg_texture(data, calcul));
		return ((t_coor){0});
	}
	if (calcul->mat.light > EPSILON)
		return ((t_coor){
			calcul->mat.argb.r * calcul->mat.light,
			calcul->mat.argb.g * calcul->mat.light,
			calcul->mat.argb.b * calcul->mat.light});
	ini_new_calcul_struct(calcul, &c, 0b11);
	c.c0 = calcul->inter;
	c.vn = calcul->vn;
	c.v = random_ray(&c);
	rtrn = calculate_random_ray(data, &c, num_bounce + 1);
	rtrn.x = calcul->mat.argb.r * rtrn.x / 255.0 * 2.0;
	rtrn.y = calcul->mat.argb.g * rtrn.y / 255.0 * 2.0;
	rtrn.z = calcul->mat.argb.b * rtrn.z / 255.0 * 2.0;
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
static t_vect	random_ray(t_c_px *calcul)
{
	t_vect	xyz;
	t_obj	o;
	double	a;
	double	b;

	a = 2 * PI * ((double)rand() / RAND_MAX);
	b = acos(1.0 - 2.0 * (double)rand() / RAND_MAX);
	xyz.dx = cos(a) * sin(b);
	xyz.dy = sin(a) * sin(b);
	xyz.dz = cos(b);
	o.view = calcul->vn;
	create_vector_space(&o);
	xyz = mult_3x3_vect(&o, &xyz);
	ft_normalize_vect(&xyz);
	if (ft_dot_p(&calcul->vn, &xyz) < EPSILON)
		xyz = (t_vect){-xyz.dx, -xyz.dy, -xyz.dz};
	return (xyz);
}

///////////////////////////////////////////////////////////////////////////////]
static t_coor	h_bg_texture(t_data *data, t_c_px *c)
{
	double	l_x;
	double	l_y;

	l_x = fmin(1.0, fmax(0.0, atan2(c->v.dz, c->v.dx) / (2 * PI) + 0.5));
	l_y = fmin(1.0, fmax(0.0, acos(c->v.dy) / PI));
	c->mat.argb = return_px_img(data->bgl[0]->texture, l_x, l_y);
	return ((t_coor){
		c->mat.argb.r,
		c->mat.argb.g,
		c->mat.argb.b
	});
}

///////////////////////////////////////////////////////////////////////////////]
void	clean_buffer(t_data *data)
{
	int		x;
	int		y;
	char	*dst;
	int		offset;

	y = -1;
	while (++y < SZ_Y)
	{
		x = -1;
		while (++x < SZ_X)
		{
			offset = (y * data->buffer.ll + x * (data->buffer.bpp / 8));
			dst = data->buffer.addr + offset;
			*(unsigned int *)dst = 0;
		}
	}
	mlx_put_image_to_window(data->mlx, data->win, data->buffer.img, 0, 0);
}
