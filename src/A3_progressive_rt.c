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

int	ft_render_frame_multi_prog(t_data *data, int sublim);
t_coor	calculate_random_ray(t_data *data, t_c_px *calcul, int num_bounce);
t_vect	random_ray(t_c_px *calcul);
void	clean_buffer(t_data *data);

unsigned int	ft_new_average_color(t_data *data, int x, int y, t_rgb new_ray);
int	ft_render_frame_multi_prog(t_data *data, int sublim);
static void	h_loop_thread(t_data *data, t_c_px *c, t_c_px *calcul_tab, int sublim);
static void	*f_thread2(void *calcul);

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
t_coor	calculate_random_ray(t_data *data, t_c_px *calcul, int num_bounce)
{
	if (num_bounce > MAX_RAY_BOUNCE)
		return ((t_coor){0});
	if (!find_coli(data, calcul, NOT_SHADOWS, SET_DIST))
	{
		if (!num_bounce && data->bgl[0]->texture)
			return (h_bg_texture(data, calcul));
		return ((t_coor){0});
	}
		// return ((t_rgb){
		// 	(int)(round(data->bgl[0]->rgb.r * data->bgl[0]->ratio)),
		// 	(int)(round(data->bgl[0]->rgb.g * data->bgl[0]->ratio)),
		// 	(int)(round(data->bgl[0]->rgb.b * data->bgl[0]->ratio))
		// });
	if (calcul->mat.light > EPSILON)
	{
		return ((t_coor){
			calcul->mat.argb.r * calcul->mat.light,
			calcul->mat.argb.g * calcul->mat.light,
			calcul->mat.argb.b * calcul->mat.light
		});
	}
	t_c_px	c;
	ini_new_calcul_struct(calcul, &c, 0b11);
	c.c0 = calcul->inter;
	c.vn = calcul->vn;
	c.v = random_ray(&c);
	t_coor rtrn;
	rtrn = calculate_random_ray(data, &c, num_bounce + 1);

	// rtrn.x = sqrt((calcul->mat.argb.r / 255.0) * (rgb.r / 255.0)) * 255.0 * 2.0;
	// rtrn.y = sqrt((calcul->mat.argb.g / 255.0) * (rgb.g / 255.0)) * 255.0 * 2.0;
	// rtrn.z = sqrt((calcul->mat.argb.b / 255.0) * (rgb.b / 255.0)) * 255.0 * 2.0;

	rtrn.x = calcul->mat.argb.r * rtrn.x / 255.0 * 2.0;
	rtrn.y = calcul->mat.argb.g * rtrn.y / 255.0 * 2.0;
	rtrn.z = calcul->mat.argb.b * rtrn.z / 255.0 * 2.0;
	return (rtrn);
}


t_vect	random_ray(t_c_px *calcul)
{
	double	u1 = (double)rand() / RAND_MAX;
	double	u2 = (double)rand() / RAND_MAX;
	double	θ = 2 * PI * u1;
	double	ϕ = acos(1 - 2 * u2);
	// double ϕ = acos(pow(1 - u2, 1 / (roughness + 1)));// rough = 0 (mirror) rough = 1 (pure diffuse)

	t_vect	xyz;
	xyz.dx = cos(θ) * sin(ϕ);
	xyz.dy = sin(θ) * sin(ϕ);
	xyz.dz = cos(ϕ);

	t_obj	o;
	o.view = calcul->vn;
	create_vector_space(&o);
	xyz = mult_3x3_vect(&o, &xyz);
	ft_normalize_vect(&xyz);
	if (ft_dot_p(&calcul->vn, &xyz) < EPSILON)
		xyz = (t_vect){-xyz.dx, -xyz.dy, -xyz.dz};
	if (calcul->print == 1)
		printf("vn: [%.3f,%.3f,%.3f]\n", calcul->vn.dx, calcul->vn.dy, calcul->vn.dz);
	// t_vect final_dir = lerp(specular_factor, D, R);
		// Specular reflection direction R: Compute the perfect reflection direction R
		// Randomly sampled diffuse direction D: Sample a diffuse direction DD with the modified distribution
	// lerp(a, b, t) = a * (1 - t) + b * t
	// ft_normalize_vect(&final_dir);
	// printf("v: [%.3f,%.3f,%.3f]\n", xyz.dx, xyz.dy, xyz.dz);
	return (xyz);
}

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

///////////////////////////////////////////////////////////////////////////////]
unsigned int	ft_new_average_color(t_data *data, int x, int y, t_rgb new_ray)
{
	unsigned int	color;
	t_img 			*buffer;

	buffer = &data->buffer;
	color = *(unsigned int *)(buffer->addr + (y * buffer->ll + x * (buffer->bpp / 8)));

	double px_iteration = *(int *)&data->ram;
	if (!new_ray.r && !new_ray.g && !new_ray.b)
		px_iteration *= 3;

	t_coor	current_px;
	current_px.x = (((color >> 16) & 0xFF) * px_iteration + new_ray.r) / (px_iteration + 1);
	current_px.y = (((color >> 8) & 0xFF) * px_iteration + new_ray.g) / (px_iteration + 1);
	current_px.z = ((color & 0xFF) * px_iteration + new_ray.b) / (px_iteration + 1);

	t_rgb rgb;
	rgb.r = clamp((int)round(current_px.x), 0, 255);
	rgb.g = clamp((int)round(current_px.y), 0, 255);
	rgb.b = clamp((int)round(current_px.z), 0, 255);

	color = rgb.r << 16 | rgb.g << 8 | rgb.b;
	return (color);
}

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
static void	h_loop_thread(t_data *data, t_c_px *c, t_c_px *calcul_tab, int sublim);
static void	*f_thread2(void *calcul);
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
		calcul_tab[y].object = (t_obj2 *)data;
		calcul_tab[y].transparence_depth = y;
		calcul_tab[y].reflected_depth = sublim;
		if (pthread_create(&threads[y], NULL, f_thread2, &calcul_tab[y]))
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
	t_data	*data;
	int		xy[2];

	c = (t_c_px *)calcul;
	data = (t_data *)c->object;
	xy[Y] = c->transparence_depth;
	c->object = NULL;
	while (xy[Y] < SZ_Y)
	{
		xy[X] = -1;
		while (++xy[X] < SZ_X)
		{
			c->v = v_cam(data, xy[X], xy[Y], NOT_AA);
			t_coor	rgb2;
			t_rgb	rgb;
			rgb2 = calculate_random_ray(data, c, 0);
			rgb = (t_rgb){
				clamp((int)round(rgb2.x), 0, 255),
				clamp((int)round(rgb2.y), 0, 255),
				clamp((int)round(rgb2.z), 0, 255)
			};
			w_px_buff(&data->buffer, xy[X], xy[Y], ft_new_average_color(data, xy[X], xy[Y], rgb));
		}
		xy[Y] += NUM_THREAD;
	}
	return (NULL);
}
