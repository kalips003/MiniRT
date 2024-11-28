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


static void	compute_const_y(t_data *data, t_temp_calc *c2, t_vect *v1);
static void	compute_final_vect(t_data *data, double angle, t_vect *rtrn, t_temp_calc *c3, t_vect *v1);
int	ft_render_frame(t_data *data);


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

int	ft_render_frame(t_data *data)
{
	int y = -1;
	int x;
	t_calcul c;

	h_eye_compute_const_var(data);
	// put("view  2>\t [%f,%f,%f]\n", data->e.c->view.dx, data->e.c->view.dy, data->e.c->view.dz);
	// put("up    2>\t [%f,%f,%f]\n", data->e.c->up.dx, data->e.c->up.dy, data->e.c->up.dz);
	// put("right 2>\t [%f,%f,%f]\n", data->e.c->right.dx, data->e.c->right.dy, data->e.c->right.dz);
	while (++y < SIZE_SCREEN_Y)
	{
		x = -1;
		// Calculate cos & sin for angle A (x)
		t_vect Vα;
		compute_temp_vect(data, data->e.py0 + y * data->e.px, &Vα);
		printf("\tangle = %f\n", data->e.px0 + x * data->e.px);
		t_temp_calc	c2;
		compute_const_y(data, &c2, &Vα);

		while (++x < SIZE_SCREEN_X)
		{
			// Calculate cos & sin for angle β (y)
			t_vect Vβ;
			compute_final_vect(data, data->e.px0 + x * data->e.px, &Vβ, &c2, &Vα);

			printf("X=%d, Y=%d; V FINAL = [%f,%f,%f]\n", x, y, Vβ.dx, Vβ.dy, Vβ.dz);
			ft_find_pixel_colision(data, &c, &Vβ);
			t_rgb color;
			color = calculate_pixel_color(data, &Vβ);
			mlx_pixel_put(data->mlx, data->win, x, y, color.r << 16 | color.g << 8 | color.b);

			// mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
		}
	}
	// end(data, 0);
	printf("----------------------------------------------\n");
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
/// compute 6 Const value f(V', UP) to compute temp rotation vector V'
static void	compute_const_y(t_data *data, t_temp_calc *c2, t_vect *v1)
{
	double Rx2 = data->e.c->up.dx * data->e.c->up.dx;
	double Ry2 = data->e.c->up.dy * data->e.c->up.dy;
	double Rz2 = data->e.c->up.dz * data->e.c->up.dz;

	c2->A2x = v1->dx * (Rx2 - Ry2 - Rz2) + 2 * data->e.c->up.dx * (data->e.c->up.dy * v1->dy + data->e.c->up.dz * v1->dz);
	c2->A2y = v1->dy * (-Rx2 + Ry2 - Rz2) + 2 * data->e.c->up.dy * (data->e.c->up.dx * v1->dx + data->e.c->up.dz * v1->dz);
	c2->A2z = v1->dz * (-Rx2 - Ry2 + Rz2) + 2 * data->e.c->up.dz * (data->e.c->up.dx * v1->dx + data->e.c->up.dy * v1->dy);

	c2->B2x = 2 * (data->e.c->up.dy * v1->dz - data->e.c->up.dz * v1->dy);
	c2->B2y = 2 * (data->e.c->up.dz * v1->dx - data->e.c->up.dx * v1->dz);
	c2->B2z = 2 * (data->e.c->up.dx * v1->dy - data->e.c->up.dy * v1->dx);
}

///////////////////////////////////////////////////////////////////////////////]
/// compute from the angle β and temp vect the final rotated vector in the vertical
// cos²Cx + sin²(A2x) + cossin(B2x)
// cos²Cy + sin²(A2y) + cossin(B2y)
// cos²Cz + sin²(A2z) + cossin(B2z)
static void	compute_final_vect(t_data *data, double angle, t_vect *rtrn, t_temp_calc *c3, t_vect *v1)
{
		double cos2B = cos(angle / 2);
		double sin2B = sin(angle / 2);
		double cossinB = cos2B * sin2B;
		cos2B = cos2B * cos2B;
		sin2B = sin2B * sin2B;

		rtrn->dx = cos2B * v1->dx + sin2B * c3->A2x + cossinB * c3->B2x;
		rtrn->dy = cos2B * v1->dy + sin2B * c3->A2y + cossinB * c3->B2y;
		rtrn->dz = cos2B * v1->dz + sin2B * c3->A2z + cossinB * c3->B2z;

		ft_normalize_vect(rtrn);
}
