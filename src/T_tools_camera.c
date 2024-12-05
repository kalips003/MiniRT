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

void	h_camera_calc_up_right_vect(t_camera *camera);
void	f_recalculate_camera(t_data *data);

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

void	f_recalculate_camera(t_data *data)
{
		data->eye.c = data->camera[0];
// DATA CAMERA, should be recalculated at each change in camera
	data->eye.px = data->eye.c->fov * (PI / 180) / SIZE_SCREEN_X;
	data->eye.px0 = -(SIZE_SCREEN_X / 2) * data->eye.px;
	data->eye.py0 = -(SIZE_SCREEN_Y / 2) * data->eye.px;
}