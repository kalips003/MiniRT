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

int	ft_render_rt(t_data *data);
int	ft_render_rc(t_data *data);
static void	ft_find_pixel_vector(t_data *data, t_vect *v, int x, int y);
t_rgb	calculate_pixel_color(t_data *data, t_vect *v);
int	ft_find_pixel_colision(t_data *data, t_calcul *c, t_vect *v);

///////////////////////////////////////////////////////////////////////////////]
// 	RENDER RAY CASTING
int	ft_render_rc(t_data *data)
{
	int x;
	int y = -1;
	t_vect v;

	t_calcul c;

	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		while (++x < SIZE_SCREEN_X)
		{
			ft_find_pixel_vector(data, &v, x, y);
			ft_find_pixel_colision(data, &c, &v);

			//> apply color at pixel pos x.y
			mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
		}
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// 	RENDER RAY TRACING
int	ft_render_rt(t_data *data)
{
	int x;
	int y = -1;
	t_vect v;
	t_rgb color;

	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		while (++x < SIZE_SCREEN_X)
		{
			ft_find_pixel_vector(data, &v, x, y);
			
			//>	for each object, calculate if there is colision, if there is, calcul the closest intersection point, retrun color?
			color = calculate_pixel_color(data, &v);

			//> apply color at pixel pos x.y
			mlx_pixel_put(data->mlx, data->win, x, y, color.r << 16 | color.g << 8 | color.b);
		}
	}
	return (0);
}

static void	ft_find_pixel_vector(t_data *data, t_vect *v, int x, int y)
{
	double	angle_α;
	double	angle_β;

	// where α is a rotation around the Y axis == screen width
	// β is a rotation around the X axis == screen height
	angle_α = data->e.px0 + x * data->e.px;
	angle_β = data->e.py0 + y * data->e.px;


	ft_normalize_vect(v);
}

// 	OLD EULER ROTATION
// static void	ft_find_pixel_vector(t_data *data, t_vect *v, int x, int y)
// {
// 	double	angle_α;
// 	double	angle_β;

// 	// where α is a rotation around the Y axis == screen width
// 	// β is a rotation around the X axis == screen height
// 	angle_α = data->px0 + x * data->px;
// 	angle_β = data->py0 + y * data->px;
// 	//	a.cosα + b.sinα.sinβ + c.sinα.cosβ
// 	//	b.cosβ - c.sinβ
// 	//	-a.sinα + b.cosα.sinβ + c.cosα.cosβ
// 	v->dx = data->eye->abc.dx * cos(angle_α) + data->eye->abc.dy * sin(angle_α) * sin(angle_β) + data->eye->abc.dz * sin(angle_α) * cos(angle_β);
// 	v->dy = data->eye->abc.dy * cos(angle_β) - data->eye->abc.dz * sin(angle_β);
// 	v->dz = -data->eye->abc.dx * sin(angle_α) + data->eye->abc.dy * cos(angle_α) * sin(angle_β) + data->eye->abc.dz * cos(angle_α) * cos(angle_β);

// 	ft_normalize_vect(v);
// }
///////////////////////////////////////////////////////////////////////////////]
t_rgb	calculate_pixel_color(t_data *data, t_vect *v)
{
	t_calcul	c;

	ft_memset(&c, 0, sizeof(t_calcul));

	if (ft_find_pixel_colision(data, &c, v))
		ft_handle_sky(data, &c);
	else
		ft_handle_shadows(data, &c);

	return (c.px_color);
}

///////////////////////////////////////////////////////////////////////////////]
// 	find the pixel color of the closest object
// fills:
// position xyz of the colision (default 0,0,0)
// abc vector normal to the surface
// color of object in view
int	ft_find_pixel_colision(t_data *data, t_calcul *c, t_vect *v)
{
	t_sphere	**sphere_ptr;
	t_plane		**plane_ptr;
	t_cylinder	**cyl_ptr;

	c->dist = -1.0;
	sphere_ptr = data->spheres - 1;
	while (++sphere_ptr && *sphere_ptr)
		distance_from_sphere(data, c, v, *sphere_ptr);

	plane_ptr = data->planes - 1;
	while (++plane_ptr && *plane_ptr)
		distance_from_plane(data, c, v, *plane_ptr);

	// cyl_ptr = data->cylinders - 1;
	// while (++cyl_ptr && *cyl_ptr)
	// 	distance_from_cyl(data, &c, v, cyl_ptr);
	return (c->dist == -1.0);
}



