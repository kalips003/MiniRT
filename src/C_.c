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

typedef struct s_temp_calc
{
// 6 Const value f(x) to compute temp rotation vector V'' with const rotation X
	// (X) = cos²Vαx + sin²(Vαx(Rx²-Ry²-Rz²)+2Rx(RyVαy+RzVαz)) + 2cossin(RyVαz-RzVαy) == cos²Vαx + sin² * (A2x) + (B2x)
	// (Y) = cos²Vαy + sin²(Vαx(-Rx²+Ry²-Rz²)+2Ry(RxVαx+RzVαz)) + 2cossin(RzVαx-RxVαz) == cos²Vαy + sin² * (A2y) + (B2y)
	// (Z) = cos²Vαz + sin²(Vαx(-Rx²-Ry²+Rz²)+2Rz(RxVαx+RyVαy)) + 2cossin(RxVαy-RyVαx) == cos²Vαz + sin² * (A2z) + (B2z)
	double A2x;
	double A2y;
	double A2z;

	double B2x;
	double B2y;
	double B2z;

}	t_temp_calc;

// int	ft_render_frame(t_data *data);
// static void	compute_const_y(t_data *data, t_temp_calc *c2, t_vect *v1);
// static void	compute_final_vect(t_data *data, double angle, t_vect *rtrn, t_temp_calc *c3, t_vect *v1);

///////////////////////////////////////////////////////////////////////////////]
// 	RENDER RAY CASTING
// int	ft_render_frame(t_data *data)
// {
// 	int y;
// 	int x = -1;
// 	t_calcul c;

// 	h_eye_compute_const_var(data);
// 	// put("view  2>\t [%f,%f,%f]\n", data->e.c->view.dx, data->e.c->view.dy, data->e.c->view.dz);
// 	// put("up    2>\t [%f,%f,%f]\n", data->e.c->up.dx, data->e.c->up.dy, data->e.c->up.dz);
// 	// put("right 2>\t [%f,%f,%f]\n", data->e.c->right.dx, data->e.c->right.dy, data->e.c->right.dz);
// 	while (++x < SIZE_SCREEN_X)
// 	{
// 		y = -1;
// 		// Calculate cos & sin for angle A (x)
// 		t_vect Vα;
// 		compute_temp_vect(data, data->e.px0 + x * data->e.px, &Vα);
// 		printf("\tangle = %f\n", data->e.px0 + x * data->e.px);
// 		t_temp_calc	c2;
// 		compute_const_y(data, &c2, &Vα);

// 		while (++y < SIZE_SCREEN_Y)	
// 		{
// 			// Calculate cos & sin for angle β (y)
// 			t_vect Vβ;
// 			compute_final_vect(data, data->e.py0 + y * data->e.px, &Vβ, &c2, &Vα);

// 			printf("X=%d, Y=%d; V FINAL = [%f,%f,%f]\n", x, y, Vβ.dx, Vβ.dy, Vβ.dz);
// 			ft_find_pixel_colision(data, &c, &Vβ);
// 			t_rgb color;
// 			color = calculate_pixel_color(data, &Vβ);
// 			mlx_pixel_put(data->mlx, data->win, x, y, color.r << 16 | color.g << 8 | color.b);

// 			// mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
// 		}
// 	}
// 	// end(data, 0);
// 	printf("----------------------------------------------\n");
// 	return (0);
// }

// int	ft_render_frame(t_data *data)
// {
// 	int y = -1;
// 	int x;
// 	t_calcul c;

// 	h_eye_compute_const_var(data);
// 	// put("view  2>\t [%f,%f,%f]\n", data->e.c->view.dx, data->e.c->view.dy, data->e.c->view.dz);
// 	// put("up    2>\t [%f,%f,%f]\n", data->e.c->up.dx, data->e.c->up.dy, data->e.c->up.dz);
// 	// put("right 2>\t [%f,%f,%f]\n", data->e.c->right.dx, data->e.c->right.dy, data->e.c->right.dz);
// 	while (++y < SIZE_SCREEN_Y)
// 	{
// 		x = -1;
// 		// Calculate cos & sin for angle A (x)
// 		t_vect Vα;
// 		compute_temp_vect(data, data->e.py0 + y * data->e.px, &Vα);
// 		printf("\tangle = %f\n", data->e.px0 + x * data->e.px);
// 		t_temp_calc	c2;
// 		compute_const_y(data, &c2, &Vα);

