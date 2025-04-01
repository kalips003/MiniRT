/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D5_dist_para.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 15:43:32 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			distance_from_para(t_c_px *calcul, void *obj, int simple);
static int	h_dist_hyper(t_c_px *calcul, t_hyper *hy, t_hyper_calc *c, \
	int simple);
static void	h_img_para(t_c_px *calcul, t_hyper *hy, t_hyper_calc *c);
static void	h_normal_para(t_c_px *ca, t_hyper *para, t_hyper_calc *c);

///////////////////////////////////////////////////////////////////////////////]
// hyperbolic paraboloid
//	x²/a² - y²/b² = z/c
int	distance_from_para(t_c_px *calcul, void *obj, int simple)
{
	t_hyper_calc	c;
	t_hyper			*para;

	para = (t_hyper *)obj;
	c.a2 = para->abc.x * para->abc.x;
	c.b2 = para->abc.y * para->abc.y;
	ft_rotate_around_obj(calcul, (t_obj2 *)para, (t_obj *)&c.new_o);
	c.a = c.rot_v.dx * c.rot_v.dx / c.a2 - c.rot_v.dy * c.rot_v.dy / c.b2;
	c.b = 2 * (c.rot_v.dx * c.new_o.x / c.a2 - c.rot_v.dy * c.new_o.y / c.b2) \
		- c.rot_v.dz / para->abc.z;
	c.c = c.new_o.x * c.new_o.x / c.a2 + c.new_o.y * c.new_o.y / c.b2 \
		- c.new_o.z / para->abc.z;
	c.delta = c.b * c.b - 4 * c.a * c.c;
	if (c.delta < EPSILON || fabs(c.a) < EPSILON)
		return (0);
	c.det1 = (-c.b + sqrt(c.delta)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.delta)) / (2 * c.a);
	c.dist = h_smalest_delta(c.det1, c.det2);
	if (c.dist <= 0.0)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		return (h_dist_hyper(calcul, para, &c, simple));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
static int	h_dist_hyper(t_c_px *ca, t_hyper *para, t_hyper_calc *c, int simple)
{
	if (simple)
		return (1);
	ca->dist = c->dist;
	ca->object = (void *)para;
	ca->inter = new_moved_point(&ca->c0, &ca->v, ca->dist);
	h_normal_para(ca, para, c);
	ca->mat = *(t_mat2 *)&para->param;
	if (!para->param.txt && para->param.c2.r >= 0)
		ca->mat.argb = dual_color(&para->param.argb, &para->param.c2, \
			ft_dot_p(&ca->vn, &para->O.view) / 2 + 0.5);
	c->inside = 0;
	if (ft_dot_p(&ca->v, &ca->vn) > EPSILON)
	{
		c->inside = 1;
		ca->vn = (t_vect){-ca->vn.dx, -ca->vn.dy, -ca->vn.dz};
	}
	if (is_there_txt(&para->param))
		h_img_para(ca, para, c);
	return (1);
}

///////////////////////////////////////////////////////////////////////////////]
// 		ATAN2 [−π,π][−π,π] > [0,1].
// 		cosϕ=[1top,-1bot]	ACOS [0,π] > [0,1].
static void	h_img_para(t_c_px *calcul, t_hyper *pa, t_hyper_calc *c)
{
	int		in;
	double	u;
	double	v;
	t_vect	normal_map;
	t_obj	local;

	in = (1 - 2 * c->inside);
	u = fmin(1.0, fmax(0.0, atan2(ft_dot_p(&calcul->vn, &pa->O.right) * in, \
		ft_dot_p(&calcul->vn, &pa->O.up) * in) / (2 * PI) + 0.5));
	v = fmin(1.0, fmax(0.0, acos(ft_dot_p(&calcul->vn, &pa->O.view) * in) \
		/ PI));
	update_mat_w_txt(calcul, (t_obj2 *)pa, u, v);
	if (pa->param.n_map)
	{
		normal_map = return_vect_img(pa->param.n_map, u, v);
		local.view = calcul->vn;
		local.right = ft_cross_product_norm(&pa->O.view, &local.view);
		local.up = ft_cross_product_norm(&local.view, &local.right);
		normal_map.dx *= in;
		calcul->vn = mult_3x3_vect(&local, &calcul->vn);
		ft_normalize_vect(&calcul->vn);
	}
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_normal_para(t_c_px *ca, t_hyper *para, t_hyper_calc *c)
{
	t_coor	temp_inter;

	temp_inter = new_moved_point(&c->new_o, &c->rot_v, c->dist);//
	ca->vn = (t_vect){2.0 * temp_inter.x / c->a2, 2.0 \
		* temp_inter.y / c->b2, -2.0 * temp_inter.z / c->c2};
	ca->vn = (t_vect){
		ca->vn.dx * para->O.right.dx + ca->vn.dy * para->O.right.dy \
			+ ca->vn.dz * para->O.right.dz,
		ca->vn.dx * para->O.up.dx + ca->vn.dy * para->O.up.dy \
			+ ca->vn.dz * para->O.up.dz,
		ca->vn.dx * para->O.view.dx + ca->vn.dy * para->O.view.dy \
			+ ca->vn.dz * para->O.view.dz};
	ft_normalize_vect(&ca->vn);
}
