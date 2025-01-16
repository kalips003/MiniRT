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

void	ft_handle_shadows_simple(t_data *data, t_calcul_px *c);

double	distance_from_cylinder_v2(t_calcul_px *calcul, t_cylinder *cy);
void	h_dist_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);
t_rgb	ft_textures_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c);

///////////////////////////////////////////////////////////////////////////////]
void	ft_handle_shadows_simple(t_data *data, t_calcul_px *c)
{
	t_coor ambient;
	t_coor	diffuse;
	t_light	**lights;
	double adjusted_intensity;
	
	ambient = ft_ambient(data, c);
	memset(&diffuse, 0 , sizeof(t_coor));

	lights = data->light_source - 1;
	while (lights && *(++lights))
	{
		c->cos_angle = calculate_light_angle(&c->inter, &(*lights)->xyz, &c->v_normal);
		if (c->cos_angle < 0.0 || something_block_the_light(data, c, *lights))
			continue ;
		adjusted_intensity = (*lights)->ratio * c->cos_angle;
		adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
		diffuse.x += c->px_color.r * (*lights)->color.r / 255.0 * adjusted_intensity;
		diffuse.y += c->px_color.g * (*lights)->color.g / 255.0 * adjusted_intensity;
		diffuse.z += c->px_color.b * (*lights)->color.b / 255.0 * adjusted_intensity;
	}

	c->px_color.r = fmax(0, fmin(255, round(ambient.x + diffuse.x)));
	c->px_color.g = fmax(0, fmin(255, round(ambient.y + diffuse.y)));
	c->px_color.b = fmax(0, fmin(255, round(ambient.z + diffuse.z)));

}


double	to_unit(char color)
{
	return ((color / 255.0) * 2.0 - 1.0);
}

t_vect	ft_cross_pproduct(t_vect *a, t_vect *b)
{
	t_vect	rtrn;


}

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
	calcul->inter = (t_coor){
		calcul->c0.x + calcul->v_view.dx * c->dist,
		calcul->c0.y + calcul->v_view.dy * c->dist,
		calcul->c0.z + calcul->v_view.dz * c->dist};

	calcul->dist = c->dist;
	calcul->px_color = cylinder->color;
	c->projec_point = (t_coor){cylinder->c0.x + c->dist_h * cylinder->v.dx, cylinder->c0.y + c->dist_h * cylinder->v.dy, cylinder->c0.z + c->dist_h * cylinder->v.dz};
	calcul->v_normal = (t_vect){
		calcul->inter.x - c->projec_point.x,
		calcul->inter.y - c->projec_point.y,
		calcul->inter.z - c->projec_point.z};
	ft_normalize_vect(&calcul->v_normal);

	// if (cylinder->texture || cylinder->normal_map)
	// 	ft_textures_cylinder();


	if (c->det1 < 0.0 || c->det2 < 0.0)
		calcul->v_normal = (t_vect){-calcul->v_normal.dx, -calcul->v_normal.dy, -calcul->v_normal.dz};
	calcul->object = (void *)cylinder;
}

t_rgb	ft_textures_cylinder(t_calcul_px *calcul, t_cylinder *cylinder, t_cylinder_calc *c)
{
	
	
	
	
	
	
	if (!((t_sphere*)calcul->object)->radius)
		return (((t_sphere*)calcul->object)->color);
	t_vect	normal = (t_vect){
		(calcul->inter.x - ((t_sphere*)calcul->object)->c0.x) / ((t_sphere*)calcul->object)->radius,
		(calcul->inter.y - ((t_sphere*)calcul->object)->c0.y) / ((t_sphere*)calcul->object)->radius,
		(calcul->inter.z - ((t_sphere*)calcul->object)->c0.z) / ((t_sphere*)calcul->object)->radius
	};
	//  [−π,π][−π,π] > [0,1].
	double	l_θ = min(1.0, max(0.0, atan2(normal.dz, normal.dx) / (2 * PI) + 0.5));
	// [0,π] > [0,1].
	double	l_ϕ = min(1.0, max(0.0, acos(normal.dy) / PI));

	t_img *texture = ((t_sphere*)calcul->object)->texture;

	int text_x = (int)(l_θ * texture->sz_x) % texture->sz_x;
	int text_y = (int)(l_ϕ * texture->sz_y) % texture->sz_y;
	char *pixel = texture->addr + (text_y * texture->ll + text_x * (texture->bpp / 8));
	int color = *(unsigned int *)pixel;

	t_rgb	rtrn = {
		(color >> 16) & 0xFF,
		(color >> 8) & 0xFF,
		color & 0xFF
	};

	return (rtrn);
	
}