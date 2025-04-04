/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Key_2_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/04 19:58:31 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void		print_clic(t_data *data, int x, int y);
static void	h_print_1(t_data *data, t_c_px *c, int x, int y);
static void	h_print_2(t_c_px *c);

///////////////////////////////////////////////////////////////////////////////]

static const char	*g_obj_names[] = {
	"Circle",
	"Sphere",
	"Plane",
	"Cylinder",
	"Cone",
	"Hyperboloid",
	"Paraboloid",
	"Arrow",
	"Sprite",
	"Object",
	NULL
};

///////////////////////////////////////////////////////////////////////////////]
void	print_clic(t_data *data, int x, int y)
{
	t_c_px	c;

	ft_memset(&c, 0, sizeof(t_c_px));
	ini_stack(data, &c);
	c.c0 = data->eye.c->O.c0;
	c.print = 1;
	c.data = data;
	c.v = v_cam(data, x, y, NOT_AA);
	h_print_1(data, &c, x, y);
	if (c.object && data->change_function == f_render_normal_arrow)
		render_normal(data, &c);
	else if (c.object && data->change_function == f_render_v_space)
		h_render_v_space(data, &c.object->O);
	h_print_2(&c);
	if (c.object == data->change_obj)
		data->change_obj = NULL;
	else
		data->change_obj = c.object;
	printf(C_440"OBJECT SELECTED %p\n", data->change_obj);
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_print_1(t_data *data, t_c_px *c, int x, int y)
{
	printf(CLEAR);
	printf(C_512"Camera\t[%.1f,%.1f,%.1f\t%.3f,%.3f,%.3f]\n\n", \
		data->eye.c->O.c0.x, data->eye.c->O.c0.y, data->eye.c->O.c0.z, \
		data->eye.c->O.view.dx, data->eye.c->O.view.dy, data->eye.c->O.view.dz);
	printf("\tview \t[% .2f,% .2f,% .2f]\n", data->eye.c->O.view.dx, \
		data->eye.c->O.view.dy, data->eye.c->O.view.dz);
	printf("\tup \t[% .2f,% .2f,% .2f]\n", data->eye.c->O.up.dx, \
		data->eye.c->O.up.dy, data->eye.c->O.up.dz);
	printf("\tright \t[% .2f,% .2f,% .2f]\n", data->eye.c->O.right.dx, \
		data->eye.c->O.right.dy, data->eye.c->O.right.dz);
	printf(C_323"\nMouse clicked at (%d, %d)\tv: [%.3f,%.3f,%.3f]\n", x, y, \
		c->v.dx, c->v.dy, c->v.dz);
	put(C_244"\n-------------------[INSIDE PRINT DATA]-------------------\n");
	calc_px_color(data, c, PRINT_DEPTH);
	printf(C_244"---------------------------------------------------------\n");
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_print_2(t_c_px *c)
{
	t_obj2	*obj;
	t_obj	*o;

	if (c->object)
	{
		obj = c->object;
		o = &obj->O;
		printf(C_142"\nOBJECT:\t\t%s", g_obj_names[obj->type]);
		put(C_533"\n\n\txyz \t[%.1f,%.1f,%.1f]\n", o->c0.x, o->c0.y, o->c0.z);
		printf(C_455"\n\tview \t[%.2f,%.2f,%.2f]\n", o->view.dx, o->view.dy, \
			o->view.dz);
		printf("\tup \t[% .2f,% .2f,% .2f]\n", o->up.dx, o->up.dy, o->up.dz);
		printf("\tright \t[% .2f,% .2f,% .2f]\n", o->right.dx, o->right.dy, \
			o->right.dz);
		printf(C_433"\n\tcolor obj\t[%d,%d,%d]\n", obj->param.argb.r, \
			obj->param.argb.g, obj->param.argb.b);
		printf(C_433"\tpx color\t[%d,%d,%d]\n\n", c->mat.argb.r, \
			c->mat.argb.g, c->mat.argb.b);
	}
	printf(C_134"Vector normal surface\t[%.3f, %.3f, %.3f]\n", c->vn.dx, \
		c->vn.dy, c->vn.dz);
	printf("Intersection point\t[%.2f, %.2f, %.2f]\n", c->inter.x, \
		c->inter.y, c->inter.z);
	printf("Distance Intersection\t[%.2f]\n\n", c->dist);
}
