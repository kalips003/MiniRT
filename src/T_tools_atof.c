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

double	ft_atof(char *string, int *error);
int		ato_coor(char *str, t_coor *xyz);
int		ato_rgb(char *str, t_rgb *rgb);

///////////////////////////////////////////////////////////////////////////////]
// err redondant?
double	ft_atof(char *string, int *error)
{
	double rtrn = 0.0;

	char **tab = split(string, ".");
	if (!tab || tab_size(tab) > 2)
		return (free_tab(tab), put(ERR6"(%s) not a correct number\n", string), (*error)++, rtrn);
	int err = 0;
	rtrn = ft_atoi(tab[0], &err);
	if (err < 0)
		return (put(ERR7"(%s) not a correct number\n", string), free_tab(tab), (*error)++, rtrn);
	if (tab[1])
	{
		double fraction = ft_atoi(tab[1], &err);
		if (err < 0 || fraction < 0)
			return (put(ERR8"(%s) bad fractional part\n", tab[1]), free_tab(tab), (*error)++, rtrn);
		int	frac_len = len(tab[1]);
		while (frac_len-- != 0)
			fraction /= 10.0;
		rtrn += fraction;
	}
	free_tab(tab);
	if (err == 1 && rtrn > 0)
		rtrn *= -1;
	return (rtrn);
}

// a.0,b.0,c.0
int	ato_coor(char *str, t_coor *xyz)
{
	char	**spl = split(str, ",");
	if (tab_size(spl) != 3)
		return (put(ERR8"(%s) bad coordonates\n", str), free_tab(spl), 1);
	int err = 0;
	xyz->x = ft_atof(spl[0], &err);
	xyz->y = ft_atof(spl[1], &err);
	xyz->z = ft_atof(spl[2], &err);
	free_tab(spl);
	if (err)
		return (put(ERR8"(%s) bad coordonates\n", str), 1);
	return (0);
}

// a,b,c [0-255]
int	ato_rgb(char *str, t_rgb *rgb)
{
	char	**spl = split(str, ",");
	if (tab_size(spl) != 3)
		return (put(ERR8"(%s) bad colors\n", str), free_tab(spl), 1);
	int err = 0;
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
