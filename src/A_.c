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

// double fov_radians = FOV * (M_PI / 180); // Convert FOV from degrees to radians
// double px = fov_radians / SIZE_SCREEN_X;
t_rgb	calculate_pixel_color(t_data *data, t_vect *v);
double	distance_from_sphere(t_data *data, t_calcul *calc, t_vect *v);
double	distance_from_plane(t_data *data, t_calcul *c, t_vect *v);

///////////////////////////////////////////////////////////////////////////////]
t_rgb	calculate_pixel_color(t_data *data, t_vect *v)
{
	t_calcul	c;
	void **ptr;
	c.dist = -1;
	ft_memset(&c.px_color, 0, sizeof(t_rgb));

	ptr = (void **)data->spheres - 1;
	while (++ptr && *ptr)
	{
		c.sphere = (t_sphere *)*ptr;
		c.tmp_dist = distance_from_sphere(data, &c, v);
		if (c.tmp_dist > 0 && (c.tmp_dist < c.dist || c.dist < 0))
		{
			c.dist = c.tmp_dist;
			c.px_color = c.sphere->color;
		}
	}

	ptr = (void **)data->planes - 1;
	while (++ptr && *ptr)
	{
		c.plane = (t_plane *)*ptr;
		c.tmp_dist = distance_from_plane(data, &c, v);
		if (c.tmp_dist > 0 && (c.tmp_dist < c.dist || c.dist < 0))
		{
			c.dist = c.tmp_dist;
			c.px_color = c.plane->color;
		}
	}

	if (c.dist < 0)
		c.px_color = data->light[0]->color;
	// ptr = data->cylinders - 1;
	// while (++ptr && *ptr)
	// {
	// 	c.cylinder = (t_cylinder *)*ptr;
	// 	c.tmp_dist = distance_from_cyl(data, &c, v);
	// 	if (c.tmp_dist > 0 && (c.tmp_dist < c.dist || c.dist < 0))
	// 		c.px_color = c.sphere->color;
	// }
	return (c.px_color);


}

///////////////////////////////////////////////////////////////////////////////]
static double h_smalest_Δ(double a, double b)
{
	if (a < 0 && b < 0)
		return (-1.0);
	if (a < 0)
		return (b);
	if (b < 0)
		return (a);
	return (a * (a < b) + b * (b < a));
}

//	(x-x0)² + (y-y0)² + (z-z0)² = R²
double	distance_from_sphere(t_data *data, t_calcul *calc, t_vect *v)
{
	t_coor	xyz = data->camera[0]->xyz;
	t_vect	abc = data->camera[0]->abc;
	t_sphere *s = calc->sphere;

//	diff center sphere and center camera
	double x0 = xyz.x - s->xyz.x;
	double y0 = xyz.y - s->xyz.y;
	double z0 = xyz.z - s->xyz.z;

	double a = abc.dx * abc.dx + abc.dy * abc.dy + abc.dz * abc.dz;
	double b = 2 * (abc.dx * x0 + abc.dy * y0 + abc.dz * z0);
	double c = x0 * x0 + y0 * y0 + z0 * z0 - s->radius;

	double Δ = b * b - 4 * a * c;

	if (Δ < 0)
		return (-1.0);

	double	det1 = (-b + sqrt(Δ)) / (2 * a);
	double	det2 = (-b - sqrt(Δ)) / (2 * a);

	double det = h_smalest_Δ(det1, det2);

	if (det < 0)
		return (-1.0);

	calc->inter_point.x = xyz.x + abc.dx * det;
	calc->inter_point.y = xyz.y + abc.dy * det;
	calc->inter_point.z = xyz.z + abc.dz * det;

	return (det);
}

///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
double	distance_from_plane(t_data *data, t_calcul *c, t_vect *v)
{
	double top;
	double bot;

	t_coor	xyz = data->camera[0]->xyz;
	t_vect	abc = data->camera[0]->abc;
	t_plane *p = c->plane;

	top = -(p->abc.dx * xyz.x + p->abc.dy * xyz.y + p->abc.dz * xyz.z + p->d);
	bot = p->abc.dx * abc.dx + p->abc.dy * abc.dy + p->abc.dz * abc.dz;

	if (!top || !bot)
		return (-1.0);

	double	det = top / bot;

	if (det < 0)
		return (-1.0);

	c->inter_point.x = xyz.x + abc.dx * det;
	c->inter_point.y = xyz.y + abc.dy * det;
	c->inter_point.z = xyz.z + abc.dz * det;

	return (det);
}

/*
typedef struct s_matrice {
	double	x1;
	double	y1;
	double	z1;
	double	x2;
	double	y2;
	double	z2;
	double	x3;
	double	y3;
	double	z3;
}	t_matrice;

void	fill_matrice(t_matrice *m, t_vect *a, t_vect *b, t_vect *c)
{
	m->x1 = a->dx;
	m->y1 = a->dy;
	m->z1 = a->dz;

	m->x2 = b->dx;
	m->y2 = b->dy;
	m->z2 = b->dz;

	m->x3 = c->dx;
	m->y3 = c->dy;
	m->z3 = c->dz;
}

void	apply_rotation_matrice(t_matrice *rota_m, t_vect *v);

void	multiply_matrices3(t_matrice *result, t_matrice *a, t_matrice *b)
{
	result->x1 = a->x1 * b->x1 + a->x2 * b->x2 + a->x3 * b->x3;
	result->y1 = a->x1 * b->x2 + a->x2 * b->y2 + a->x3 * b->z2;
	result->z1 = a->x1 * b->x3 + a->x2 * b->y3 + a->x3 * b->z3;

	result->x2 = a->y1 * b->x1 + a->y2 * b->y1 + a->y3 * b->z1;
	result->y2 = a->y1 * b->x2 + a->y2 * b->y2 + a->y3 * b->z2;
	result->z2 = a->y1 * b->x3 + a->y2 * b->y3 + a->y3 * b->z3;

	result->x3 = a->z1 * b->x1 + a->z2 * b->y1 + a->z3 * b->z1;
	result->y3 = a->z1 * b->x2 + a->z2 * b->y2 + a->z3 * b->z2;
	result->z3 = a->z1 * b->x3 + a->z2 * b->y3 + a->z3 * b->z3;
}
*/