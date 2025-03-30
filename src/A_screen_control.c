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

t_vect	v_cam(t_data *data, int x, int y, int aa);
t_vect	cam_vector(t_camera *cam, int x, int y, int aa);
t_vect	cam_quaternion(t_data *data, int x, int y, int aa);

///////////////////////////////////////////////////////////////////////////////]
t_vect	v_cam(t_data *data, int x, int y, int aa)
{
	if (data->f_cam == VECTOR)
		return (cam_vector(data->eye.c, x, y, aa));
	else
		return (cam_quaternion(data, x, y, aa));
}

///////////////////////////////////////////////////////////////////////////////]
// c->v = calc_angle_view(data->eye.c, x - SZ_X / 2, y - SZ_Y / 2);
// x between [-screenX/2, screenX/2] 
t_vect	cam_vector(t_camera *cam, int x, int y, int aa)
{
	t_vect	rtrn;
	double	cx;
	double	cy;

	x -= SZ_X / 2 * aa;
	y -= SZ_Y / 2 * aa;
	cx = x * cam->fov_cst / aa;
	cy = -y * cam->fov_cst / aa;
	rtrn = cam->O.view;
	rtrn = (t_vect){
		rtrn.dx + cx * cam->O.right.dx + cy * cam->O.up.dx,
		rtrn.dy + cx * cam->O.right.dy + cy * cam->O.up.dy,
		rtrn.dz + cx * cam->O.right.dz + cy * cam->O.up.dz,
	};
	ft_normalize_vect(&rtrn);
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	cam_quaternion(t_data *data, int x, int y, int aa)
{
	double		angle_a;
	double		angle_b;

	angle_a = atan((x - SZ_X / 2 * aa) * data->eye.c->fov_cst / aa);
	angle_b = atan((y - SZ_Y / 2 * aa) * data->eye.c->fov_cst / aa);
	return (dbl_quaternion_rota(&data->eye.c->O, angle_a, angle_b));
}
