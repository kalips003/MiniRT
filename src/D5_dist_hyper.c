/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   D5_dist_hyper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/30 10:59:52 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

typedef struct s_hyper_calc1 {
	double	a2;
	double	b2;
	double	c2;

	double	a;
	double	b;
	double	c;

	double	Δ;

	double	det1;
	double	det2;
	double	dist;
	
	t_coor	new_o;
	t_vect	rot_v;

	int		inside;

}	t_hyper_calc1;

int	distance_from_hyper(t_c_px *calcul, void *obj, int simple);
static int	h_dist_hyper(t_c_px *calcul, t_hyper *hy, t_hyper_calc1 *c, int simple);
void	ft_rotate_camera_vect2(t_c_px *calcul, t_hyper *hy, t_hyper_calc1 *c);
static void	h_img_sphere(t_c_px *calcul, t_sphere *sphere, t_sphere_calc *c);

// Negative radius

///////////////////////////////////////////////////////////////////////////////]
//	a.(x-x0) + b(y-y0) + c(z-z0) + d = 0
// d is dependant of the plane:
// 		ax + by + cz + d = 0
// 		d = -(ax + by + cz)
// top = signed distance camera plane ~ cos angle
// bot = dot product view vector / plane vector (means nothing)
int	distance_from_hyper(t_c_px *calcul, void *obj, int simple)
{
	t_hyper_calc1	c;
	t_hyper			*hy;

	hy = (t_hyper *)obj;
	c.a2 = hy->abc.x * hy->abc.x;
	c.b2 = hy->abc.y * hy->abc.y;
	c.c2 = hy->abc.z * hy->abc.z;
	ft_rotate_camera_vect2(calcul, hy, &c);
	c.a = c.rot_v.dx * c.rot_v.dx / c.a2 + c.rot_v.dy * c.rot_v.dy / c.b2 - c.rot_v.dz * c.rot_v.dz / c.c2;
	c.b = 2 * (c.rot_v.dx * c.new_o.x / c.a2 + c.rot_v.dy * c.new_o.y / c.b2 - c.rot_v.dz * c.new_o.z / c.c2);
	c.c = c.new_o.x * c.new_o.x / c.a2 + c.new_o.y * c.new_o.y / c.b2 - c.new_o.z * c.new_o.z / c.c2 - hy->radius * hy->radius;

	c.Δ = c.b * c.b - 4 * c.a * c.c;
	if (c.Δ < EPSILON || fabs(c.a) < EPSILON)
		return (0);
	c.det1 = (-c.b + sqrt(c.Δ)) / (2 * c.a);
	c.det2 = (-c.b - sqrt(c.Δ)) / (2 * c.a);
	c.dist = h_smalest_delta(c.det1, c.det2);
	if (c.dist <= 0.0)
		return (0);
	if (c.dist < calcul->dist || calcul->dist < 0.0)
		return (h_dist_hyper(calcul, hy, &c, simple));
	return (0);
}


///////////////////////////////////////////////////////////////////////////////]
static int	h_dist_hyper(t_c_px *calcul, t_hyper *hy, t_hyper_calc1 *c, int simple)
{
	if (simple)
		return (1);
	
	calcul->dist = c->dist;
	calcul->object = (void *)hy;
	
	calcul->inter = new_moved_point(&calcul->c0, &calcul->v, calcul->dist);
	calcul->vn = (t_vect){
			2.0 * calcul->inter.x / c->a2,
			2.0 * calcul->inter.y / c->b2,
			-2.0 * calcul->inter.z / c->c2
	};
	ft_normalize_vect(&calcul->vn);
	calcul->mat = *(t_mat *)&hy->param;
	// calcul->argb = sphere->param.argb;

	if (!hy->param.texture && hy->param.color2.r >= 0)
		calcul->mat.argb = dual_color_render(&hy->param.argb, &hy->param.color2, ft_dot_product(&calcul->vn, &hy->O.view) / 2 + 0.5);
	
	c->inside = 0;
	if (c->det1 < 0.0 || c->det2 < 0.0)
	{
		c->inside = 1;
		calcul->vn = (t_vect){-calcul->vn.dx, -calcul->vn.dy, -calcul->vn.dz};
	}
	if (hy->param.texture || hy->param.normal_map || hy->param.alpha_map || hy->param.ao_map)
	// if (sphere->param.texture || sphere->param.normal_map || sphere->param.alpha_map)
		h_img_sphere(calcul, hy, c);

	return (1 + c->inside);
}

