/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AB_helpers_img.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/04/01 12:48:28 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	update_mat_w_txt(t_c_px *calcul, t_obj2 *obj, double u, double v);
int		is_there_txt(t_param *p);
t_argb	return_px_img(t_img *img, double x, double y);
t_vect	return_vect_img(t_img *img, double x, double y);
int		return_alpha_img(t_img *img, double x, double y);

///////////////////////////////////////////////////////////////////////////////]
void	update_mat_w_txt(t_c_px *calcul, t_obj2 *obj, double u, double v)
{
	double	rough;

	if (obj->param.s_map)
		calcul->mat.sp = return_alpha_img(obj->param.s_map, u, v) / 255.0;
	if (obj->param.rough_map)
	{
		rough = return_alpha_img(obj->param.rough_map, u, v) / 255.0;
		calcul->mat.sh = fmax(0.01, 2.0 / (rough * rough + 0.001) - 2.0);
	}
	if (obj->param.txt)
		calcul->mat.argb = return_px_img(obj->param.txt, u, v);
	if (obj->param.a_map)
		calcul->mat.argb.a = return_alpha_img(obj->param.a_map, u, v);
	if (obj->param.ao_map)
		calcul->mat.ao = return_alpha_img(obj->param.ao_map, u, v) / 255.0;
}

///////////////////////////////////////////////////////////////////////////////]
int	is_there_txt(t_param *p)
{
	if (p->txt || p->n_map || p->a_map || p->ao_map || p->s_map || p->rough_map)
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
t_argb	return_px_img(t_img *img, double x, double y)
{
	int		pixel;
	int		xy[2];
	t_argb	rtrn;

	xy[0] = max(0, (int)floor(x * img->sz_x));
	xy[1] = max(0, (int)floor(y * img->sz_y));
	if (xy[0] >= img->sz_x)
		xy[0] = img->sz_x - 1;
	if (xy[1] >= img->sz_y)
		xy[1] = img->sz_y - 1;
	pixel = *(unsigned int *)(img->addr + (xy[1] * img->ll + xy[0] \
		* (img->bpp / 8)));
	rtrn = (t_argb){
		(pixel >> 24) & 0xFF,
		(pixel >> 16) & 0xFF,
		(pixel >> 8) & 0xFF,
		pixel & 0xFF
	};
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
t_vect	return_vect_img(t_img *img, double x, double y)
{
	int		pixel;
	int		xy[2];
	t_vect	rtrn;

	xy[0] = max(0, (int)floor(x * img->sz_x));
	xy[1] = max(0, (int)floor(y * img->sz_y));
	if (xy[0] >= img->sz_x)
		xy[0] = img->sz_x - 1;
	if (xy[1] >= img->sz_y)
		xy[1] = img->sz_y - 1;
	pixel = *(unsigned int *)(img->addr + (xy[1] * img->ll + xy[0] \
		* (img->bpp / 8)));
	rtrn = (t_vect){
		((pixel >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((pixel >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		(pixel & 0xFF) / 255.0 * 2.0 - 1.0
	};
	ft_normalize_vect(&rtrn);
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// White [255] = 1.0; Black [0] = 0.0
int	return_alpha_img(t_img *img, double x, double y)
{
	int		pixel;
	int		xy[2];
	t_rgb	rtrn;

	xy[0] = max(0, (int)floor(x * img->sz_x));
	xy[1] = max(0, (int)floor(y * img->sz_y));
	if (xy[0] >= img->sz_x)
		xy[0] = img->sz_x - 1;
	if (xy[1] >= img->sz_y)
		xy[1] = img->sz_y - 1;
	pixel = *(unsigned int *)(img->addr + (xy[1] * img->ll + xy[0] \
		* (img->bpp / 8)));
	rtrn = (t_rgb){
		(pixel >> 16) & 0xFF,
		(pixel >> 8) & 0xFF,
		pixel & 0xFF
	};
	return ((rtrn.r + rtrn.g + rtrn.b) / 3);
}
