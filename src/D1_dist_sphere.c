/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D1_dist_sphere.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 13:10:23 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			distance_from_sphere(t_c_px *calcul, void *obj, int simple);
static int	h_dist_sphere(t_c_px *calcul, t_sphere *sphere, t_c_sphere *c, \
	int simple);
static void	h_img_sphere(t_c_px *calcul, t_sphere *sphere, t_c_sphere *c);

///////////////////////////////////////////////////////////////////////////////]
// 	if colition, fills in the xyz position of the closest positive contact point
// 	return dist from sphere, -1 if no colosion or not in sight
//	(x-x0)² + (y-y0)² + (z-z0)² = R²
int	distance_from_sphere(t_c_px *calcul, void *obj, int simple)
{
	t_c_sphere	c;
	t_sphere	*sphere;

	sphere = (t_sphere *)obj;
	c.x0 = calcul->c0.x - sphere->O.c0.x;
	c.y0 = calcul->c0.y - sphere->O.c0.y;
	c.z0 = calcul->c0.z - sphere->O.c0.z;
	c.a = ft_dot_p(&calcul->v, &calcul->v);
	c.b = 2 * ft_dot_p(&calcul->v, (t_vect *)&c.x0);
	c.c = c.x0 * c.x0 + c.y0 * c.y0 + c.z0 * c.z0 - sphere->r * sphere->r;
	c.delta = c.b * c.b - 4 * c.a * c.c;
	if (c.delta < EPSILON || fabs(c.a) < EPSILON)
		return (0);
	c.det1 = (-c.b + sqrt(c.delta)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.delta)) / (2 * c.a);
	c.dist = h_smalest_delta(c.det1, c.det2);
	if (c.dist < EPSILON)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0)
		return (h_dist_sphere(calcul, sphere, &c, simple));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
static int	h_dist_sphere(t_c_px *calcul, t_sphere *sphere, t_c_sphere *c, \
	int simple)
{
	if (simple)
		return (1);
	calcul->dist = c->dist;
	calcul->object = (t_obj2 *)sphere;
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, calcul->dist);
	calcul->vn = vect_ab_norm(&sphere->O.c0, &calcul->inter);
	calcul->mat = *(t_mat2 *)&sphere->param;
	if (sphere->param.c2.r >= 0)
		calcul->mat.argb = dual_color(&sphere->param.argb, &sphere->param.c2, \
			ft_dot_p(&calcul->vn, &sphere->O.view) / 2 + 0.5);
	c->inside = 0;
	if (c->det1 < EPSILON || c->det2 < EPSILON)
	{
		c->inside = 1;
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
	}
	if (is_there_txt(&sphere->param))
		h_img_sphere(calcul, sphere, c);
	return (1 + c->inside);
}

///////////////////////////////////////////////////////////////////////////////]
// 		ATAN2 [−π,π][−π,π] > [0,1].
// 		cosϕ=[1top,-1bot]	ACOS [0,π] > [0,1].
static void	h_img_sphere(t_c_px *ca, t_sphere *sphere, t_c_sphere *c)
{
	t_vect	normal_map;
	t_obj	local;
	int		in;

	in = 1 - 2 * c->inside;
	c->u = fmin(1.0, fmax(0.0, atan2(ft_dot_p(&ca->vn, &sphere->O.right) \
		* in, ft_dot_p(&ca->vn, &sphere->O.up) * in) / (2 * PI) + 0.5));
	c->v = fmin(1.0, fmax(0.0, acos(ft_dot_p(&ca->vn, &sphere->O.view) \
		* in) / PI));
	update_mat_w_txt(ca, (t_obj2 *)sphere, c->u, c->v);
	if (sphere->param.n_map)
	{
		normal_map = return_vect_img(sphere->param.n_map, c->u, c->v);
		local.view = ca->vn;
		local.right = ft_cross_product_norm(&sphere->O.view, &local.view);
		local.up = ft_cross_product_norm(&local.view, &local.right);
		normal_map.dx *= in;
		ca->vn = mult_3x3_vect(&local, &normal_map);
		ft_normalize_vect(&ca->vn);
	}
}
