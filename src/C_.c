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

int	ft_render_frame(t_data *data);


typedef struct s_eyess
{

	t_camera	*C;

	double i_sin2;
	double i_cossin;
	double j_sin2;
	double j_cossin;
	double k_sin2;
	double k_cossin;


}	t_eyess;

void	calculate_const_eye(t_data *data)
{

}
///////////////////////////////////////////////////////////////////////////////]
// 	RENDER RAY CASTING
int	ft_render_frame(t_data *data)
{
	int y;
	int x = -1;
	t_calcul c;


// 6 Const value f(camera) to compute temp rotation vector V'
	// (X) = cos²Cx + sin²(Cx(Ux²-Uy²-Uz²)+2Ux(UyCy+UzCz)) + 2cossin(UyCz-UzCy) == cos²Cx + sin² * (A1x) + (B1x)
	// (Y) = cos²Cy + sin²(Cx(-Ux²+Uy²-Uz²)+2Uy(UxCx+UzCz)) + 2cossin(UzCx-UxCz) == cos²Cy + sin² * (A1y) + (B1y)
	// (Z) = cos²Cz + sin²(Cx(-Ux²-Uy²+Uz²)+2Uz(UxCx+UyCy)) + 2cossin(UxCy-UyCx) == cos²Cz + sin² * (A1z) + (B1z)
	double A1x = data->eye->view.dx * (data->eye->up.dx * data->eye->up.dx - data->eye->up.dy * data->eye->up.dy - data->eye->up.dz * data->eye->up.dz) + 2 * data->eye->up.dx * (data->eye->up.dy * data->eye->view.dy + data->eye->up.dz * data->eye->view.dz);
	double A1y = data->eye->view.dy * (-data->eye->up.dx * data->eye->up.dx + data->eye->up.dy * data->eye->up.dy - data->eye->up.dz * data->eye->up.dz) + 2 * data->eye->up.dy * (data->eye->up.dx * data->eye->view.dx + data->eye->up.dz * data->eye->view.dz);
	double A1z = data->eye->view.dz * (-data->eye->up.dx * data->eye->up.dx - data->eye->up.dy * data->eye->up.dy + data->eye->up.dz * data->eye->up.dz) + 2 * data->eye->up.dz * (data->eye->up.dx * data->eye->view.dx + data->eye->up.dy * data->eye->view.dy);

	double B1x = 2 * (data->eye->up.dy * data->eye->view.dz - data->eye->up.dz * data->eye->view.dy);
	double B1y = 2 * (data->eye->up.dz * data->eye->view.dx - data->eye->up.dx * data->eye->view.dz);
	double B1z = 2 * (data->eye->up.dx * data->eye->view.dy - data->eye->up.dy * data->eye->view.dx);

	while (++x < SIZE_SCREEN_X)
	{
		y = -1;
// Calculate cos & sin for angle A (x)
		double	angle_α = data->e.px0 + x * data->e.px;

		double cos2A = cos(angle_α);
		double sin2A = sin(angle_α);
		double cossinA = cos2A * sin2A;
		cos2A = cos2A * cos2A;
		sin2A = sin2A * sin2A;

		t_vect Vα;
		Vα.dx = cos2A * data->eye->view.dx + sin2A * A1x + cossinA * B1x;
		Vα.dy = cos2A * data->eye->view.dy + sin2A * A1y + cossinA * B1y;
		Vα.dz = cos2A * data->eye->view.dz + sin2A * A1z + cossinA * B1z;

		ft_normalize_vect(&Vα);

// 6 Const value f(x) to compute temp rotation vector V'' with const rotation X
	// (X) = cos²Vαx + sin²(Vαx(Rx²-Ry²-Rz²)+2Rx(RyVαy+RzVαz)) + 2cossin(RyVαz-RzVαy) == cos²Vαx + sin² * (A2x) + (B2x)
	// (Y) = cos²Vαy + sin²(Vαx(-Rx²+Ry²-Rz²)+2Ry(RxVαx+RzVαz)) + 2cossin(RzVαx-RxVαz) == cos²Vαy + sin² * (A2y) + (B2y)
	// (Z) = cos²Vαz + sin²(Vαx(-Rx²-Ry²+Rz²)+2Rz(RxVαx+RyVαy)) + 2cossin(RxVαy-RyVαx) == cos²Vαz + sin² * (A2z) + (B2z)
		double A2x = Vα.dx * (data->eye->right.dx * data->eye->right.dx - data->eye->right.dy * data->eye->right.dy - data->eye->right.dz * data->eye->right.dz) + 2 * data->eye->right.dx * (data->eye->right.dy * Vα.dy + data->eye->right.dz * Vα.dz);
		double A2y = Vα.dy * (-data->eye->right.dx * data->eye->right.dx + data->eye->right.dy * data->eye->right.dy - data->eye->right.dz * data->eye->right.dz) + 2 * data->eye->right.dy * (data->eye->right.dx * Vα.dx + data->eye->right.dz * Vα.dz);
		double A2z = Vα.dz * (-data->eye->right.dx * data->eye->right.dx - data->eye->right.dy * data->eye->right.dy + data->eye->right.dz * data->eye->right.dz) + 2 * data->eye->right.dz * (data->eye->right.dx * Vα.dx + data->eye->right.dy * Vα.dy);

		double B2x = 2 * (data->eye->right.dy * Vα.dz - data->eye->right.dz * Vα.dy);
		double B2y = 2 * (data->eye->right.dz * Vα.dx - data->eye->right.dx * Vα.dz);
		double B2z = 2 * (data->eye->right.dx * Vα.dy - data->eye->right.dy * Vα.dx);

		while (++y < SIZE_SCREEN_Y)
		{
		// Calculate cos & sin for angle β (y)
			double	angle_β = data->e.py0 + y * data->e.px;

			double cos2B = cos(angle_β);
			double sin2B = sin(angle_β);
			double cossinB = cos2B * sin2B;
			cos2B = cos2B * cos2B;
			sin2B = sin2B * sin2B;

			t_vect Vβ;
			Vβ.dx = cos2B * data->eye->view.dx + sin2B * A2x + cossinB * B2x;
			Vβ.dy = cos2B * data->eye->view.dy + sin2B * A2y + cossinB * B2y;
			Vβ.dz = cos2B * data->eye->view.dz + sin2B * A2z + cossinB * B2z;

			ft_normalize_vect(&Vβ);
			ft_find_pixel_colision(data, &c, &Vβ);
			// t_rgb color;
			// color = calculate_pixel_color(data, &Vβ);
			// mlx_pixel_put(data->mlx, data->win, x, y, color.r << 16 | color.g << 8 | color.b);

			mlx_pixel_put(data->mlx, data->win, x, y, c.px_color.r << 16 | c.px_color.g << 8 | c.px_color.b);
			

		}
	}
	return (0);
}