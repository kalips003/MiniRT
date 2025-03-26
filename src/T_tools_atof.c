/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   T_tools_atof.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2025/03/04 20:40:55 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int			ft_atof(char *string, double *rtrn);
static int	h_atof(char **tab, double *rtrn, int *err);
int			ato_coor(char *str, t_coor *xyz);
int			ato_rgb(char *str, t_rgb *rgb);
int			ato_argb(char *str, t_argb *argb);

///////////////////////////////////////////////////////////////////////////////]

int	ft_atof(char *string, double *rtrn)
{
	char	**tab;
	int		err;

	tab = split(string, ".");
	if (!tab || tab_size(tab) > 2)
		return (free_tab(tab), put(ERR6"(%s) not a correct float number\n", string), 1);
	err = 0;
	*rtrn = ft_atoi(tab[0], &err);
	if (err < 0)
		return (put(ERR7"(%s) not a correct number\n", string), free_tab(tab), 1);
	if (tab[1] && h_atof(tab, rtrn, &err))
		return (1);
	free_tab(tab);
	if (err == 1 && *rtrn > 0)
		*rtrn *= -1;
	return (0);
}

static int	h_atof(char **tab, double *rtrn, int *err)
{
	double	fraction;
	int		frac_len;

	fraction = ft_atoi(tab[1], err);
	if (*err < 0 || fraction < 0)
		return (printf(ERR8"(%s) bad fractional part\n", tab[1]), free_tab(tab), 1);
	frac_len = len(tab[1]);
	while (frac_len-- != 0)
		fraction /= 10.0;
	if (*rtrn < 0)
		fraction *= -1.0;
	*rtrn += fraction;
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// A.0,B,C.0
int	ato_coor(char *str, t_coor *xyz)
{
	char	**spl;
	int		err;

	spl = split(str, ",");
	if (tab_size(spl) != 3)
		return (put(ERR8"(%s) bad coordonates xyz\n", str), free_tab(spl), 1);
	err = 0;
	err += ft_atof(spl[0], &xyz->x);
	err += ft_atof(spl[1], &xyz->y);
	err += ft_atof(spl[2], &xyz->z);
	free_tab(spl);
	if (err)
		return (put(ERR8"(%s) bad coordonates xyz\n", str), 1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// a,b,c [0-255]
int	ato_rgb(char *str, t_rgb *rgb)
{
	char	**spl;
	int		err;

	spl = split(str, ",");
	if (tab_size(spl) != 3)
		return (put(ERR8"(%s) bad colors\n", str), free_tab(spl), 1);
	err = 0;
	rgb->r = ft_atoi(spl[0], &err);
	rgb->g = ft_atoi(spl[1], &err);
	rgb->b = ft_atoi(spl[2], &err);
	free_tab(spl);
	if (err)
		return (put(ERR8"(%s) bad colors\n", str), 1);
	if ((rgb->r | rgb->g | rgb->b) & 0xFFFFFF00)
		return (put(ERR8"(%s) rgb value should be [0-255]\n", str), 1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// a,b,c [0-255]
int	ato_argb(char *str, t_argb *argb)
{
	char	**spl;
	int		err;

	spl = split(str, ",");
	if (tab_size(spl) != 3)
		return (put(ERR8"(%s) bad colors\n", str), free_tab(spl), 1);
	err = 0;
	argb->a = 0;
	argb->r = ft_atoi(spl[0], &err);
	argb->g = ft_atoi(spl[1], &err);
	argb->b = ft_atoi(spl[2], &err);
	free_tab(spl);
	if (err)
		return (put(ERR8"(%s) bad colors\n", str), 1);
	if ((argb->r | argb->g | argb->b) & 0xFFFFFF00)
		return (put(ERR8"(%s) argb value should be [0-255]\n", str), 1);
	return (0);
}
