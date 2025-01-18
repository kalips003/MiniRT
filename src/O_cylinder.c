/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   O_cylinder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/18 00:18:36 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

double	distance_from_cylinder_v2(t_calcul_px *calcul, t_cylinder *cy);
void	h_dist_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
t_rgb	ft_textures_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
t_vect	ft_nmap_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
// ||(P - E) - ((P - E).W) * W||² = R²
double	distance_from_cylinder_v2(t_calcul_px *calcul, t_cylinder *cy)
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
		h_dist_cylinder(calcul, cy, &c);
	return (c.dist);
}


void	h_dist_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c)
{
	calcul->dist = c->dist;
	calcul->object = (void *)cylinder;

	calcul->inter = (t_coor){
		calcul->c0.x + calcul->v_view.dx * c->dist,
		calcul->c0.y + calcul->v_view.dy * c->dist,
		calcul->c0.z + calcul->v_view.dz * c->dist};

	calcul->v_normal = (t_vect){
		calcul->inter.x - c->projec_point.x,
		calcul->inter.y - c->projec_point.y,
		calcul->inter.z - c->projec_point.z};
	ft_normalize_vect(&calcul->v_normal);
	
	if (cylinder->texture)
		calcul->px_color = ft_textures_cylinder(calcul, cylinder, c);
	else
		calcul->px_color = cylinder->color;

	if (cylinder->normal_map)
		calcul->v_normal = ft_nmap_cylinder(calcul, cylinder, c);

	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};

}

t_rgb	ft_textures_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c)
{
	t_camera	cy_cross;
	cy_cross.view = cylinder->v;
	h_camera_calc_up_right_vect(&cy_cross);
	
	t_img *texture = ((t_sphere*)calcul->object)->texture;

	double cosθ = ft_vect_dot_product(&calcul->v_normal, &cy_cross.up);
	double sinθ = ft_vect_dot_product(&calcul->v_normal, &cy_cross.right);
	double θ = atan2(sinθ, cosθ);
	double	l_θ = fmin(1.0, fmax(0.0, θ  / (2 * PI) + 0.5));
	int text_x = (int)(l_θ * texture->sz_x) % texture->sz_x;
	int text_y = (int)((c->dist_h / cylinder->height) * texture->sz_y) % texture->sz_y;

	char *pixel = texture->addr + (text_y * texture->ll + text_x * (texture->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_rgb	rtrn = {
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	};

	return (rtrn);
	
}

t_vect	ft_nmap_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c)
{
	t_camera	cy_cross;
	cy_cross.view = cylinder->v;
	h_camera_calc_up_right_vect(&cy_cross);
	
	t_img *texture = ((t_sphere*)calcul->object)->normal_map;

	double cosθ = ft_vect_dot_product(&calcul->v_normal, &cy_cross.up);
	double sinθ = ft_vect_dot_product(&calcul->v_normal, &cy_cross.right);
	double θ = atan2(sinθ, cosθ);
	double	l_θ = fmin(1.0, fmax(0.0, θ  / (2 * PI) + 0.5));
	int text_x = (int)(l_θ * texture->sz_x) % texture->sz_x;
	int text_y = (int)((c->dist_h / cylinder->height) * texture->sz_y) % texture->sz_y;

	char *pixel = texture->addr + (text_y * texture->ll + text_x * (texture->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_vect	normal_map = {
		((color >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((color >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		(color & 0xFF) / 255.0 * 2.0 - 1.0};
	ft_normalize_vect(&normal_map);

	t_camera	x;
	x.view = calcul->v_normal;
	x.up = cylinder->v;
	x.right = ft_vect_cross_product(&x.view, &x.up);
	ft_normalize_vect(&x.right);

	t_vect world_normal;

	// 	[NTB].[normal_map]
	// world_normal.dx = normal_map.dx * x.view.dx + normal_map.dy * x.up.dx + normal_map.dz * x.right.dx;
	// world_normal.dy = normal_map.dx * x.view.dy + normal_map.dy * x.up.dy + normal_map.dz * x.right.dy;
	// world_normal.dz = normal_map.dx * x.view.dz + normal_map.dy * x.up.dz + normal_map.dz * x.right.dz;
	world_normal.dx = normal_map.dx * x.view.dx + normal_map.dy * x.up.dx + normal_map.dz * x.right.dx;
	world_normal.dy = normal_map.dx * x.view.dy + normal_map.dy * x.up.dy + normal_map.dz * x.right.dy;
	world_normal.dz = normal_map.dx * x.view.dz + normal_map.dy * x.up.dz + normal_map.dz * x.right.dz;
	ft_normalize_vect(&world_normal);

	return (normal_map);
}