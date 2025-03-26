/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AB_helpers_img.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/04 11:16:32 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

///////////////////////////////////////////////////////////////////////////////]
t_argb	return_px_img(t_img *img, double x, double y);
t_vect	return_vect_img(t_img *img, double x, double y);
int		return_alpha_img(t_img *img, double x, double y);
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
	pixel = *(unsigned int *)(img->addr + (xy[1] * img->ll + xy[0] * (img->bpp / 8)));
	rtrn = (t_argb){
		(pixel >> 24) & 0xFF,
		(pixel >> 16) & 0xFF,
		(pixel >> 8) & 0xFF,
		pixel & 0xFF
	};
	return (rtrn);
}

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
	pixel = *(unsigned int *)(img->addr + (xy[1] * img->ll + xy[0] * (img->bpp / 8)));
	rtrn = (t_vect){
		((pixel >> 16) & 0xFF) / 255.0 * 2.0 - 1.0,
		((pixel >> 8) & 0xFF) / 255.0 * 2.0 - 1.0,
		(pixel & 0xFF) / 255.0 * 2.0 - 1.0
	};
	ft_normalize_vect(&rtrn);
	return (rtrn);
}

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
	pixel = *(unsigned int *)(img->addr + (xy[1] * img->ll + xy[0] * (img->bpp / 8)));
	rtrn = (t_rgb){
		(pixel >> 16) & 0xFF,
		(pixel >> 8) & 0xFF,
		pixel & 0xFF
	};
	return ((rtrn.r + rtrn.g + rtrn.b) / 3);
}