// 		while (++x < SIZE_SCREEN_X)
// 		{
// 			// Calculate cos & sin for angle β (y)
// 			t_vect Vβ;
// 			compute_final_vect(data, data->e.px0 + x * data->e.px, &Vβ, &c2, &Vα);

// 			printf("X=%d, Y=%d; V FINAL = [%f,%f,%f]\n", x, y, Vβ.dx, Vβ.dy, Vβ.dz);
// 			ft_find_pixel_colision(data, &c, &Vβ);
// 			t_rgb color;
// 			color = calculate_pixel_color(data, &Vβ);
// 			mlx_pixel_put(data->mlx, data->win, x, y, color.r << 16 | color.g << 8 | color.b);

// 			// mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
// 		}
// 	}
// 	// end(data, 0);
// 	printf("----------------------------------------------\n");
// 	return (0);
// }

///////////////////////////////////////////////////////////////////////////////]
/// compute 6 Const value f(V', UP) to compute temp rotation vector V'
// static void	compute_const_y(t_data *data, t_temp_calc *c2, t_vect *v1)
// {
// 	double Rx2 = data->e.c->up.dx * data->e.c->up.dx;
// 	double Ry2 = data->e.c->up.dy * data->e.c->up.dy;
// 	double Rz2 = data->e.c->up.dz * data->e.c->up.dz;

// 	c2->A2x = v1->dx * (Rx2 - Ry2 - Rz2) + 2 * data->e.c->up.dx * (data->e.c->up.dy * v1->dy + data->e.c->up.dz * v1->dz);
// 	c2->A2y = v1->dy * (-Rx2 + Ry2 - Rz2) + 2 * data->e.c->up.dy * (data->e.c->up.dx * v1->dx + data->e.c->up.dz * v1->dz);
// 	c2->A2z = v1->dz * (-Rx2 - Ry2 + Rz2) + 2 * data->e.c->up.dz * (data->e.c->up.dx * v1->dx + data->e.c->up.dy * v1->dy);

// 	c2->B2x = 2 * (data->e.c->up.dy * v1->dz - data->e.c->up.dz * v1->dy);
// 	c2->B2y = 2 * (data->e.c->up.dz * v1->dx - data->e.c->up.dx * v1->dz);
// 	c2->B2z = 2 * (data->e.c->up.dx * v1->dy - data->e.c->up.dy * v1->dx);
// }

///////////////////////////////////////////////////////////////////////////////]
/// compute from the angle β and temp vect the final rotated vector in the vertical
// cos²Cx + sin²(A2x) + cossin(B2x)
// cos²Cy + sin²(A2y) + cossin(B2y)
// cos²Cz + sin²(A2z) + cossin(B2z)
// static void	compute_final_vect(t_data *data, double angle, t_vect *rtrn, t_temp_calc *c3, t_vect *v1)
// {
// 		double cos2B = cos(angle / 2);
// 		double sin2B = sin(angle / 2);
// 		double cossinB = cos2B * sin2B;
// 		cos2B = cos2B * cos2B;
// 		sin2B = sin2B * sin2B;

// 		rtrn->dx = cos2B * v1->dx + sin2B * c3->A2x + cossinB * c3->B2x;
// 		rtrn->dy = cos2B * v1->dy + sin2B * c3->A2y + cossinB * c3->B2y;
// 		rtrn->dz = cos2B * v1->dz + sin2B * c3->A2z + cossinB * c3->B2z;

// 		ft_normalize_vect(rtrn);
// }


///////////////////////////////////////////////////////////////////////////////]
/// compute 6 Const value f(Camera, UP) to compute temp rotation vector V'
/// compute 6 Const value f(Camera, Right) to compute temp rotation vector V'
/// (X) = cos²Cx + sin²(Cx(Ux²-Uy²-Uz²)+2Ux(UyCy+UzCz)) + 2cossin(UyCz-UzCy) == cos²Cx + sin²(A1x) + cossin(B1x)
/// (Y) = cos²Cy + sin²(Cx(-Ux²+Uy²-Uz²)+2Uy(UxCx+UzCz)) + 2cossin(UzCx-UxCz) == cos²Cy + sin²(A1y) + cossin(B1y)
/// (Z) = cos²Cz + sin²(Cx(-Ux²-Uy²+Uz²)+2Uz(UxCx+UyCy)) + 2cossin(UxCy-UyCx) == cos²Cz + sin²(A1z) + cossin(B1z)
// void	h_eye_compute_const_var(t_data *data)
// {
// 	double Ux2 = data->e.c->right.dx * data->e.c->right.dx;
// 	double Uy2 = data->e.c->right.dy * data->e.c->right.dy;
// 	double Uz2 = data->e.c->right.dz * data->e.c->right.dz;
	
