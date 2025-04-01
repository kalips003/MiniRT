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
t_rgb	calculate_random_ray(t_data *data, t_c_px *calcul, int num_bounce);
t_vect	random_ray(t_c_px *calcul);
void	clean_buffer(t_data *data);

unsigned int	ft_new_average_color(t_data *data, int x, int y, t_rgb new_ray);
int	ft_render_frame_multi_prog(t_data *data, int sublim);
static void	h_loop_thread(t_data *data, t_c_px *c, t_c_px *calcul_tab, int sublim);
static void	*f_thread2(void *calcul);



///////////////////////////////////////////////////////////////////////////////]
t_rgb	calculate_random_ray(t_data *data, t_c_px *calcul, int num_bounce)
{
	t_rgb	rgb;

	if (num_bounce > MAX_RAY_BOUNCE)
		return ((t_rgb){0});
	if (!find_coli(data, calcul, NOT_SHADOWS, SET_DIST))
		return ((t_rgb){0});
		// return ((t_rgb){
		// 	(int)(round(data->bgl[0]->rgb.r * data->bgl[0]->ratio)),
		// 	(int)(round(data->bgl[0]->rgb.g * data->bgl[0]->ratio)),
		// 	(int)(round(data->bgl[0]->rgb.b * data->bgl[0]->ratio))
		// });
	if (calcul->mat.light > EPSILON)
	{
		rgb.r = (int)(round(calcul->mat.argb.r * calcul->mat.light));
		rgb.g = (int)(round(calcul->mat.argb.g * calcul->mat.light));
		rgb.b = (int)(round(calcul->mat.argb.b * calcul->mat.light));
		return (rgb);
	}
	t_c_px	c;
	ini_new_calcul_struct(calcul, &c, 0b11);
	c.c0 = calcul->inter;
	c.v = random_ray(&c);
	rgb = calculate_random_ray(data, &c, num_bounce + 1);
	t_coor rtrn;
	rtrn.x = calcul->mat.argb.r * rgb.r / 255.0;
	rtrn.y = calcul->mat.argb.g * rgb.g / 255.0;
	rtrn.z = calcul->mat.argb.b * rgb.b / 255.0;


	t_rgb	rgb2;

	rgb2.r = (int)round(rtrn.x);
	rgb2.g = (int)round(rtrn.y);
	rgb2.b = (int)round(rtrn.z);

	rgb2.r = clamp(rgb2.r, 0, 255);
	rgb2.g = clamp(rgb2.g, 0, 255);
	rgb2.b = clamp(rgb2.b, 0, 255);
	// printf("rgb: [%d,%d,%d]\n", calcul->mat.argb.r, calcul->mat.argb.g, calcul->mat.argb.b);
	return (rgb2);
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
	// t_vect final_dir = lerp(specular_factor, D, R);
		// Specular reflection direction R: Compute the perfect reflection direction R
		// Randomly sampled diffuse direction D: Sample a diffuse direction DD with the modified distribution
	// lerp(a, b, t) = a * (1 - t) + b * t
	// ft_normalize_vect(&final_dir);
	// printf("v: [%.3f,%.3f,%.3f]\n", xyz.dx, xyz.dy, xyz.dz);
	return (xyz);
}

// t_vect	random_ray_rr(t_c_px *calcul)
// {
// 	t_vect	rtrn;
// 	double	u = (double)rand() / RAND_MAX;

// 	if (calcul->mat.tr > EPSILON)
// 	{
// 		double	incident_gamma;
// 		double	refracted_gamma;
// 		int		neg;

// 		neg = 1;
// 		if (calcul->object->type == PLANE || calcul->object->type == SPRITE || calcul->object->type == PARABOLOID
// 			|| !rtrn_top_stack_gamma(calcul, calcul->object, &incident_gamma, &refracted_gamma))
// 			c.v = calcul->v;
// 		else
// 			c.v = ft_vect_refracted(&calcul->v, &calcul->vn, incident_gamma / refracted_gamma, &neg);
// 		c.c0 = new_moved_point(&calcul->inter, &calcul->v, neg * EPSILON);
// 		if (neg == -1)
// 			push_stack(calcul->inside, calcul->object, &calcul->stack_top, MAX_MIRROR_DEPTH - 1);
// 	}
// 	if (calcul->mat.mi > EPSILON)
// }

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


/*

void	clean_buffer(t_data *data);
int 	ft_loop_progressive(t_data *data);
int	ft_render_random_px(t_data *data, int sublim, t_camera *cam);
void	put_pixel_buffer_v2(t_img *buffer, int x, int y, unsigned int color);
unsigned int	ft_new_average_color(t_img *buffer, int x, int y, t_argb new_ray);

///////////////////////////////////////////////////////////////////////////////]


// ray bacck trackinng loop
int 	ft_loop_progressive(t_data *data)
{
	struct stat	file_stat;
	
	if (stat(data->av[1], &file_stat) == 0)
	{
		if (file_stat.st_mtime != data->last_modif_time)
		{
			// h_refresh_input_file(data, file_stat.st_mtime);
			clean_buffer(data);
		}
	}
	else
		perror("stat");
	// sennnd ranndom ray


	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	ft_render_random_px(t_data *data, int sublim, t_camera *cam)
{
	t_calcul_px	c;

	ft_memset(&c, 0, sizeof(t_calcul_px));
	ini_stack(data, &c);
	c.c0 = data->eye.c->O.c0;

	data->change = 1;
	while (data->change)
	{
		int			x = rand() % SIZE_SCREEN_X;
		int			y = rand() % SIZE_SCREEN_Y;
		double		angleA = atan((x - SIZE_SCREEN_X / 2) * cam->fov_cst);
		double		angleB = atan((y - SIZE_SCREEN_Y / 2) * cam->fov_cst);

		c.transparence_depth = 0; 
		c.reflected_depth = 0; 
		c.v = combined_quaternion_rotation(&cam->O, angleA, angleB);
		calculate_pixel_color(data, &c, 3);
		unsigned int av_color = ft_new_average_color(&data->buffer, x, y, c.argb);
		mlx_pixel_put(data->mlx, data->win, x, y, av_color);
	}
	return (0);
}

*/

///////////////////////////////////////////////////////////////////////////////]
unsigned int	ft_new_average_color(t_data *data, int x, int y, t_rgb new_ray)
{
	unsigned int	color;
	t_img 			*buffer;

	buffer = &data->buffer;
	color = *(unsigned int *)(buffer->addr + (y * buffer->ll + x * (buffer->bpp / 8)));

	double px_iteration = *(int *)&data->ram;

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
			t_rgb	rgb;
			rgb = calculate_random_ray(data, c, 0);
			w_px_buff(&data->buffer, xy[X], xy[Y], ft_new_average_color(data, xy[X], xy[Y], rgb));
		}
		xy[Y] += NUM_THREAD;
	}
	return (NULL);
}
