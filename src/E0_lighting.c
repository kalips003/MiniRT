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

///////////////////////////////////////////////////////////////////////////////]
void	ft_lighting_simple(t_data *data, t_c_px *c);
void	ft_lighting(t_data *data, t_c_px *c, int (*f_shadow)(t_data*, t_c_px*));
int		shadow_tracing(t_data *data, t_c_px *calcul);
int		ft_diffuse_simple(t_data *data, t_c_px *c, t_light *lights);
int		something_block_the_light_simple(t_data *data, t_c_px *c);

///////////////////////////////////////////////////////////////////////////////]
void	ft_lighting_simple(t_data *data, t_c_px *c)
{
	t_light	**lights;

	if (c->object->param.light > EPSILON)
	{
		c->mat.argb = scale_argb(c->mat.argb, c->object->param.light);
		return ;
	}
	c->diffuse = ft_ambient(data, c);
	lights = data->light - 1;
	while (++lights && *lights)
	{
		if (!ft_diffuse_simple(data, c, *lights))
			continue ;
	}
	c->mat.argb.r = fmax(0, fmin(255, round(c->diffuse.x)));
	c->mat.argb.g = fmax(0, fmin(255, round(c->diffuse.y)));
	c->mat.argb.b = fmax(0, fmin(255, round(c->diffuse.z)));
}

///////////////////////////////////////////////////////////////////////////////]
void	ft_lighting(t_data *data, t_c_px *c, int (*f_shadow)(t_data*, t_c_px*))
{
	t_light	**lights;
	t_coor	final;

	if (c->object->param.light > EPSILON)
		return (c->mat.argb = scale_argb(c->mat.argb, c->object->param.light), \
			(void)0);
	c->diffuse = ft_ambient(data, c);
	c->specular = (t_coor){0.0, 0.0, 0.0};
	lights = data->light - 1;
	while (++lights && *lights)
	{
		if (!ft_diffuse(data, c, *lights, f_shadow))
			continue ;
		ft_specular(c);
	}
	ft_refracted(data, c);
	ft_reflected(data, c);
	final = (t_coor){
		c->r_diff * c->diffuse.x + c->specular.x + c->reflected.x + c->behind.x,
		c->r_diff * c->diffuse.y + c->specular.y + c->reflected.y + c->behind.y,
		c->r_diff * c->diffuse.z + c->specular.z + c->reflected.z + c->behind.z
	};
	c->mat.argb.r = fmax(0, fmin(255, round(final.x)));
	c->mat.argb.g = fmax(0, fmin(255, round(final.y)));
	c->mat.argb.b = fmax(0, fmin(255, round(final.z)));
}

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
int	shadow_tracing(t_data *data, t_c_px *calcul)
{
	t_c_px		c;
	t_coor		rgb;
	double		dist_l;
	double		dot_l;
	int			b;

	c.c0 = new_moved_point(&calcul->inter, &calcul->vn, EPSILON);
	c.v = calcul->v_light;
	c.print = calcul->print + !!(calcul->print);
	rgb = (t_coor){calcul->eff_light.color.r, calcul->eff_light.color.g, calcul->eff_light.color.b};
	dist_l = calcul->dist_light;
	c.dist = dist_l;
	dot_l = 0.0;
	b = 0;
	while (c.dist > EPSILON)
	{
		if (ft_find_pixel_colision(data, &c, 0, 0))
		{
			c.mat.tr = fmin(1.0, c.mat.argb.a / 255.0 + c.mat.tr);
			if (c.mat.tr < EPSILON)
				return (1);
			c.mat.tr = sqrt(c.mat.tr);
			calcul->eff_light.ratio *= c.mat.tr;
			rgb.x = rgb.x * c.mat.tr + c.mat.argb.r * (1.0 - c.mat.tr);
			rgb.y = rgb.y * c.mat.tr + c.mat.argb.g * (1.0 - c.mat.tr);
			rgb.z = rgb.z * c.mat.tr + c.mat.argb.b * (1.0 - c.mat.tr);
			c.c0 = new_moved_point(&c.inter, &c.vn, -EPSILON);
			dist_l -= c.dist;
			c.dist = dist_l;
			dot_l += acos(ft_dot_p(&calcul->v_light, &c.vn)) * (c.mat.gamma > 1.0 + EPSILON);
			b += (c.mat.gamma > 1.0 + EPSILON);
		}
		else
			break ;
	}
	if (b)
		calcul->eff_light.ratio *= FAKE_REFR_SCALAR * pow(cos(dot_l), FAKE_REFR_POW);
	calcul->eff_light.color.r = (int)min(255.0, max(0.0, floor(rgb.x)));
	calcul->eff_light.color.g = (int)min(255.0, max(0.0, floor(rgb.y)));
	calcul->eff_light.color.b = (int)min(255.0, max(0.0, floor(rgb.z)));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
///////////////////////////////////////////////////////////////////////////////]
int	ft_diffuse_simple(t_data *data, t_c_px *c, t_light *lights)
{
	double	adj_i;
	double	cos_angle;

	c->dist_light = dist_two_points(&c->inter, &lights->xyz);
	c->v_light = vect_ab_norm(&c->inter, &lights->xyz);
	cos_angle = ft_dot_p(&c->v_light, &c->vn);
	if (cos_angle < EPSILON || something_block_the_light_simple(data, c))
		return (0);
	adj_i = lights->ratio * cos_angle;
	adj_i = SCALAR_LIGHT_DIST * adj_i / (1 + c->dist_light * c->dist_light);
	c->diffuse.x += c->mat.argb.r * lights->color.r / 255.0 * adj_i;
	c->diffuse.y += c->mat.argb.g * lights->color.g / 255.0 * adj_i;
	c->diffuse.z += c->mat.argb.b * lights->color.b / 255.0 * adj_i;
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
int	something_block_the_light_simple(t_data *data, t_c_px *c)
{
	t_c_px	calcul;
	int		r;

	calcul.c0 = new_moved_point(&c->inter, &c->vn, EPSILON);
	calcul.v = c->v_light;
	calcul.dist = c->dist_light;
	calcul.print = c->print + !!(c->print);
	r = ft_find_pixel_colision(data, &calcul, 1, 0);
	return (r);
}
