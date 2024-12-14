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
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg);

///////////////////////////////////////////////////////////////////////////////]
// 	Compute the Up and Right vector for each camera
// recalculate everytime the camera is rotated
void	h_camera_calc_up_right_vect(t_camera *camera)
{
// if camera vector is == Y vector
	if (fabs(camera->view.dx) < EPSILON && fabs(camera->view.dz) < EPSILON)
	{
		if (camera->view.dy > 0)
		{
			camera->up = (t_vect){0.0, 0.0, -1.0};
			camera->right = (t_vect){1.0, 0.0, 0.0};
			return ;
		}
		else
		{
			camera->up = (t_vect){0.0, 0.0, 1.0};
			camera->right = (t_vect){-1.0, 0.0, 0.0};
			return ;
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
// rotate the camera (eye) vector of fixed angle
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg)
{
	t_vect *c = &data->eye.c->view;
	
	double Vx2 = axis_rota->dx * axis_rota->dx;
	double Vy2 = axis_rota->dy * axis_rota->dy;
	double Vz2 = axis_rota->dz * axis_rota->dz;


	double resultx = COS_ROTA2 * c->dx + SIN_ROTA2 * (c->dx * (Vx2 - Vy2 - Vz2) + 2 * axis_rota->dx * (axis_rota->dy * c->dy + axis_rota->dz * c->dz)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dy * c->dz - axis_rota->dz * c->dy);
	double resulty = COS_ROTA2 * c->dy + SIN_ROTA2 * (c->dy * (Vy2 - Vx2 - Vz2) + 2 * axis_rota->dy * (axis_rota->dx * c->dx + axis_rota->dz * c->dz)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dz * c->dx - axis_rota->dx * c->dz);
	double resultz = COS_ROTA2 * c->dz + SIN_ROTA2 * (c->dz * (Vz2 - Vx2 - Vy2) + 2 * axis_rota->dz * (axis_rota->dx * c->dx + axis_rota->dy * c->dy)) + 2 * (posi_neg) * COSSIN_ROTA * (axis_rota->dx * c->dy - axis_rota->dy * c->dx);
	c->dx = resultx;
	c->dy = resulty;
	c->dz = resultz;

	ft_normalize_vect(c);
	h_camera_calc_up_right_vect(data->eye.c);
}
