/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_obj_shadows.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/12/14 20:28:49 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	in_shadow_of_sphere(t_calcul_px *calcul, t_sphere *sphere);
int	in_shadow_of_plane(t_calcul_px *calcul, t_plane *p);
int	in_shadow_of_cicle(t_calcul_px *calcul, t_circle circle);
int	in_shadow_of_cylinder(t_calcul_px *calcul, t_cylinder *cy);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// 		if colition, fills in the xyz position of the closest positive contact point
// 	return dist from sphere, -1 if no colosion or not in sight
//	(x-x0)² + (y-y0)² + (z-z0)² = R²
int	in_shadow_of_sphere(t_calcul_px *calcul, t_sphere *sphere)
{
	t_sphere_calc	c;

//	diff center sphere and center camera
	c.x0 = calcul->c0.x - sphere->c0.x;
	c.y0 = calcul->c0.y - sphere->c0.y;
	c.z0 = calcul->c0.z - sphere->c0.z;

// RESOLVE ((t.Vx + EYEx) - x0)² + ((t.Vy + EYEy) - y0)² + ((t.Vz + EYEz) - z0)² = R²
// ==> At² + Bt + C = 0; 
	c.a = calcul->v_view.dx * calcul->v_view.dx + calcul->v_view.dy * calcul->v_view.dy + calcul->v_view.dz * calcul->v_view.dz;
	c.b = 2 * (calcul->v_view.dx * c.x0 + calcul->v_view.dy * c.y0 + calcul->v_view.dz * c.z0);
	c.c = c.x0 * c.x0 + c.y0 * c.y0 + c.z0 * c.z0 - sphere->radius * sphere->radius;

	c.Δ = c.b * c.b - 4 * c.a * c.c;

// if Δ = 0, the view_vector only touch the sphere
// if Δ < 0, the view_vector doesnt colide the sphere
	// if (c.Δ < 0.0 || c.a == 0.0)
	if (c.Δ < EPSILON || fabs(c.a) < EPSILON)
		return (0);

	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);

	c.dist = h_smalest_Δ(c.det1, c.det2);
// if c.dist < 0, the view_vector touch the sphere but behind
	if (c.dist <= EPSILON || c.dist > calcul->dist)
		return (0);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
int	in_shadow_of_plane(t_calcul_px *calcul, t_plane *p)
{
	t_plane_calc	c;

// RESOLVE A(t.Vx + EYEx) + B(t.Vy + EYEy) + C(t.Vz + EYEz) + D = 0
// ==> t = top / bot;
	c.top = -(p->abc.dx * calcul->c0.x + p->abc.dy * calcul->c0.y + p->abc.dz * calcul->c0.z + p->d);
	c.bot = p->abc.dx * calcul->v_view.dx + p->abc.dy * calcul->v_view.dy + p->abc.dz * calcul->v_view.dz;

// if top = 0, the camera is on the plane
// if bot = 0, the view_vector is parallele to the plane
	if (fabs(c.top) < EPSILON || fabs(c.bot) < EPSILON)
		return (0);

	c.dist = c.top / c.bot;
// if dist < 0, the view_vector touch the plane but behind
	if (c.dist < EPSILON || c.dist > calcul->dist)
		return (0);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
int	in_shadow_of_cicle(t_calcul_px *calcul, t_circle circle)
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
		return (0);

	c.dist = c.top / c.bot;
// if dist < 0, the view_vector touch the sphere but behind
	if (c.dist < EPSILON || c.dist > calcul->dist)
		return (0);
	inter_temp = (t_coor){
			calcul->c0.x + calcul->v_view.dx * c.dist,
			calcul->c0.y + calcul->v_view.dy * c.dist,
			calcul->c0.z + calcul->v_view.dz * c.dist};

	if (dist_two_points(&inter_temp, &circle.c0) > circle.radius)
		return (0);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// ||(P - E) - ((P - E).W) * W||² = R²
int	in_shadow_of_cylinder(t_calcul_px *calcul, t_cylinder *cy)
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
		return (0);

	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);
	c.dist = h_smalest_Δ(c.det1, c.det2);
	c.dist_h = c.A * c.dist + c.B;

	if (c.dist < EPSILON || c.dist > calcul->dist || c.dist_h > cy->height || c.dist_h < 0.0)//cylinder behind camera  // hit the cylinder but outside of bounds
		return (0);
	return (1);
}
