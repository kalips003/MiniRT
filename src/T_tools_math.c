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
double calculate_angle(t_coor *intersection, t_coor *light, t_vect *normal);
void	h_camera(t_camera *camera);
void	h_eye(t_data *data);

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
// for light direction?
double calculate_angle(t_coor *intersection, t_coor *light, t_vect *normal)
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
void	h_camera(t_camera *camera)
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
// 6 Const value f(camera) to compute temp rotation vector V'
// (X) = cos²Cx + sin²(Cx(Ux²-Uy²-Uz²)+2Ux(UyCy+UzCz)) + 2cossin(UyCz-UzCy) == cos²Cx + sin²(A1x) + cossin(B1x)
// (Y) = cos²Cy + sin²(Cx(-Ux²+Uy²-Uz²)+2Uy(UxCx+UzCz)) + 2cossin(UzCx-UxCz) == cos²Cy + sin²(A1y) + cossin(B1y)
// (Z) = cos²Cz + sin²(Cx(-Ux²-Uy²+Uz²)+2Uz(UxCx+UyCy)) + 2cossin(UxCy-UyCx) == cos²Cz + sin²(A1z) + cossin(B1z)
void	h_eye(t_data *data)
{
	double Ux2 = data->e.c->up.dx * data->e.c->up.dx;
	double Uy2 = data->e.c->up.dy * data->e.c->up.dy;
	double Uz2 = data->e.c->up.dz * data->e.c->up.dz;
	
	data->e.A1x = data->e.c->view.dx * (Ux2 - Uy2 - Uz2) + 2 * data->e.c->up.dx * (data->e.c->up.dy * data->e.c->view.dy + data->e.c->up.dz * data->e.c->view.dz);
	data->e.A1y = data->e.c->view.dy * (-Ux2 + Uy2 - Uz2) + 2 * data->e.c->up.dy * (data->e.c->up.dx * data->e.c->view.dx + data->e.c->up.dz * data->e.c->view.dz);
	data->e.A1z = data->e.c->view.dz * (-Ux2 - Uy2 + Uz2) + 2 * data->e.c->up.dz * (data->e.c->up.dx * data->e.c->view.dx + data->e.c->up.dy * data->e.c->view.dy);

	data->e.B1x = 2 * (data->e.c->up.dy * data->e.c->view.dz - data->e.c->up.dz * data->e.c->view.dy);
	data->e.B1y = 2 * (data->e.c->up.dz * data->e.c->view.dx - data->e.c->up.dx * data->e.c->view.dz);
	data->e.B1z = 2 * (data->e.c->up.dx * data->e.c->view.dy - data->e.c->up.dy * data->e.c->view.dx);
}

// cos²Cx + sin²(A1x) + cossin(B1x)
// cos²Cy + sin²(A1y) + cossin(B1y)
// cos²Cz + sin²(A1z) + cossin(B1z)
void	h_angle(t_data *data, double angle, t_vect *rtrn)
{
	// double	angle_α = data->e.px0 + x * data->e.px;

	double cos2A = cos(angle);
	double sin2A = sin(angle);
	double cossinA = cos2A * sin2A;
	cos2A = cos2A * cos2A;
	sin2A = sin2A * sin2A;

	printf("A1x=%f; A1y=%f; A1z=%f; B1x=%f; B1y=%f; B1z=%f\n", data->e.A1x, data->e.A1y, data->e.A1z, data->e.B1x, data->e.B1y, data->e.B1z);
	rtrn->dx = cos2A * data->e.c->view.dx + sin2A * data->e.A1x + cossinA * data->e.B1x;
	rtrn->dy = cos2A * data->e.c->view.dy + sin2A * data->e.A1y + cossinA * data->e.B1y;
	rtrn->dz = cos2A * data->e.c->view.dz + sin2A * data->e.A1z + cossinA * data->e.B1z;

	ft_normalize_vect(rtrn);
}

double	vect_dot_product(t_vect *a, t_vect *b)
{
	return (a->dx * b->dx + a->dy * b->dy + a->dz * b->dz);
}