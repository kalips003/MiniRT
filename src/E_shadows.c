/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_shadows.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/01/14 11:14:55 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	ft_handle_shadows_plus(t_data *data, t_calcul_px *c);
void	ft_handle_shadows_v2(t_data *data, t_calcul_px *c);
t_coor	ft_ambient(t_data *data, t_calcul_px *c);
void	ft_diffuse_and_reflected(t_data *data, t_calcul_px *c, t_light *light);
t_coor	ft_diffuse(t_data *data, t_calcul_px *c);
t_coor	ft_reflected(t_data *data, t_calcul_px *c);

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
void	ft_handle_shadows_plus(t_data *data, t_calcul_px *c)
{
	t_coor ambient;

	ambient = ft_ambient(data, c);
	// diffuse = ft_diffuse(data, c);


	t_coor	diffuse;
	t_coor	reflected;
	t_light	**lights;
	double cos_angle;
	double adjusted_intensity;
	
	lights = data->light_source - 1;
	memset(&diffuse, 0 , sizeof(t_coor));

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

		t_vect reflected_view = c->v_view;
		double	dot_pro = 2.0 * ft_vect_dot_product(&c->v_view, &c->v_normal);
		reflected_view.dx -= dot_pro * c->v_normal.dx;
		reflected_view.dy -= dot_pro * c->v_normal.dy;
		reflected_view.dz -= dot_pro * c->v_normal.dz;
		ft_normalize_vect(&reflected_view);
		double	cos_angle2 = c->v_light.dx * reflected_view.dx + c->v_light.dy * reflected_view.dy + c->v_light.dz * reflected_view.dz;
		cos_angle2 = fmax(-1.0, fmin(1.0, cos_angle2));
		if (cos_angle2 < 0.0)
			continue ;
		adjusted_intensity = SCALAR_REFLECTION * (*lights)->ratio * pow(cos_angle2, SCALAR_SHINY);
		// adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
		diffuse.x += (*lights)->color.r * adjusted_intensity;
		diffuse.y += (*lights)->color.g * adjusted_intensity;
		diffuse.z += (*lights)->color.b * adjusted_intensity;
	}

	// t_light	**lights;
	// lights = data->light_source - 1;
	// while (lights && *(++lights))
	// {
	// 	c->cos_angle = calculate_light_angle(&c->inter, &(*lights)->xyz, &c->v_normal);
	// 	if (c->cos_angle < 0.0 || something_block_the_light(data, c, *lights))
	// 		continue ;
	// 	ft_diffuse_and_reflected(data, c, *lights);
	// }


	c->px_color.r = fmax(0, fmin(255, round(ambient.x + diffuse.x)));
	c->px_color.g = fmax(0, fmin(255, round(ambient.y + diffuse.y)));
	c->px_color.b = fmax(0, fmin(255, round(ambient.z + diffuse.z)));

}

///////////////////////////////////////////////////////////////////////////////]
void	ft_handle_shadows_v2(t_data *data, t_calcul_px *c)
{
	t_coor ambient;
	t_coor diffuse;
	t_coor reflected;
	
	ambient = ft_ambient(data, c);
	diffuse = ft_diffuse(data, c);
	reflected = ft_reflected(data, c);
	
	c->px_color.r = fmax(0, fmin(255, round(ambient.x + diffuse.x + reflected.x)));
	c->px_color.g = fmax(0, fmin(255, round(ambient.y + diffuse.y + reflected.y)));
	c->px_color.b = fmax(0, fmin(255, round(ambient.z + diffuse.z + reflected.z)));

}

///////////////////////////////////////////////////////////////////////////////]///////////////////////////////////////////////////////////////////////////////]
t_coor	ft_ambient(t_data *data, t_calcul_px *c)
{
	t_coor	color_ambient;

// HANDLE HERE PARTIAL OCCLUSION
	color_ambient.x = c->px_color.r * data->bg_light[0]->color.r / 255.0 * data->bg_light[0]->ratio;
	color_ambient.y = c->px_color.g * data->bg_light[0]->color.g / 255.0 * data->bg_light[0]->ratio;
	color_ambient.z = c->px_color.b * data->bg_light[0]->color.b / 255.0 * data->bg_light[0]->ratio;

	return (color_ambient);
}

///////////////////////////////////////////////////////////////////////////////]
// R = V - 2( V⋅N ) N
void	ft_diffuse_and_reflected(t_data *data, t_calcul_px *c, t_light *light)
{
	double adjusted_intensity;

// DIFFUSE
	adjusted_intensity = light->ratio * c->cos_angle;
	adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
	c->tmp_color.x += c->px_color.r * light->color.r / 255.0 * adjusted_intensity;
	c->tmp_color.y += c->px_color.g * light->color.g / 255.0 * adjusted_intensity;
	c->tmp_color.z += c->px_color.b * light->color.b / 255.0 * adjusted_intensity;
// REFLECTED (SPECULAR)
	t_vect reflected_view = c->v_view;
	double	dot_pro = 2.0 * ft_vect_dot_product(&c->v_view, &c->v_normal);
	reflected_view.dx -= dot_pro * c->v_normal.dx;
	reflected_view.dy -= dot_pro * c->v_normal.dy;
	reflected_view.dz -= dot_pro * c->v_normal.dz;
	ft_normalize_vect(&reflected_view);
	double	cos_angle2 = c->v_light.dx * reflected_view.dx + c->v_light.dy * reflected_view.dy + c->v_light.dz * reflected_view.dz;
	cos_angle2 = fmax(-1.0, fmin(1.0, cos_angle2));
	if (cos_angle2 < 0.0)
		return ;
	adjusted_intensity = light->ratio * pow(cos_angle2, SCALAR_SHINY);
	c->tmp_color.x += light->color.r / 255.0 * adjusted_intensity;
	c->tmp_color.y += light->color.g / 255.0 * adjusted_intensity;
	c->tmp_color.z += light->color.b / 255.0 * adjusted_intensity;

}

///////////////////////////////////////////////////////////////////////////////]
t_coor	ft_diffuse(t_data *data, t_calcul_px *c)
{
	t_coor	color_diffuse;
	t_light	**lights;
	double cos_angle;
	double adjusted_intensity;
	
	lights = data->light_source - 1;
	memset(&color_diffuse, 0 , sizeof(t_coor));

	while (lights && *(++lights))
	{
		cos_angle = calculate_light_angle(&c->inter, &(*lights)->xyz, &c->v_normal);
		if (cos_angle < 0.0 || something_block_the_light(data, c, *lights))
			continue ;
		adjusted_intensity = (*lights)->ratio * cos_angle;
		adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (c->dist_light * c->dist_light);
		color_diffuse.x += c->px_color.r * (*lights)->color.r / 255.0 * adjusted_intensity;
		color_diffuse.y += c->px_color.g * (*lights)->color.g / 255.0 * adjusted_intensity;
		color_diffuse.z += c->px_color.b * (*lights)->color.b / 255.0 * adjusted_intensity;
	}
	// color_diffuse.x = fmin(255.0, color_diffuse.x);
	// color_diffuse.y = fmin(255.0, color_diffuse.y);
	// color_diffuse.z = fmin(255.0, color_diffuse.z);

	return (color_diffuse);
}

t_coor	ft_reflected(t_data *data, t_calcul_px *c)
{
	t_coor	color_diffuse;

	memset(&color_diffuse, 0 , sizeof(t_coor));

	return (color_diffuse);
}
