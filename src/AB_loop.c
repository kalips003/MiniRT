/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AB_loop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/04 17:34:15 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int					ft_render_frame_multi_prog(t_data *data, int sublim);
static void			h_loop_thread(t_data *data, t_c_px *c, t_c_px *c_tab, \
	int sublim);
static void			*f_thread2(void *calcul);
static unsigned int	ft_new_average_color(t_data *data, int x, int y, \
	t_rgb new_ray);

///////////////////////////////////////////////////////////////////////////////]
int	ft_render_frame_multi_prog(t_data *data, int sublim)
{
	t_c_px	c;
	t_c_px	*calcul_tab;

	calcul_tab = mem(0, sizeof(t_c_px) * NUM_THREAD);
	if (!calcul_tab)
		return (1);
	ini_stack(data, &c);
	c.c0 = data->eye.c->O.c0;
	h_loop_thread(data, &c, calcul_tab, sublim);
	free_s(calcul_tab);
	mlx_put_image_to_window(data->mlx, data->win, data->buffer.img, 0, 0);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_loop_thread(t_data *data, t_c_px *c, t_c_px *c_tab, int sublim)
{
	pthread_t	threads[NUM_THREAD];
	int			y;

	y = -1;
	while (++y < NUM_THREAD)
	{
		ft_memcpy(c_tab[y].inside, c->inside, sizeof(c->inside));
		c_tab[y].stack_top = c->stack_top;
		c_tab[y].data = data;
		c_tab[y].c0 = c->c0;
		c_tab[y].transparence_depth = y;
		c_tab[y].reflected_depth = sublim;
		if (pthread_create(&threads[y], NULL, f_thread2, &c_tab[y]))
		{
			printf("Error creating thread for row %d\n", y);
			break ;
		}
	}
	while (--y >= 0)
		pthread_join(threads[y], NULL);
}

///////////////////////////////////////////////////////////////////////////////]
static void	*f_thread2(void *calcul)
{
	t_c_px	*c;
	int		xy[2];
	t_coor	rgb2;
	t_rgb	rgb;

	c = (t_c_px *)calcul;
	xy[Y] = c->transparence_depth;
	while (xy[Y] < SZ_Y)
	{
		xy[X] = -1;
		while (++xy[X] < SZ_X)
		{
			c->v = v_cam(c->data, xy[X], xy[Y], NOT_AA);
			rgb2 = calculate_random_ray(c->data, c, 0);
			rgb = (t_rgb){
				clamp((int)round(rgb2.x), 0, 255),
				clamp((int)round(rgb2.y), 0, 255),
				clamp((int)round(rgb2.z), 0, 255)};
			w_px_buff(&c->data->buffer, xy[X], xy[Y], \
				ft_new_average_color(c->data, xy[X], xy[Y], rgb));
		}
		xy[Y] += NUM_THREAD;
	}
	return (NULL);
}

///////////////////////////////////////////////////////////////////////////////]
static unsigned int	ft_new_average_color(t_data *data, int x, int y, \
	t_rgb new_ray)
{
	unsigned int	color;
	double			px_ite;
	t_coor			current_px;
	t_rgb			rgb;

	color = *(unsigned int *)(data->buffer.addr + (y * data->buffer.ll + x \
		* (data->buffer.bpp / 8)));
	px_ite = *(int *)&data->ram;
	if (!new_ray.r && !new_ray.g && !new_ray.b)
		px_ite *= 3;
	current_px.x = (((color >> 16) & 0xFF) * px_ite + new_ray.r) / (px_ite + 1);
	current_px.y = (((color >> 8) & 0xFF) * px_ite + new_ray.g) / (px_ite + 1);
	current_px.z = ((color & 0xFF) * px_ite + new_ray.b) / (px_ite + 1);
	rgb.r = clamp((int)round(current_px.x), 0, 255);
	rgb.g = clamp((int)round(current_px.y), 0, 255);
	rgb.b = clamp((int)round(current_px.z), 0, 255);
	color = rgb.r << 16 | rgb.g << 8 | rgb.b;
	return (color);
}
