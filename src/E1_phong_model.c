/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E1_phong_model.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/19 13:06:17 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_coor	ft_ambient(t_data *data, t_c_px *c);
int		ft_diffuse(t_data *data, t_c_px *c, t_light *light, int (*f_shadow)(t_data*, t_c_px*));
void	ft_reflected(t_data *data, t_c_px *c);
void	ft_refracted(t_data *data, t_c_px *c);
void	ft_specular(t_c_px *c);

///////////////////////////////////////////////////////////////////////////////]
t_coor	ft_ambient(t_data *data, t_c_px *c)
{
	t_coor			color_ambient;
	t_ambient_light	*l;

	l = data->bgl[0];

	color_ambient.x = c->mat.argb.r * l->rgb.r / 255.0 * l->ratio * c->mat.ao;
	color_ambient.y = c->mat.argb.g * l->rgb.g / 255.0 * l->ratio * c->mat.ao;
	color_ambient.z = c->mat.argb.b * l->rgb.b / 255.0 * l->ratio * c->mat.ao;

	return (color_ambient);
}

///////////////////////////////////////////////////////////////////////////////]
int	ft_diffuse(t_data *data, t_c_px *c, t_light *light, int (*f_shadow)(t_data*, t_c_px*))
{
	double	adjusted_intensity;
	double	cos_angle;

	c->dist_light = dist_two_points(&c->inter, &light->xyz);
	c->v_light = vect_ab_norm(&c->inter, &light->xyz);
	cos_angle = ft_dot_p(&c->v_light, &c->vn);
	c->eff_light = *light;
	if (cos_angle < EPSILON || f_shadow(data, c))
		return (0);
	adjusted_intensity = c->eff_light.ratio * cos_angle;
	adjusted_intensity = SCALAR_LIGHT_DIST * adjusted_intensity / (1 + c->dist_light * c->dist_light);
	adjusted_intensity = fmin(adjusted_intensity, 2.5);
	c->diffuse.x += c->mat.argb.r * c->eff_light.color.r / 255.0 * adjusted_intensity;
	c->diffuse.y += c->mat.argb.g * c->eff_light.color.g / 255.0 * adjusted_intensity;
	c->diffuse.z += c->mat.argb.b * c->eff_light.color.b / 255.0 * adjusted_intensity;
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
void	ft_specular(t_c_px *c)
{
	double	adjusted_intensity;
	double	cos_angle;
	t_vect	reflected_light;

	reflected_light = ft_vect_reflected(&c->v_light, &c->vn);
	cos_angle = ft_dot_p(&c->v, &reflected_light);
	if (cos_angle < EPSILON)
		return ;
	adjusted_intensity = c->mat.sp * c->eff_light.ratio * pow(cos_angle, c->mat.sh);
	c->specular.x += c->eff_light.color.r * adjusted_intensity;
	c->specular.y += c->eff_light.color.g * adjusted_intensity;
	c->specular.z += c->eff_light.color.b * adjusted_intensity;
}

///////////////////////////////////////////////////////////////////////////////]
void	ft_refracted(t_data *data, t_c_px *c)
{
	t_argb	behind;

	c->mat.tr = fmin(1.0, c->mat.argb.a / 255.0 + c->mat.tr);
	if (c->transparence_depth >= MAX_REFRACTION_DEPTH)
		c->mat.tr = 0.0;
	if (c->mat.tr > EPSILON)
	{
		behind = what_is_behind(data, c);
		c->behind.x = behind.r * c->mat.tr;
		c->behind.y = behind.g * c->mat.tr;
		c->behind.z = behind.b * c->mat.tr;
	}
	else
	{
		c->behind.x = c->mat.argb.r * c->mat.tr;
		c->behind.y = c->mat.argb.g * c->mat.tr;
		c->behind.z = c->mat.argb.b * c->mat.tr;
	}
}

///////////////////////////////////////////////////////////////////////////////]
void	ft_reflected(t_data *data, t_c_px *c)
{
	t_argb	reflected;

	c->mat.mi = fmin(c->mat.mi, 1.0 - c->mat.tr);
	if (c->reflected_depth >= MAX_MIRROR_DEPTH)
		c->mat.mi = 0.0;
	if (c->mat.mi > EPSILON)
	{
		reflected = what_is_reflected(data, c);
		c->reflected.x = reflected.r * c->mat.mi;
		c->reflected.y = reflected.g * c->mat.mi;
		c->reflected.z = reflected.b * c->mat.mi;
	}
	else
	{
		c->reflected.x = c->mat.argb.r * c->mat.mi;
		c->reflected.y = c->mat.argb.g * c->mat.mi;
		c->reflected.z = c->mat.argb.b * c->mat.mi;
	}
	c->r_diff = fmax(0.0, 1.0 - c->mat.mi - c->mat.tr);
}
