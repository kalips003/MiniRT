/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   O_sphere.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/16 16:49:42 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

double	distance_from_sphere(t_calcul_px *calcul, t_sphere *sphere);
double	distance_from_sphere_v2(t_calcul_px *calcul, t_sphere *sphere);
void	h_dist_sphere(t_calcul_px *calcul, t_sphere *sphere, double dist, int inside);
t_rgb	ft_txt_sphere(t_calcul_px *calcul);
t_vect	ft_nmap_sphere(t_calcul_px *calcul);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// 		if colition, fills in the xyz position of the closest positive contact point
// 	return dist from sphere, -1 if no colosion or not in sight
//	(x-x0)² + (y-y0)² + (z-z0)² = R²
double	distance_from_sphere(t_calcul_px *calcul, t_sphere *sphere)
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
	if (c.Δ < 0.0 || c.a == 0.0)
		return (-1.0);

	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);

	c.dist = h_smalest_Δ(c.det1, c.det2);
// if c.dist < 0, the view_vector touch the sphere but behind
	if (c.dist < 0.0)
		return (-1.0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
	{
		calcul->dist = c.dist;
		calcul->px_color = sphere->color;
	
		calcul->inter = (t_coor){
			calcul->c0.x + calcul->v_view.dx * c.dist, 
			calcul->c0.y + calcul->v_view.dy * c.dist, 
			calcul->c0.z + calcul->v_view.dz * c.dist};

		calcul->v_normal = (t_vect){
			calcul->inter.x - sphere->c0.x, 
			calcul->inter.y - sphere->c0.y, 
			calcul->inter.z - sphere->c0.z};
		// calcul->v_reflected
		if (c.det1 < 0.0 || c.det2 < 0.0)
			calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
		// ft_normalize_vect(&calcul->v_normal);
		calcul->object = (void *)sphere;
	}
	return (c.dist);
}


///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
double	distance_from_sphere_v2(t_calcul_px *calcul, t_sphere *sphere)
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
	if (c.Δ < 0.0 || c.a == 0.0)
		return (-1.0);

	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);

	c.dist = h_smalest_Δ(c.det1, c.det2);
// if c.dist < 0, the view_vector touch the sphere but behind
	if (c.dist < 0.0)
		return (-1.0);

	if (c.dist < calcul->dist || calcul->dist < 0.0)
		h_dist_sphere(calcul, sphere, c.dist, (c.det1 < 0.0 || c.det2 < 0.0));

	return (c.dist);
}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
void	h_dist_sphere(t_calcul_px *calcul, t_sphere *sphere, double dist, int inside)
{
	calcul->dist = dist;
	calcul->object = (void *)sphere;

	calcul->inter = (t_coor){
		calcul->c0.x + calcul->v_view.dx * dist, 
		calcul->c0.y + calcul->v_view.dy * dist, 
		calcul->c0.z + calcul->v_view.dz * dist};

	calcul->v_normal = (t_vect){
		calcul->inter.x - sphere->c0.x, 
		calcul->inter.y - sphere->c0.y, 
		calcul->inter.z - sphere->c0.z};
	ft_normalize_vect(&calcul->v_normal);
	if (sphere->texture)
		calcul->px_color = ft_txt_sphere(calcul);
	else
		calcul->px_color = sphere->color;

	if (sphere->normal_map)
		calcul->v_normal = ft_nmap_sphere(calcul);

	if (inside)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};

}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
t_rgb	ft_txt_sphere(t_calcul_px *calcul)
{
	//  [−π,π][−π,π] > [0,1].
	double	l_θ = fmin(1.0, fmax(0.0, atan2(calcul->v_normal.dz, calcul->v_normal.dx) / (2 * PI) + 0.5));
	// [0,π] > [0,1].
	double	l_ϕ = fmin(1.0, fmax(0.0, acos(calcul->v_normal.dy) / PI));

	t_img *texture = ((t_sphere*)calcul->object)->texture;

	int text_x = (int)(l_θ * texture->sz_x) % texture->sz_x;
	int text_y = (int)(l_ϕ * texture->sz_y) % texture->sz_y;
	char *pixel = texture->addr + (text_y * texture->ll + text_x * (texture->bpp / 8));
	int color = *(unsigned int *)pixel;
	// printf("l_θ = %f, l_ϕ= %f, color = %d\n", l_θ, l_ϕ, color);
	t_rgb	rtrn = {
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	};
	// printf("texture color = r:%d g:%d b:%d\n", rtrn.r, rtrn.g, rtrn.b);
	return (rtrn);
}

t_vect	ft_nmap_sphere(t_calcul_px *calcul)
{
	//  [−π,π][−π,π] > [0,1].
	double	l_θ = fmin(1.0, fmax(0.0, atan2(calcul->v_normal.dz, calcul->v_normal.dx) / (2 * PI) + 0.5));
	// [0,π] > [0,1].
	double	l_ϕ = fmin(1.0, fmax(0.0, acos(calcul->v_normal.dy) / PI));

	t_img *nmap = ((t_sphere*)calcul->object)->normal_map;
	int text_x = (int)(l_θ * nmap->sz_x) % nmap->sz_x;
	int text_y = (int)(l_ϕ * nmap->sz_y) % nmap->sz_y;
	char *pixel = nmap->addr + (text_y * nmap->ll + text_x * (nmap->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_vect	normal_map = {
		((color >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((color >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		(color & 0xFF) / 255.0 * 2.0 - 1.0};
	ft_normalize_vect(&normal_map);

	t_camera	x;
	x.view = calcul->v_normal;
	h_camera_calc_up_right_vect(&x);


	t_vect world_normal;

	// 	[NTB].[normal_map]
	world_normal.dx = normal_map.dx * x.right.dx + normal_map.dy * x.up.dx + normal_map.dz * x.view.dx;
	world_normal.dy = normal_map.dx * x.right.dy + normal_map.dy * x.up.dy + normal_map.dz * x.view.dy;
	world_normal.dz = normal_map.dx * x.right.dz + normal_map.dy * x.up.dz + normal_map.dz * x.view.dz;
	ft_normalize_vect(&world_normal);

	return (world_normal);
	
}

