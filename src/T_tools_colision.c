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

double	distance_from_sphere(t_data *data, t_calcul *calc, t_sphere *sphere);
double	distance_from_plane(t_data *data, t_calcul *calc, t_plane *p);
double	distance_from_cylinder(t_data *data, t_calcul *calc, t_cylinder *cy);
static double h_smalest_Δ(double a, double b);

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
double	distance_from_sphere(t_data *data, t_calcul *calc, t_sphere *sphere)
{
	t_sphere_calc	c;

//	diff center sphere and center camera
	c.x0 = calc->origin.x - sphere->xyz.x;
	c.y0 = calc->origin.y - sphere->xyz.y;
	c.z0 = calc->origin.z - sphere->xyz.z;

// RESOLVE ((t.Vx + EYEx) - x0)² + ((t.Vy + EYEy) - y0)² + ((t.Vz + EYEz) - z0)² = R²
// ==> At² + Bt + C = 0; 
	c.a = calc->v_rotated.dx * calc->v_rotated.dx + calc->v_rotated.dy * calc->v_rotated.dy + calc->v_rotated.dz * calc->v_rotated.dz;
	c.b = 2 * (calc->v_rotated.dx * c.x0 + calc->v_rotated.dy * c.y0 + calc->v_rotated.dz * c.z0);
	c.c = c.x0 * c.x0 + c.y0 * c.y0 + c.z0 * c.z0 - sphere->radius * sphere->radius;

	c.Δ = c.b * c.b - 4 * c.a * c.c;

// if a = 0, the view_vector only touch the sphere
// if Δ < 0, the view_vector doesnt colide the sphere
	if (c.Δ < 0.0 || !c.a)
		return (-1.0);

	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);
	//  what if in the sphere (light source also in)
	// if (c.det1 < 0.0 || c.det2 < 0.0)
	// 	return (-1.0);
	// c.dist = fmin(c.det1, c.det2);
	c.dist = h_smalest_Δ(c.det1, c.det2);
// if c.dist < 0, the view_vector touch the sphere but behind
	if (c.dist <= 0.0)
		return (-1.0);


	if (c.dist < calc->dist || calc->dist < 0.0)
	{
		calc->inter_point = (t_coor){calc->origin.x + calc->v_rotated.dx * c.dist
			, calc->origin.y + calc->v_rotated.dy * c.dist
			, calc->origin.z + calc->v_rotated.dz * c.dist};
			
		calc->dist = c.dist;
		calc->px_color = sphere->color;
		calc->vect_norm = (t_vect){calc->inter_point.x - sphere->xyz.x
			, calc->inter_point.y - sphere->xyz.y
			, calc->inter_point.z - sphere->xyz.z};
		ft_normalize_vect(&calc->vect_norm);
	}
	return (c.dist);
}

///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
double	distance_from_plane(t_data *data, t_calcul *calc, t_plane *p)
{
	double	top;
	double	bot;
	double	dist;
	t_coor	xyz;

	// xyz = data->camera[0]->xyz;
	xyz = calc->origin;

// RESOLVE A(t.Vx + EYEx) + B(t.Vy + EYEy) + C(t.Vz + EYEz) + D = 0
// ==> t = top / bot; 
	top = -(p->abc.dx * xyz.x + p->abc.dy * xyz.y + p->abc.dz * xyz.z + p->d);
	bot = p->abc.dx * calc->v_rotated.dx + p->abc.dy * calc->v_rotated.dy + p->abc.dz * calc->v_rotated.dz;

// if top = 0, the camera is on the plane
// if bot = 0, the view_vector is parallele to the plane
	if (!top || !bot)
		return (-1.0);

	dist = top / bot;
// if dist < 0, the view_vector touch the sphere but behind
	if (dist <= 0)
		return (-1.0);

	if (dist < calc->dist || calc->dist < 0)
	{
		calc->inter_point = (t_coor){xyz.x + calc->v_rotated.dx * dist
			, xyz.y + calc->v_rotated.dy * dist
			, xyz.z + calc->v_rotated.dz * dist};
			
		calc->dist = dist;
		calc->px_color = p->color;
		if (ft_vect_dot_product(&calc->v_rotated, &p->abc) < 0.0)
			calc->vect_norm = p->abc;
		else
			calc->vect_norm = (t_vect){-p->abc.dx, -p->abc.dy, -p->abc.dz};
		ft_normalize_vect(&calc->vect_norm);
	}
	return (dist);
}

