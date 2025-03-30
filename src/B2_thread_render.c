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

int			ft_render_frame_multi(t_data *data, int sublim);
static void	h_loop_thread(t_data *data, t_c_px *c, t_c_px *calcul_tab, int sublim);
static void	*f_thread(void *calcul);

///////////////////////////////////////////////////////////////////////////////]
int	ft_render_frame_multi(t_data *data, int sublim)
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
static void	h_loop_thread(t_data *data, t_c_px *c, t_c_px *calcul_tab, int sublim)
{
	pthread_t	threads[NUM_THREAD];
	int			y;

	y = -1;
	while (++y < NUM_THREAD)
	{
		ft_memcpy(calcul_tab[y].inside, c->inside, sizeof(c->inside));
		calcul_tab[y].stack_top = c->stack_top;
		calcul_tab[y].c0 = c->c0;
		calcul_tab[y].object = data;
		calcul_tab[y].transparence_depth = y;
		calcul_tab[y].reflected_depth = sublim;
		if (pthread_create(&threads[y], NULL, f_thread, &calcul_tab[y]))
		{
			printf("Error creating thread for row %d\n", y);
			break ;
		}
	}
	while (--y >= 0)
		pthread_join(threads[y], NULL);
}

///////////////////////////////////////////////////////////////////////////////]
static void	*f_thread(void *calcul)
{
	t_c_px	*c;
	t_data	*data;
	int		xy[2];
	int		sublim;

	c = (t_c_px *)calcul;
	data = c->object;
	xy[Y] = c->transparence_depth;
	sublim = c->reflected_depth;
	c->object = NULL;
	while (xy[Y] < SZ_Y)
	{
		xy[X] = -1;
		while (++xy[X] < SZ_X)
		{
			c->transparence_depth = 0;
			c->reflected_depth = 0;
			c->ao = 1.0;
			c->v = v_cam(data, xy[X], xy[Y], NOT_AA);
			w_px_buff(&data->buffer, xy[X], xy[Y], calc_px_color(data, c, sublim));
		}
		xy[Y] += NUM_THREAD;
	}
	return (NULL);
}
