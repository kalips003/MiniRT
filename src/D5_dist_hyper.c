/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D5_dist_hyper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 15:40:40 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			distance_from_hyper(t_c_px *calcul, void *obj, int simple);
static int	h_dist_para(t_c_px *calcul, t_hyper *hy, t_hyper_calc *c, \
	int simple);
static void	h_img_hyper(t_c_px *calcul, t_hyper *hy, t_hyper_calc *c);
static void	h_normal_hyper(t_c_px *ca, t_hyper *hy, t_hyper_calc *c);

///////////////////////////////////////////////////////////////////////////////]
// 		x²/a² + y²/b² - z²/c² = R²
int	distance_from_hyper(t_c_px *calcul, void *obj, int simple)
{
	t_hyper_calc	c;
	t_hyper			*hy;

	hy = (t_hyper *)obj;
	c.a2 = hy->abc.x * hy->abc.x;
	c.b2 = hy->abc.y * hy->abc.y;
	c.c2 = hy->abc.z * hy->abc.z;
	ft_rotate_around_obj(calcul, (t_obj2 *)hy, (t_obj *)&c.new_o);
	c.a = c.rot_v.dx * c.rot_v.dx / c.a2 + c.rot_v.dy * c.rot_v.dy / c.b2 \
		- c.rot_v.dz * c.rot_v.dz / c.c2;
	c.b = 2 * (c.rot_v.dx * c.new_o.x / c.a2 + c.rot_v.dy * c.new_o.y / c.b2 \
		- c.rot_v.dz * c.new_o.z / c.c2);
	c.c = c.new_o.x * c.new_o.x / c.a2 + c.new_o.y * c.new_o.y / c.b2 \
		- c.new_o.z * c.new_o.z / c.c2 - hy->radius;
	c.delta = c.b * c.b - 4 * c.a * c.c;
	if (c.delta < EPSILON || fabs(c.a) < EPSILON)
		return (0);
	c.det1 = (-c.b + sqrt(c.delta)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.delta)) / (2 * c.a);
	c.dist = h_smalest_delta(c.det1, c.det2);
	if (c.dist < EPSILON)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0)
		return (h_dist_para(calcul, hy, &c, simple));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
static int	h_dist_para(t_c_px *ca, t_hyper *hy, t_hyper_calc *c, int simple)
{
	double	in;

	if (simple)
		return (1);
	ca->dist = c->dist;
	ca->object = (void *)hy;
	ca->inter = new_moved_point(&ca->c0, &ca->v, ca->dist);
	h_normal_hyper(ca, hy, c);
	ca->mat = *(t_mat2 *)&hy->param;
	if (!hy->param.txt && hy->param.c2.r >= 0)
		ca->mat.argb = dual_color(&hy->param.argb, &hy->param.c2, \
			ft_dot_p(&ca->vn, &hy->O.view) / 2 + 0.5);
	in = ca->c0.x * ca->c0.x / c->a2 + ca->c0.y * ca->c0.y \
		/ c->b2 - ca->c0.z * ca->c0.z / c->c2 - hy->radius;
	c->inside = 0;
	if (in > EPSILON)
	{
		c->inside = 1;
		ca->vn = (t_vect){-ca->vn.dx, -ca->vn.dy, -ca->vn.dz};
	}
	if (is_there_txt(&hy->param))
		h_img_hyper(ca, hy, c);
	return (1 + c->inside);
}

///////////////////////////////////////////////////////////////////////////////]
// 		ATAN2 [−π,π][−π,π] > [0,1].
// 		cosϕ=[1top,-1bot]	ACOS [0,π] > [0,1].
static void	h_img_hyper(t_c_px *calcul, t_hyper *hy, t_hyper_calc *c)
{
	int		in;
	double	u;
	double	v;
	t_vect	normal_map;
	t_obj	local;

	in = (1 - 2 * c->inside);
	u = fmin(1.0, fmax(0.0, atan2(ft_dot_p(&calcul->vn, &hy->O.right) * in, \
		ft_dot_p(&calcul->vn, &hy->O.up) * in) / (2 * PI) + 0.5));
	v = fmin(1.0, fmax(0.0, acos(ft_dot_p(&calcul->vn, &hy->O.view) * in) \
		/ PI));
	update_mat_w_txt(calcul, (t_obj2 *)hy, u, v);
	if (hy->param.n_map)
	{
		normal_map = return_vect_img(hy->param.n_map, u, v);//
		local.view = calcul->vn;
		local.right = ft_cross_product_norm(&hy->O.view, &local.view);
		local.up = ft_cross_product_norm(&local.view, &local.right);
		normal_map.dx *= in;
		calcul->vn = mult_3x3_vect(&local, &calcul->vn);
		ft_normalize_vect(&calcul->vn);
	}
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_normal_hyper(t_c_px *ca, t_hyper *hy, t_hyper_calc *c)
{
	t_coor	temp_inter;

	temp_inter = new_moved_point(&c->new_o, &c->rot_v, c->dist);
	ca->vn = (t_vect){2.0 * temp_inter.x / c->a2, 2.0 \
		* temp_inter.y / c->b2, -2.0 * temp_inter.z / c->c2};
	ca->vn = (t_vect){
		ca->vn.dx * hy->O.right.dx + ca->vn.dy * hy->O.right.dy \
			+ ca->vn.dz * hy->O.right.dz,
		ca->vn.dx * hy->O.up.dx + ca->vn.dy * hy->O.up.dy \
			+ ca->vn.dz * hy->O.up.dz,
		ca->vn.dx * hy->O.view.dx + ca->vn.dy * hy->O.view.dy \
			+ ca->vn.dz * hy->O.view.dz};
	ft_normalize_vect(&ca->vn);
}
