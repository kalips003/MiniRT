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

int	ft_normalize_vect(t_vect *vect);
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal);
void	h_camera_calc_up_right_vect(t_camera *camera);
void	h_eye_compute_const_var(t_data *data);
void	compute_temp_vect(t_data *data, double angle, t_vect *rtrn);
double	ft_vect_dot_product(t_vect *a, t_vect *b);


///////////////////////////////////////////////////////////////////////////////]
// Normalize vector
int	ft_normalize_vect(t_vect *vect)
{
	double normal_magnitude;

	normal_magnitude = sqrt(vect->dx * vect->dx + vect->dy * vect->dy + vect->dz * vect->dz);
	if (normal_magnitude == 0.0)
		return (1);
	vect->dx = vect->dx / normal_magnitude;
	vect->dy = vect->dy / normal_magnitude;
	vect->dz = vect->dz / normal_magnitude;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// calculate angle between camera ray and light source at intersection
double calculate_light_angle(t_coor *intersection, t_coor *light, t_vect *normal)
{
	t_vect	l;
	double cos_theta;

// Calculate Light Direction vector
	l.dx = light->x - intersection->x;
	l.dy = light->y - intersection->y;
	l.dz = light->z - intersection->z;

	if(ft_normalize_vect(&l) || ft_normalize_vect(normal))
		return (put(ERR9 "i dont know? vector normal?\n"), -1.0);

// Dot product of Light Direction and Normal
	cos_theta = l.dx * normal->dx + l.dy * normal->dy + l.dz * normal->dz;
	cos_theta = fmax(-1.0, fmin(1.0, cos_theta));

// Return the angle in radians
	return acos(cos_theta);
}


///////////////////////////////////////////////////////////////////////////////]
// 	Compute the Up and Right vector for each camera
// recalculate everytime the camera is rotated
void	h_camera_calc_up_right_vect(t_camera *camera)
{
// if camera vector is == Y vector
	if (camera->view.dx == 0.0 && camera->view.dz == 0.0)
	{
		if (camera->view.dy > 0)
		{
			camera->up = (t_vect){0.0, 0.0, -1.0};
			camera->right = (t_vect){1.0, 0.0, 0.0};
		}
		else
		{
			camera->up = (t_vect){0.0, 0.0, 1.0};
			camera->right = (t_vect){-1.0, 0.0, 0.0};
		}
	}

// Right = Camera x Y = {-Cz, 0, Cx}
	camera->right.dx = -camera->view.dz;
	camera->right.dy = 0;
	camera->right.dz = camera->view.dx;
	ft_normalize_vect(&camera->right);

// Up = Right x Camera = {-CxCy, Cx²+Cz², -CyCz}
	camera->up.dx = -camera->view.dx * camera->view.dy;
	camera->up.dy = camera->view.dx * camera->view.dx + camera->view.dz * camera->view.dz;
	camera->up.dz = -camera->view.dy * camera->view.dz;
	ft_normalize_vect(&camera->up);

}

///////////////////////////////////////////////////////////////////////////////]
/// compute 6 Const value f(Camera, UP) to compute temp rotation vector V'
/// compute 6 Const value f(Camera, Right) to compute temp rotation vector V'
/// (X) = cos²Cx + sin²(Cx(Ux²-Uy²-Uz²)+2Ux(UyCy+UzCz)) + 2cossin(UyCz-UzCy) == cos²Cx + sin²(A1x) + cossin(B1x)
/// (Y) = cos²Cy + sin²(Cx(-Ux²+Uy²-Uz²)+2Uy(UxCx+UzCz)) + 2cossin(UzCx-UxCz) == cos²Cy + sin²(A1y) + cossin(B1y)
/// (Z) = cos²Cz + sin²(Cx(-Ux²-Uy²+Uz²)+2Uz(UxCx+UyCy)) + 2cossin(UxCy-UyCx) == cos²Cz + sin²(A1z) + cossin(B1z)
void	h_eye_compute_const_var(t_data *data)
{
	double Ux2 = data->e.c->right.dx * data->e.c->right.dx;
	double Uy2 = data->e.c->right.dy * data->e.c->right.dy;
	double Uz2 = data->e.c->right.dz * data->e.c->right.dz;
	
	data->e.A1x = data->e.c->view.dx * (Ux2 - Uy2 - Uz2) + 2 * data->e.c->right.dx * (data->e.c->right.dy * data->e.c->view.dy + data->e.c->right.dz * data->e.c->view.dz);
	data->e.A1y = data->e.c->view.dy * (-Ux2 + Uy2 - Uz2) + 2 * data->e.c->right.dy * (data->e.c->right.dx * data->e.c->view.dx + data->e.c->right.dz * data->e.c->view.dz);
	data->e.A1z = data->e.c->view.dz * (-Ux2 - Uy2 + Uz2) + 2 * data->e.c->right.dz * (data->e.c->right.dx * data->e.c->view.dx + data->e.c->right.dy * data->e.c->view.dy);

	data->e.B1x = 2 * (data->e.c->right.dy * data->e.c->view.dz - data->e.c->right.dz * data->e.c->view.dy);
	data->e.B1y = 2 * (data->e.c->right.dz * data->e.c->view.dx - data->e.c->right.dx * data->e.c->view.dz);
	data->e.B1z = 2 * (data->e.c->right.dx * data->e.c->view.dy - data->e.c->right.dy * data->e.c->view.dx);
}

// void	h_eye_compute_const_var(t_data *data)
// {
// 	double Ux2 = data->e.c->up.dx * data->e.c->up.dx;
// 	double Uy2 = data->e.c->up.dy * data->e.c->up.dy;
// 	double Uz2 = data->e.c->up.dz * data->e.c->up.dz;
	
// 	data->e.A1x = data->e.c->view.dx * (Ux2 - Uy2 - Uz2) + 2 * data->e.c->up.dx * (data->e.c->up.dy * data->e.c->view.dy + data->e.c->up.dz * data->e.c->view.dz);
// 	data->e.A1y = data->e.c->view.dy * (-Ux2 + Uy2 - Uz2) + 2 * data->e.c->up.dy * (data->e.c->up.dx * data->e.c->view.dx + data->e.c->up.dz * data->e.c->view.dz);
// 	data->e.A1z = data->e.c->view.dz * (-Ux2 - Uy2 + Uz2) + 2 * data->e.c->up.dz * (data->e.c->up.dx * data->e.c->view.dx + data->e.c->up.dy * data->e.c->view.dy);

// 	data->e.B1x = 2 * (data->e.c->up.dy * data->e.c->view.dz - data->e.c->up.dz * data->e.c->view.dy);
// 	data->e.B1y = 2 * (data->e.c->up.dz * data->e.c->view.dx - data->e.c->up.dx * data->e.c->view.dz);
// 	data->e.B1z = 2 * (data->e.c->up.dx * data->e.c->view.dy - data->e.c->up.dy * data->e.c->view.dx);
// }

///////////////////////////////////////////////////////////////////////////////]
/// compute from the angle α the rotated vector in the horizontal
// cos²Cx + sin²(A1x) + cossin(B1x)
// cos²Cy + sin²(A1y) + cossin(B1y)
// cos²Cz + sin²(A1z) + cossin(B1z)
void	compute_temp_vect(t_data *data, double angle, t_vect *rtrn)
{
	double cos2A = cos(angle / 2);
	double sin2A = sin(angle / 2);
	double cossinA = cos2A * sin2A;
	cos2A = cos2A * cos2A;
	sin2A = sin2A * sin2A;

	printf("A1x=%f; A1y=%f; A1z=%f; B1x=%f; B1y=%f; B1z=%f\n", data->e.A1x, data->e.A1y, data->e.A1z, data->e.B1x, data->e.B1y, data->e.B1z);
	rtrn->dx = cos2A * data->e.c->view.dx + sin2A * data->e.A1x + cossinA * data->e.B1x;
	rtrn->dy = cos2A * data->e.c->view.dy + sin2A * data->e.A1y + cossinA * data->e.B1y;
	rtrn->dz = cos2A * data->e.c->view.dz + sin2A * data->e.A1z + cossinA * data->e.B1z;

	ft_normalize_vect(rtrn);
}

// void	compute_temp_vect(t_data *data, double angle, t_vect *rtrn)
// {
// 	double cos2A = cos(angle / 2);
// 	double sin2A = sin(angle / 2);
// 	double cossinA = cos2A * sin2A;
// 	cos2A = cos2A * cos2A;
// 	sin2A = sin2A * sin2A;

// 	printf("A1x=%f; A1y=%f; A1z=%f; B1x=%f; B1y=%f; B1z=%f\n", data->e.A1x, data->e.A1y, data->e.A1z, data->e.B1x, data->e.B1y, data->e.B1z);
// 	rtrn->dx = cos2A * data->e.c->view.dx + sin2A * data->e.A1x + cossinA * data->e.B1x;
// 	rtrn->dy = cos2A * data->e.c->view.dy + sin2A * data->e.A1y + cossinA * data->e.B1y;
// 	rtrn->dz = cos2A * data->e.c->view.dz + sin2A * data->e.A1z + cossinA * data->e.B1z;

// 	ft_normalize_vect(rtrn);
// }

double	ft_vect_dot_product(t_vect *a, t_vect *b)
{
	return (a->dx * b->dx + a->dy * b->dy + a->dz * b->dz);
}

// Q rotation combined = qβ.qα
void	f_calculate_combined_quaternion(t_data *data, double angle_α, double angle_β, t_vect *rtrn)
{
	t_camera *c = data->e.c;
	
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

int	ft_render_frame_v45(t_data *data)
{
	int y = -1;
	int x;
	t_calcul c;


	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		while (++x < SIZE_SCREEN_X)
		{

			t_vect v_final;
			f_calculate_combined_quaternion(data, data->e.px0 + x * data->e.px, data->e.py0 + y * data->e.px, &v_final);
			ft_find_pixel_colision(data, &c, &v_final);
			t_rgb color;
			color = calculate_pixel_color(data, &v_final);
			mlx_pixel_put(data->mlx, data->win, x, y, color.r << 16 | color.g << 8 | color.b);

			// mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
		}
	}
	// end(data, 0);
	// printf("----------------------------------------------\n");
	return (0);
}

// rotate the camera (eye) vector of fixed angle
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg)
{
	t_vect *c = &data->e.c->view;
	
	double Vx2 = axis_rota->dx * axis_rota->dx;
	double Vy2 = axis_rota->dy * axis_rota->dy;
	double Vz2 = axis_rota->dz * axis_rota->dz;

	c->dx = COS_ROTA2 * c->dx + SIN_ROTA2 * (c->dx * (Vx2 - Vy2 - Vz2) + 2 * axis_rota->dx * (axis_rota->dy * c->dy + axis_rota->dz * c->dz)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dy * c->dz - axis_rota->dz * c->dy);
	c->dy = COS_ROTA2 * c->dy + SIN_ROTA2 * (c->dy * (Vy2 - Vx2 - Vz2) + 2 * axis_rota->dy * (axis_rota->dx * c->dx + axis_rota->dz * c->dz)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dz * c->dx - axis_rota->dx * c->dz);
	c->dz = COS_ROTA2 * c->dz + SIN_ROTA2 * (c->dz * (Vz2 - Vx2 - Vy2) + 2 * axis_rota->dz * (axis_rota->dx * c->dx + axis_rota->dy * c->dy)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dx * c->dy - axis_rota->dy * c->dx);

	ft_normalize_vect(c);
	h_camera_calc_up_right_vect(data->e.c);
}