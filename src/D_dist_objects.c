/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D_dist_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/16 14:44:30 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"


double	distance_from_plane(t_calcul_px *calcul, t_plane *p);
double	distance_from_cicle(t_calcul_px *calcul, t_circle circle);
double	distance_from_cylinder(t_calcul_px *calcul, t_cylinder *cy);


///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
double	distance_from_plane(t_calcul_px *calcul, t_plane *p)
{
	t_plane_calc	c;

// RESOLVE A(t.Vx + EYEx) + B(t.Vy + EYEy) + C(t.Vz + EYEz) + D = 0
// ==> t = top / bot;
	c.top = -(p->abc.dx * calcul->c0.x + p->abc.dy * calcul->c0.y + p->abc.dz * calcul->c0.z + p->d);
	c.bot = p->abc.dx * calcul->v_view.dx + p->abc.dy * calcul->v_view.dy + p->abc.dz * calcul->v_view.dz;

// if top = 0, the camera is on the plane
// if bot = 0, the view_vector is parallele to the plane
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (-1.0);

	c.dist = c.top / c.bot;
// if dist < 0, the view_vector touch the sphere but behind
	if (c.dist <= 0)
		return (-1.0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
	{
		calcul->dist = c.dist;
		calcul->px_color = p->color;

		calcul->inter = (t_coor){
			calcul->c0.x + calcul->v_view.dx * c.dist,
			calcul->c0.y + calcul->v_view.dy * c.dist,
			calcul->c0.z + calcul->v_view.dz * c.dist};
			
		calcul->v_normal = p->abc;
		if (ft_vect_dot_product(&calcul->v_view, &p->abc) > 0.0)
			calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
		// ft_normalize_vect(&calcul->v_normal);
		calcul->object = (void *)p;
	}
	return (c.dist);
}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
double	distance_from_cicle(t_calcul_px *calcul, t_circle circle)
{
	t_plane_calc	c;
	t_coor	inter_temp;
	double	d = -(circle.v.dx * circle.c0.x + circle.v.dy * circle.c0.y + circle.v.dz * circle.c0.z);

// RESOLVE A(t.Vx + EYEx) + B(t.Vy + EYEy) + C(t.Vz + EYEz) + D = 0
// ==> t = top / bot;
	c.top = -(circle.v.dx * calcul->c0.x + circle.v.dy * calcul->c0.y + circle.v.dz * calcul->c0.z + d);
	c.bot = circle.v.dx * calcul->v_view.dx + circle.v.dy * calcul->v_view.dy + circle.v.dz * calcul->v_view.dz;

// if top = 0, the camera is on the plane
// if bot = 0, the view_vector is parallele to d plane
	if (!c.top || !c.bot)
		return (-1.0);

	c.dist = c.top / c.bot;
// if dist < 0, the view_vector touch the sphere but behind
	if (c.dist <= 0)
		return (-1.0);
	inter_temp = (t_coor){
			calcul->c0.x + calcul->v_view.dx * c.dist,
			calcul->c0.y + calcul->v_view.dy * c.dist,
			calcul->c0.z + calcul->v_view.dz * c.dist};

	if (dist_two_points(&inter_temp, &circle.c0) > circle.radius)
		return (-1.0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
	{
		calcul->dist = c.dist;
		calcul->px_color = circle.color;

		calcul->inter = inter_temp;

		calcul->v_normal = circle.v;
		if (ft_vect_dot_product(&calcul->v_view, &circle.v) > 0.0)
			calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
		// ft_normalize_vect(&calcul->v_normal);
		calcul->object = (void *)(&circle);
	}
	return (c.dist);
}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// ||(P - E) - ((P - E).W) * W||² = R²
double	distance_from_cylinder(t_calcul_px *calcul, t_cylinder *cy)
{
	t_cylinder_calc	c;

	c.radius = cy->diameter / 2;
	// (P - E).W = At + B
	c.A = calcul->v_view.dx * cy->v.dx + calcul->v_view.dy * cy->v.dy + calcul->v_view.dz * cy->v.dz;
	c.B = cy->v.dx * (calcul->c0.x - cy->c0.x) + cy->v.dy * (calcul->c0.y - cy->c0.y) + cy->v.dz * (calcul->c0.z - cy->c0.z);
	
	// (P - E) - ((P - E).W) * W = {X0t + X1, Y0t + Y1, Z0t + Z1};
	c.x0 = calcul->v_view.dx - c.A * cy->v.dx;
	c.y0 = calcul->v_view.dy - c.A * cy->v.dy;
	c.z0 = calcul->v_view.dz - c.A * cy->v.dz;
	c.x1 = calcul->c0.x - c.B * cy->v.dx - cy->c0.x;
	c.y1 = calcul->c0.y - c.B * cy->v.dy - cy->c0.y;
	c.z1 = calcul->c0.z - c.B * cy->v.dz - cy->c0.z;

	c.a = c.x0 * c.x0 + c.y0 * c.y0 + c.z0 * c.z0;
	c.b = 2 * (c.x0 * c.x1 + c.y0 * c.y1 + c.z0 * c.z1);
	c.c = c.x1 * c.x1 + c.y1 * c.y1 + c.z1 * c.z1 - c.radius * c.radius;

	c.Δ = c.b * c.b - 4 * c.a * c.c;
	if (c.Δ < EPSILON || fabs(c.a) < EPSILON)
		return (-1.0);

	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);
	c.dist = h_smalest_Δ(c.det1, c.det2);
	c.dist_h = c.A * c.dist + c.B;

	if (c.dist < 0.0 || c.dist_h > cy->height || c.dist_h < 0.0)//cylinder behind camera  // hit the cylinder but outside of bounds
		return (-1.0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
	{
		calcul->inter = (t_coor){
			calcul->c0.x + calcul->v_view.dx * c.dist,
			calcul->c0.y + calcul->v_view.dy * c.dist,
			calcul->c0.z + calcul->v_view.dz * c.dist};

		calcul->dist = c.dist;
		calcul->px_color = cy->color; //(t_rgb){0,0,0};
		c.projec_point = (t_coor){cy->c0.x + c.dist_h * cy->v.dx, cy->c0.y + c.dist_h * cy->v.dy, cy->c0.z + c.dist_h * cy->v.dz};
		calcul->v_normal = (t_vect){
			calcul->inter.x - c.projec_point.x,
			calcul->inter.y - c.projec_point.y,
			calcul->inter.z - c.projec_point.z};
		if (c.det1 < 0.0 || c.det2 < 0.0)
			calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
		// ft_normalize_vect(&calcul->v_normal);
		calcul->object = (void *)cy;
	}
	return (c.dist);
}