void	ft_rotate_camera_vect2(t_c_px *calcul, t_hyper *hy, t_hyper_calc1 *c)
{
	c->new_o = new_moved_point(&calcul->c0, (t_vect *)&hy->O.c0, -1.0);
	c->new_o = (t_coor){
		c->new_o.x * hy->O.right.dx + c->new_o.y * hy->O.up.dx + c->new_o.z * hy->O.view.dx,
		c->new_o.x * hy->O.right.dy + c->new_o.y * hy->O.up.dy + c->new_o.z * hy->O.view.dy,
		c->new_o.x * hy->O.right.dz + c->new_o.y * hy->O.up.dz + c->new_o.z * hy->O.view.dz,
	};
	c->rot_v = (t_vect)
	{
		calcul->v.dx * hy->O.right.dx + calcul->v.dy * hy->O.up.dx + calcul->v.dz * hy->O.view.dx,
		calcul->v.dx * hy->O.right.dy + calcul->v.dy * hy->O.up.dy + calcul->v.dz * hy->O.view.dy,
		calcul->v.dx * hy->O.right.dz + calcul->v.dy * hy->O.up.dz + calcul->v.dz * hy->O.view.dz,
	};
	ft_normalize_vect(&c->rot_v);
}

///////////////////////////////////////////////////////////////////////////////]
// 		ATAN2 [−π,π][−π,π] > [0,1].
// 		cosϕ=[1top,-1bot]	ACOS [0,π] > [0,1].
static void	h_img_sphere(t_c_px *calcul, t_sphere *sphere, t_sphere_calc *c)
{
	int	inside = (1 - 2 * c->inside);
	
	double cosθ = ft_dot_product(&calcul->vn, &sphere->O.up) * inside;
	double sinθ = ft_dot_product(&calcul->vn, &sphere->O.right) * inside;
	double cosϕ = ft_dot_product(&calcul->vn, &sphere->O.view) * inside;
	double	l_θ = fmin(1.0, fmax(0.0, atan2(sinθ, cosθ)  / (2 * PI) + 0.5));
	double	l_ϕ = fmin(1.0, fmax(0.0, acos(cosϕ) / PI));

	if (sphere->param.ao_map)
	{
		if (calcul->print == 1)
			printf("ao before: %f\n", calcul->ao);
		calcul->ao = return_alpha_img(sphere->param.ao_map, l_θ, l_ϕ) / 255.0;
		if (calcul->print == 1)
			printf("ao after: %f\n", calcul->ao);
	}
	if (sphere->param.texture)
		calcul->mat.argb = return_px_img(sphere->param.texture, l_θ, l_ϕ);
	if (sphere->param.alpha_map)
		calcul->mat.argb.a = return_alpha_img(sphere->param.alpha_map, l_θ, l_ϕ);
	if (sphere->param.normal_map)
	{
		t_vect	normal_map = return_vect_img(sphere->param.normal_map, l_θ, l_ϕ);
		t_obj	local;
		local.view = calcul->vn;
		local.right = ft_cross_product_norm(&sphere->O.view, &local.view);
		local.up = ft_cross_product_norm(&local.view, &local.right);
		normal_map.dx *= inside;
		calcul->vn = (t_vect){
			local.right.dx * normal_map.dx + local.up.dx * normal_map.dy + local.view.dx * normal_map.dz,
			local.right.dy * normal_map.dx + local.up.dy * normal_map.dy + local.view.dy * normal_map.dz,
			local.right.dz * normal_map.dx + local.up.dz * normal_map.dy + local.view.dz * normal_map.dz,
		};
		ft_normalize_vect(&calcul->vn);
	}
}