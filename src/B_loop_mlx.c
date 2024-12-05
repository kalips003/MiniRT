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

int		ft_loop_empty(t_data *data);
int		ft_render_frame(t_data *data);
int		ft_render_frame_plus(t_data *data);
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn);

///////////////////////////////////////////////////////////////////////////////]
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int 	ft_loop_empty(t_data *data)
{
	return (0);
}

int 	ft_loop(t_data *data)
{
	if (!data->is_not_moving)
		ft_render_frame(data);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	ft_render_frame(t_data *data)
{
	int y = -1;
	int x;
	t_calcul c;
	ft_memset(&c, 0, sizeof(t_calcul));
	c.origin = data->eye.c->xyz;

	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		while (++x < SIZE_SCREEN_X)
		{

			// double tx = (2.0 * x - SIZE_SCREEN_X) / SIZE_SCREEN_X;  // Normalized horizontal position (-1 to 1)
			// double ty = (2.0 * y - SIZE_SCREEN_Y) / SIZE_SCREEN_Y;  // Normalized vertical position (-1 to 1)

			// // Adjusted angles with tangent scaling
			// double angleA = atan(tan(data->eye.px0) + tx * tan(data->eye.px));
			// double angleB = atan(tan(data->eye.py0) + ty * tan(data->eye.px));

			double angleA = data->eye.px0 + x * data->eye.px;
			double angleB = data->eye.py0 + y * data->eye.px;
			
			f_calculate_combined_quaternion(data, angleA, angleB, &c.v_rotated);
			calculate_pixel_color_simple(data, &c);
			mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
		}
	}
	return (0);
}

int	ft_render_frame_plus(t_data *data)
{
	int y = -1;
	int x;
	t_calcul c;
	ft_memset(&c, 0, sizeof(t_calcul));
	c.origin = data->eye.c->xyz;

	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		while (++x < SIZE_SCREEN_X)
		{
			f_calculate_combined_quaternion(data, data->eye.px0 + x * data->eye.px, data->eye.py0 + y * data->eye.px, &c.v_rotated);
			calculate_pixel_color(data, &c);
			mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
		}
	}
	return (0);
}



///////////////////////////////////////////////////////////////////////////////]
// fills rtrnn with the ccoordonates of the rotated vector camera for [x,y]pixel
// Q rotation combined = qβ.qα
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn)
{
	t_camera *c = data->eye.c;
	
	double cosA = cos(angle_α / 2);
	double sinA = sin(angle_α / 2);
	double cosB = cos(-angle_β / 2);
	double sinB = sin(-angle_β / 2);

	double Qw = cosA*cosB - sinA*sinB * (c->right.dx * c->up.dx + c->right.dy * c->up.dy + c->right.dz * c->up.dz);
	double Qi = cosB*sinA * c->up.dx + cosA*sinB * c->right.dx + sinA*sinB*(c->right.dy * c->up.dz - c->right.dz * c->up.dy);
	double Qj = cosB*sinA * c->up.dy + cosA*sinB * c->right.dy + sinA*sinB*(c->right.dz * c->up.dx - c->right.dx * c->up.dz);
	double Qk = cosB*sinA * c->up.dz + cosA*sinB * c->right.dz + sinA*sinB*(c->right.dx * c->up.dy - c->right.dy * c->up.dx);

	rtrn->dx = c->view.dx * (Qw * Qw + Qi * Qi - Qj * Qj - Qk * Qk) + 2*c->view.dy * (Qi*Qj - Qw*Qk) + 2*c->view.dz * (Qi*Qk + Qw*Qj);
	rtrn->dy = c->view.dy * (Qw * Qw + Qj * Qj - Qi * Qi - Qk * Qk) + 2*c->view.dz * (Qj*Qk - Qw*Qi) + 2*c->view.dx * (Qi*Qj + Qw*Qk);
	rtrn->dz = c->view.dz * (Qw * Qw + Qk * Qk - Qi * Qi - Qj * Qj) + 2*c->view.dx * (Qi*Qk - Qw*Qj) + 2*c->view.dy * (Qj*Qk + Qw*Qi);

	ft_normalize_vect(rtrn);
}

