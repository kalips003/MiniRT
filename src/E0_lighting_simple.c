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

void		ft_lighting_simple(t_data *data, t_c_px *c);
static int	ft_diffuse_simple(t_data *data, t_c_px *c, t_light *lights);
static int	something_block_the_light_simple(t_data *data, t_c_px *c);

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
static int	ft_diffuse_simple(t_data *data, t_c_px *c, t_light *lights)
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
	c->diffuse.x += c->mat.argb.r * lights->rgb.r / 255.0 * adj_i;
	c->diffuse.y += c->mat.argb.g * lights->rgb.g / 255.0 * adj_i;
	c->diffuse.z += c->mat.argb.b * lights->rgb.b / 255.0 * adj_i;
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
static int	something_block_the_light_simple(t_data *data, t_c_px *c)
{
	t_c_px	calcul;
	int		r;

	calcul.c0 = new_moved_point(&c->inter, &c->vn, EPSILON);
	calcul.v = c->v_light;
	calcul.dist = c->dist_light;
	calcul.print = c->print + !!(c->print);
	r = find_coli(data, &calcul, 1, 0);
	return (r);
}
