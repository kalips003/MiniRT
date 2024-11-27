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

double	distance_from_sphere(t_data *data, t_calcul *calc, t_vect *v, t_sphere *sphere);
double	distance_from_plane(t_data *data, t_calcul *c, t_vect *v, t_plane *plane);
static double h_smalest_Δ(double a, double b);
// void	multiply_matrices3(t_matrice *result, t_matrice *a, t_matrice *b)

///////////////////////////////////////////////////////////////////////////////]
typedef struct s_sphere_calc {
	double x0;
	double y0;
	double z0;

	double a;
	double b;
	double c;

	double Δ;

	double	det1;
	double	det2;

	double dist;
} t_sphere_calc;


///////////////////////////////////////////////////////////////////////////////]
// 		if colition, fills in the xyz position of the closest positive contact point
// 	return dist from sphere, -1 if no colosion or not in sight
//	(x-x0)² + (y-y0)² + (z-z0)² = R²
double	distance_from_sphere(t_data *data, t_calcul *calc, t_vect *v, t_sphere *sphere)
{
	t_sphere_calc	c;

//	diff center sphere and center camera
	c.x0 = data->e.c->xyz.x - sphere->xyz.x;
	c.y0 = data->e.c->xyz.y - sphere->xyz.y;
	c.z0 = data->e.c->xyz.z - sphere->xyz.z;

// RESOLVE ((t.Vx + EYEx) - x0)² + ((t.Vy + EYEy) - y0)² + ((t.Vz + EYEz) - z0)² = R²
// ==> At² + Bt + C = 0; 
	c.a = v->dx * v->dx + v->dy * v->dy + v->dz * v->dz;
	c.b = 2 * (v->dx * c.x0 + v->dy * c.y0 + v->dz * c.z0);
	c.c = c.x0 * c.x0 + c.y0 * c.y0 + c.z0 * c.z0 - sphere->radius * sphere->radius;

	c.Δ = c.b * c.b - 4 * c.a * c.c;

// if a = 0, the view_vector only touch the sphere
// if Δ < 0, the view_vector doesnt colide the sphere
	if (c.Δ < 0.0 || !c.a)
		return (-1.0);

	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);
	c.dist = h_smalest_Δ(c.det1, c.det2);

// if Δ < 0, the view_vector touch the sphere but behind
	if (c.dist <= 0.0)
		return (-1.0);


	if (c.dist < calc->dist || calc->dist < 0.0)
	{
		// calc->inter_point.x = data->camera[0]->xyz.x + v->dx * c.dist;
		// calc->inter_point.y = data->camera[0]->xyz.y + v->dy * c.dist;
		// calc->inter_point.z = data->camera[0]->xyz.z + v->dz * c.dist;
		calc->inter_point = (t_coor){data->camera[0]->xyz.x + v->dx * c.dist
			, data->camera[0]->xyz.y + v->dy * c.dist
			, data->camera[0]->xyz.z + v->dz * c.dist};
			
		calc->dist = c.dist;
		calc->px_color = sphere->color;
		calc->vect_norm = (t_vect){calc->inter_point.x - sphere->xyz.x
			, calc->inter_point.y - sphere->xyz.y
			, calc->inter_point.z - sphere->xyz.z};
	}
	return (c.dist);
}

///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
double	distance_from_plane(t_data *data, t_calcul *calc, t_vect *v, t_plane *p)
{
	double	top;
	double	bot;
	double	dist;
	t_coor	xyz;

	xyz = data->camera[0]->xyz;

// RESOLVE A(t.Vx + EYEx) + B(t.Vy + EYEy) + C(t.Vz + EYEz) + D = 0
// ==> t = top / bot; 
	top = -(p->abc.dx * xyz.x + p->abc.dy * xyz.y + p->abc.dz * xyz.z + p->d);
	bot = p->abc.dx * v->dx + p->abc.dy * v->dy + p->abc.dz * v->dz;

// if top = 0, the camera is on the plane
// if bot = 0, the view_vector is parallele to the plane
	if (!top || !bot)
		return (-1.0);

	dist = top / bot;
// if dist < 0, the view_vector touch the sphere but behind
	if (dist < 0)
		return (-1.0);

	calc->inter_point.x = xyz.x + v->dx * dist;
	calc->inter_point.y = xyz.y + v->dy * dist;
	calc->inter_point.z = xyz.z + v->dz * dist;


	if (dist < calc->dist || calc->dist < 0)
	{
		calc->inter_point = (t_coor){calc->inter_point.x = xyz.x + v->dx * dist
			, calc->inter_point.y = xyz.y + v->dy * dist
			, calc->inter_point.z = xyz.z + v->dz * dist};
			
		calc->dist = dist;
		calc->px_color = p->color;
		calc->vect_norm = p->abc;
	}
	return (dist);
}

///////////////////////////////////////////////////////////////////////////////]
// 	return smalest positive
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