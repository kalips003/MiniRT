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

void		ft_lighting(t_data *data, t_c_px *c, \
	int (*f_shadow)(t_data*, t_c_px*));
int			shadow_tracing(t_data *data, t_c_px *calcul);
static int	h_loop_1(t_data *data, t_c_px *calcul, t_c_px *c, t_coor *rgb);
static int	h_loop_2(t_c_px *calcul, t_c_px *c, t_coor *rgb, double *dbl);

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
int	shadow_tracing(t_data *data, t_c_px *calcul)
{
	t_c_px		c;
	t_coor		rgb;

	c.c0 = new_moved_point(&calcul->inter, &calcul->vn, EPSILON);
	c.print = calcul->print + !!(calcul->print);
	c.v = calcul->v_light;
	c.print = calcul->print + !!(calcul->print);
	rgb = (t_coor){calcul->eff_l.rgb.r, calcul->eff_l.rgb.g, \
		calcul->eff_l.rgb.b};
	c.dist = calcul->dist_light;
	if (h_loop_1(data, calcul, &c, &rgb))
		return (1);
	calcul->eff_l.rgb.r = (int)min(255.0, max(0.0, floor(rgb.x)));
	calcul->eff_l.rgb.g = (int)min(255.0, max(0.0, floor(rgb.y)));
	calcul->eff_l.rgb.b = (int)min(255.0, max(0.0, floor(rgb.z)));
	return (0);
}

// d: [ 0 ] distance light 
// p: [ 1 ] accumulation of dot product 
// t: [ 2 ] there was transparence
static int	h_loop_1(t_data *data, t_c_px *calcul, t_c_px *c, t_coor *rgb)
{
	double		dpt[3];

	dpt[0] = calcul->dist_light;
	dpt[1] = 0.0;
	dpt[2] = 0;
	while (c->dist > EPSILON)
	{
		if (find_coli(data, c, 0, 0))
		{
			if (h_loop_2(calcul, c, rgb, dpt))
				return (1);
		}
		else
			break ;
	}
	if (dpt[2])
		calcul->eff_l.ratio *= FAKE_L_REFR * pow(cos(dpt[1]), FAKE_REFR_POW);
	return (0);
}

// // dbl[0] = &dist_l
// // dbl[1] = &dot_l
// // dbl[2] = &b
static int	h_loop_2(t_c_px *calcul, t_c_px *c, t_coor *rgb, double *dbl)
{
	double	dot;

	c->mat.tr = fmin(1.0, c->mat.argb.a / 255.0 + c->mat.tr);
	if (c->mat.tr < EPSILON)
		return (1);
	c->mat.tr = sqrt(c->mat.tr);
	calcul->eff_l.ratio *= c->mat.tr;
	rgb->x = rgb->x * c->mat.tr + c->mat.argb.r * (1.0 - c->mat.tr);
	rgb->y = rgb->y * c->mat.tr + c->mat.argb.g * (1.0 - c->mat.tr);
	rgb->z = rgb->z * c->mat.tr + c->mat.argb.b * (1.0 - c->mat.tr);
	c->c0 = new_moved_point(&c->inter, &c->vn, -EPSILON);
	dbl[0] -= c->dist;
	c->dist = dbl[0];
	dot = ft_dot_p(&calcul->v_light, &c->vn);
	if (calcul->print == 1)
	{
		printf("dot: %.3f\n", dot);
		printf("calcul->v_light: [%.3f,%.3f,%.3f]\n", calcul->v_light.dx, calcul->v_light.dy, calcul->v_light.dz);
		printf("c->vn: [%.3f,%.3f,%.3f]\n", c->vn.dx, c->vn.dy, c->vn.dz);

	}
	if (!(dot < EPSILON -1.0))
	{
		dbl[1] += acos(ft_dot_p(&calcul->v_light, &c->vn)) * \
			(c->mat.gamma > 1.0 + EPSILON);
		dbl[2] += (c->mat.gamma > 1.0 + EPSILON);
	}
	return (0);
}
