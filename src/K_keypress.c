/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   K_keypress.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/12/14 15:53:23 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	key_press(int keysym, t_data *data);
int	key_release(int keysym, t_data *data);
int	mouse_clic(int button, int x, int y, void *data);
void	print_clic(t_data *data, int x, int y);

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
	t_calcul_px c;
	ft_memset(&c, 0, sizeof(t_calcul_px));
	c.c0 = data->eye.c->xyz;
	double angleB = atan((y - SIZE_SCREEN_Y / 2) * data->eye.c->fov_cst_y);
	double angleA = atan((x - SIZE_SCREEN_X / 2) * data->eye.c->fov_cst_x);

	f_calculate_combined_quaternion(data, angleA, angleB, &c.v_view);
	calculate_pixel_color(data, &c);

	
	printf("Mouse clicked at position (%d, %d)\n", x, y);
	printf("Vector view = [%f, %f, %f]\n", c.v_view.dx, c.v_view.dy, c.v_view.dz);
	printf("Camera = [%f, %f, %f]\n", data->eye.c->xyz.x, data->eye.c->xyz.y, data->eye.c->xyz.z);
	printf("Vector normal surface = [%f, %f, %f]\n", c.v_normal.dx, c.v_normal.dy, c.v_normal.dz);
	printf("Intersection point = [%f, %f, %f]\n", c.inter.x, c.inter.y, c.inter.z);
	printf("Mouse clicked at position (%d, %d)\n", x, y);

	c.dist = -1.0;
	c.c0 = c.inter;
	t_sphere	**sphere_ptr;
	sphere_ptr = data->spheres - 1;
	double dist_from_light = sqrt(c.v_view.dx * c.v_view.dx + c.v_view.dy * c.v_view.dy + c.v_view.dz * c.v_view.dz);
	while (++sphere_ptr && *sphere_ptr)
		if (distance_from_sphere(&c, *sphere_ptr) >= 0.0 && c.dist < dist_from_light)
		{
			printf("AAA%f\n", c.dist);
			printf("Vector view = [%f, %f, %f]\n", c.v_view.dx, c.v_view.dy, c.v_view.dz);

		}
}