// ||(P - E) - ((P - E).W) * W||² = R²
double	distance_from_cylinder(t_data *data, t_calcul *calc, t_cylinder *cy)
{
	t_vect	*vect_c = &calc->v_rotated;
	t_coor	*Camera0 = &calc->origin;

	t_vect	*vect_cy = &cy->abc;
	t_coor	*Cy0 = &cy->xyz;

	double	radius = cy->diameter / 2;
	double	h = cy->height;

	// (P - E).W = At + B
	double	A = vect_c->dx * vect_cy->dx + vect_c->dy * vect_cy->dy + vect_c->dz * vect_cy->dz;
	double	B = vect_cy->dx * (Camera0->x - Cy0->x) + vect_cy->dy * (Camera0->y - Cy0->y) + vect_cy->dz * (Camera0->z - Cy0->z);
	
	// (P - E) - ((P - E).W) * W = {X0t + X1, Y0t + Y1, Z0t + Z1};
	double x0 = vect_c->dx - A * vect_cy->dx;
	double y0 = vect_c->dy - A * vect_cy->dy;
	double z0 = vect_c->dz - A * vect_cy->dz;
	double x1 = Camera0->x - B * vect_cy->dx - Cy0->x;
	double y1 = Camera0->y - B * vect_cy->dy - Cy0->y;
	double z1 = Camera0->z - B * vect_cy->dz - Cy0->z;

	double a = x0 * x0 + y0 * y0 + z0 * z0;
	double b = 2 * (x0 * x1 + y0 * y1 + z0 * z1);
	double c = x1 * x1 + y1 * y1 + z1 * z1 - radius * radius;

	double Δ = b * b - 4 * a * c;
	if (Δ <= 0.0 || !a)
		return (-1.0);

	double det1 = (-b + sqrt(Δ)) / (2 * a);
	double det2 = (-b - sqrt(Δ)) / (2 * a);
	double small_posi_det = h_smalest_Δ(det1, det2);
	double dist_h1 = A * det1 + B;
	double dist_h2 = A * det2 + B;
	double dist_small_dist = A * small_posi_det + B;


	t_vect temp_vect_norm;
	t_coor temp_inter_point;
	if (det1 < 0.0 && det2 < 0.0)//cylinder behind camera
		return (-1.0);
	if ((dist_h1 > h && dist_h2 > h) || (dist_h1 < 0.0 && dist_h2 < 0.0))// hit the cylinder but outside of bounds
		return (-1.0);
	// if here at least 1 det is positive, either dist_h1<0 & dist_h2>h || 
	if (dist_h1 < h && dist_h2 < h && dist_h1 > 0.0 && dist_h2 > 0.0)// 2 colision inside range
	{
		temp_inter_point = (t_coor){
			Camera0->x + small_posi_det * vect_c->dx,
			Camera0->y + small_posi_det * vect_c->dy,
			Camera0->z + small_posi_det * vect_c->dz
		};
		temp_vect_norm = (t_vect){
			temp_inter_point.x - (Cy0->x + dist_small_dist * vect_cy->dx),
			temp_inter_point.y - (Cy0->y + dist_small_dist * vect_cy->dy),
			temp_inter_point.z - (Cy0->z + dist_small_dist * vect_cy->dz)
		};
	}//calculate colision for closest det
	else// we see the circle, calculate dot product of vector camera and cy_vector to know what end we see
	{
		
	}

	if (det1 < 0.0 || det2 < 0.0)
		;//inside cylinder, vect norm = negative 





	if (dist < dist)
	t_coor projec_point = (t_coor){Cy0->x + dist_h * vect_cy->dx, Cy0->y + dist_h * vect_cy->dy, Cy0->z + dist_h * vect_cy->dz};
	// double A = calc->v_rotated.dx * cy->abc.dx + calc->v_rotated.dy * cy->abc.dy + calc->v_rotated.dz * cy->abc.dz;
	// double B = cy->abc.dx * (calc->origin.x - cy->xyz.x) + cy->abc.dy * (calc->origin.y - cy->xyz.y) + cy->abc.dz * (calc->origin.z - cy->xyz.z);

	// c.a = (calc->v_rotated.dx - A * cy->abc.dx) * (calc->v_rotated.dx - A * cy->abc.dx) + (calc->v_rotated.dy - A * cy->abc.dy) * (calc->v_rotated.dy - A * cy->abc.dy) + (calc->v_rotated.dz - A * cy->abc.dz) * (calc->v_rotated.dz - A * cy->abc.dz);
	// c.b = 2 * ((calc->v_rotated.dx - A * cy->abc.dx) * (calc->origin.x - B * cy->abc.dx) + (calc->v_rotated.dy - A * cy->abc.dy) * (calc->origin.y - B * cy->abc.dy) + (calc->v_rotated.dz - A * cy->abc.dz) * (calc->origin.z - B * cy->abc.dz));
	// c.c = (calc->origin.x - B * cy->abc.dx) * (calc->origin.x - B * cy->abc.dx) + (calc->origin.y - B * cy->abc.dy) * (calc->origin.y - B * cy->abc.dy) + (calc->origin.z - B * cy->abc.dz) * (calc->origin.z - B * cy->abc.dz) - (cy->diameter / 2) * (cy->diameter / 2);

// 	c.Δ = c.b * c.b - 4 * c.a * c.c;
// 	// if a = 0?
// 	// if Δ < 0, the view_vector doesnt colide the sphere
// 	if (c.Δ < 0.0 || !c.a)
// 		return (-1.0);

// 	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
// 	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);
// 	//  what if in the sphere (light source also in)
// 	// if (c.det1 < 0.0 || c.det2 < 0.0)
// 	// 	return (-1.0);
// 	// c.dist = fmin(c.det1, c.det2);
// 	c.dist = h_smalest_Δ(c.det1, c.det2);
// 	c.x0 = A * c.dist + B;
// // if c.dist < 0, the view_vector touch the sphere but behind
// 	if (c.dist <= 0.0 || c.x0 < 0 || c.x0 > cy->height)
// 		return (-1.0);





	if (dist < calc->dist || calc->dist < 0.0)
	{
		calc->inter_point = (t_coor){calc->origin.x + calc->v_rotated.dx * dist
			, calc->origin.y + calc->v_rotated.dy * dist
			, calc->origin.z + calc->v_rotated.dz * dist};
			
		calc->dist = dist;
		calc->px_color = cy->color; //(t_rgb){0,0,0};
		calc->vect_norm = (t_vect){calc->inter_point.x - projec_point.x
			, calc->inter_point.y - projec_point.y
			, calc->inter_point.z - projec_point.z};
		ft_normalize_vect(&calc->vect_norm);
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