// 	data->e.A1x = data->e.c->view.dx * (Ux2 - Uy2 - Uz2) + 2 * data->e.c->right.dx * (data->e.c->right.dy * data->e.c->view.dy + data->e.c->right.dz * data->e.c->view.dz);
// 	data->e.A1y = data->e.c->view.dy * (-Ux2 + Uy2 - Uz2) + 2 * data->e.c->right.dy * (data->e.c->right.dx * data->e.c->view.dx + data->e.c->right.dz * data->e.c->view.dz);
// 	data->e.A1z = data->e.c->view.dz * (-Ux2 - Uy2 + Uz2) + 2 * data->e.c->right.dz * (data->e.c->right.dx * data->e.c->view.dx + data->e.c->right.dy * data->e.c->view.dy);

// 	data->e.B1x = 2 * (data->e.c->right.dy * data->e.c->view.dz - data->e.c->right.dz * data->e.c->view.dy);
// 	data->e.B1y = 2 * (data->e.c->right.dz * data->e.c->view.dx - data->e.c->right.dx * data->e.c->view.dz);
// 	data->e.B1z = 2 * (data->e.c->right.dx * data->e.c->view.dy - data->e.c->right.dy * data->e.c->view.dx);
// }

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




// 	OLD EULER ROTATION
// static void	ft_find_pixel_vector(t_data *data, t_vect *v, int x, int y)
// {
// 	double	angle_α;
// 	double	angle_β;

// 	// where α is a rotation around the Y axis == screen width
// 	// β is a rotation around the X axis == screen height
// 	angle_α = data->px0 + x * data->px;
// 	angle_β = data->py0 + y * data->px;
// 	//	a.cosα + b.sinα.sinβ + c.sinα.cosβ
// 	//	b.cosβ - c.sinβ
// 	//	-a.sinα + b.cosα.sinβ + c.cosα.cosβ
// 	v->dx = data->eye->abc.dx * cos(angle_α) + data->eye->abc.dy * sin(angle_α) * sin(angle_β) + data->eye->abc.dz * sin(angle_α) * cos(angle_β);
// 	v->dy = data->eye->abc.dy * cos(angle_β) - data->eye->abc.dz * sin(angle_β);
// 	v->dz = -data->eye->abc.dx * sin(angle_α) + data->eye->abc.dy * cos(angle_α) * sin(angle_β) + data->eye->abc.dz * cos(angle_α) * cos(angle_β);

// 	ft_normalize_vect(v);
// }



///////////////////////////////////////////////////////////////////////////////]
// 	RENDER RAY CASTING
// int	ft_render_rc(t_data *data)
// {
// 	int x;
// 	int y = -1;
// 	t_vect v;

// 	t_calcul c;

// 	while (++y < SIZE_SCREEN_Y)
// 	{
// 		x = -1;
// 		while (++x < SIZE_SCREEN_X)
// 		{
// 			ft_find_pixel_vector(data, &v, x, y);
// 			ft_find_pixel_colision(data, &c, &v);

// 			//> apply color at pixel pos x.y
// 			mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
// 		}
// 	}
// 	return (0);
// }

///////////////////////////////////////////////////////////////////////////////]
// 	RENDER RAY TRACING
// int	ft_render_rt(t_data *data)
// {
// 	int x;
// 	int y = -1;
// 	t_vect v;
// 	t_rgb color;

// 	while (++y < SIZE_SCREEN_Y)
// 	{
// 		x = -1;
// 		while (++x < SIZE_SCREEN_X)
// 		{
// 			ft_find_pixel_vector(data, &v, x, y);
			
// 			//>	for each object, calculate if there is colision, if there is, calcul the closest intersection point, retrun color?
// 			color = calculate_pixel_color(data, &v);

// 			//> apply color at pixel pos x.y
// 			mlx_pixel_put(data->mlx, data->win, x, y, color.r << 16 | color.g << 8 | color.b);
// 		}
// 	}
// 	return (0);
// }
