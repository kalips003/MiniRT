/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   K_keypress.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/12/01 18:22:33 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int		key_press(int keysym, t_data *data);
int		key_release(int keysym, t_data *data);
int 	mouse_clic(int button, int x, int y, void *data);
void	print_clic(t_data *data, int x, int y);
void	rotation_camera(t_data *data, t_vect *axis_rota, int posi_neg);

///////////////////////////////////////////////////////////////////////////////]
int	key_press(int keysym, t_data *data)
{
	if (keysym == XK_Escape)
		end2(data);
	if (keysym == XK_KP_Enter)
	{
		data->is_not_moving = 1;
		ft_render_frame_plus(data);
		return (0);
	}
	else
		data->is_not_moving = 0;

	if (keysym == XK_Up)
	{
		data->eye.c->xyz.x += 5.0 * data->eye.c->view.dx;
		data->eye.c->xyz.y += 5.0 * data->eye.c->view.dy;
		data->eye.c->xyz.z += 5.0 * data->eye.c->view.dz;
	}
	else if (keysym == XK_Down)
	{
		data->eye.c->xyz.x -= 5.0 * data->eye.c->view.dx;
		data->eye.c->xyz.y -= 5.0 * data->eye.c->view.dy;
		data->eye.c->xyz.z -= 5.0 * data->eye.c->view.dz;
	}
	else if (keysym == XK_Right)
	{
		data->eye.c->xyz.x -= 5.0 * data->eye.c->right.dx;
		data->eye.c->xyz.y -= 5.0 * data->eye.c->right.dy;
		data->eye.c->xyz.z -= 5.0 * data->eye.c->right.dz;
	}
	else if (keysym == XK_Left)
	{
		data->eye.c->xyz.x += 5.0 * data->eye.c->right.dx;
		data->eye.c->xyz.y += 5.0 * data->eye.c->right.dy;
		data->eye.c->xyz.z += 5.0 * data->eye.c->right.dz;
	}

	else if (keysym == XK_a)
		rotation_camera(data, &data->eye.c->up, -1);
	else if (keysym == XK_d)
		rotation_camera(data, &data->eye.c->up, 1);
	else if (keysym == XK_w)
		rotation_camera(data, &data->eye.c->right, 1);
	else if (keysym == XK_s)
		rotation_camera(data, &data->eye.c->right, -1);
	else if (keysym == XK_n)
	{
		data->eye.current_camera++;
		data->eye.c = data->camera[data->eye.current_camera];
		if (!data->eye.c)
		{
			data->eye.c = data->camera[0];
			data->eye.current_camera = 0;
		}
	}
	else
		return (0);
	ft_render_frame(data);
	return (0);
}

int	key_release(int keysym, t_data *data)
{
	// if (keysym == XK_d || keysym == XK_Right || 
	// keysym == XK_s || keysym == XK_Down || 
	// keysym == XK_a || keysym == XK_Left ||
	// keysym == XK_w || keysym == XK_Up || keysym == XK_n)
	// 	ft_render_rt(data);
	// if (keysym == XK_Control_L && !data->player.time)
	// 	data->running ^= 1;
	// if (keysym == XK_e)
	// 	print_data(data);
	// if (keysym == XK_space)
	// 	data->running = 0;
	return (0);
}

// int mlx_mouse_hook(void *win_ptr, int (*funct_ptr)(), void *param);
// int mlx_hook(void *win_ptr, int x_event, int x_mask, int (*funct_ptr)(), void *param);
// ButtonPress = 4, ButtonRelease = 5, MotionNotify = 6
// Left Button: 1, Middle Button: 2, Right Button: 3, Scroll Up: 4, Scroll Down: 5
int mouse_clic(int button, int x, int y, void *data)
{
	if (button == 3) // Right mouse button
		print_clic((t_data *)data, x, y);
	return (0);
}

void	print_clic(t_data *data, int x, int y)
{
	printf(CLEAR);
	t_calcul c;
	ft_memset(&c, 0, sizeof(t_calcul));
	c.origin = data->eye.c->xyz;
	f_calculate_combined_quaternion(data, data->eye.px0 + x * data->eye.px, data->eye.py0 + y * data->eye.px, &c.v_rotated);
	calculate_pixel_color(data, &c);

	
	printf("Mouse clicked at position (%d, %d)\n", x, y);
	printf("Vector view = [%f, %f, %f]\n", c.v_rotated.dx, c.v_rotated.dy, c.v_rotated.dz);
	printf("Camera = [%f, %f, %f]\n", data->eye.c->xyz.x, data->eye.c->xyz.y, data->eye.c->xyz.z);
	printf("Vector normal surface = [%f, %f, %f]\n", c.vect_norm.dx, c.vect_norm.dy, c.vect_norm.dz);
	printf("Intersection point = [%f, %f, %f]\n", c.inter_point.x, c.inter_point.y, c.inter_point.z);
	printf("Mouse clicked at position (%d, %d)\n", x, y);

	c.dist = -1.0;
	c.origin = c.inter_point;
	t_sphere	**sphere_ptr;
	sphere_ptr = data->spheres - 1;
	double dist_from_light = sqrt(c.v_rotated.dx * c.v_rotated.dx + c.v_rotated.dy * c.v_rotated.dy + c.v_rotated.dz * c.v_rotated.dz);
	while (++sphere_ptr && *sphere_ptr)
		if (distance_from_sphere(data, &c, *sphere_ptr) >= 0.0 && c.dist < dist_from_light)
		{
			printf("AAA%f\n", c.dist);
			printf("Vector view = [%f, %f, %f]\n", c.v_rotated.dx, c.v_rotated.dy, c.v_rotated.dz);

		}
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