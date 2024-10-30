/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_e+.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 16:35:59 by agallon           #+#    #+#             */
/*   Updated: 2024/09/03 17:30:49 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ft_printf.h"

//////////////////////////////////////////////////////////// (%e)(%E)
int	ft_scientific(va_list args, t_flags *f)
{
	double	exp;
	int		size;
	int		num_size;

	exp = va_arg(args, double);
	num_size = size_exp(f);
	size = 0;
	if (!f->minus)
		size += f_spacing(f, num_size + size_format_num(exp, f, num_size));
	size += f_format_num(f, exp, num_size);
	size += ft_put_science(exp, f);
	if (f->minus)
		size += f_spacing(f, num_size + size_format_num(exp, f, num_size));
	return (size);
}

//////////////////////////////////////////////////////////// (%S)
// [ % S ] > spacing, show white space
// [ %*S ] > print memory
// [ %.*S ] > \n sizeof (bit)
// [ %#S ] > hexadecimal
// [ %-#S ] > no 2 precision for hexa
// [ %+S ] > colors +128
int	ft_string_hexa(va_list args, t_flags *f)
{
	char	*str;
	int		i;

	str = va_arg(args, char *);
	if (!str)
		return (put(BLINK REVERSE "NULL" RESET) - 12);
	i = -1;
	while (++i < f->width || (!f->width && str[i]))
	{
		if (f->preci && i % f->preci == 0)
			put(RESET"\n");
		put("\033[38;5;0;48;5;%um", ((unsigned char)str[i] + 128 * \
			(f->plus)) % 256);
		if (f->hash)
			f->size += put("%.*x", 2 - f->minus, (unsigned char)str[i]);
		else
			f->size += put("%c", str[i]);
		if (f->space)
			f->size += put(" ");
	}
	put(RESET);
	return (f->size);
}

//////////////////////////////////////////////////////////// (%t)
// [ %.*S ] > put * \t in front of the tab
// [ %-S ] > dont put \n after each line (for gnl return)
// [ %#S ] > dont put \t on first line
// [ %+S ] > add char * in front of each line
int	ft_tab(va_list args, t_flags *f)
{
	char	**tab;
	char	*line;
	int		i;

	i = -1;
	line = NULL;
	tab = (char **)va_arg(args, char *);
	if (!tab)
		return (put(BLINK REVERSE "NULL" RESET) - 12);
	if (f->plus)
	{
		line = (char *)va_arg(args, char *);
		if (!line)
			return (put("ARG MISSING") - 12);
		while (tab[++i])
			f->size += put("%.*c%s%s%.*c", f->preci * !(f->hash && (i == 0)),
					' ', line, tab[i], f->minus, '\n');
	}
	else
		while (tab[++i])
			f->size += put("%.*c%s%.*c", f->preci * !(f->hash && (i == 0)),
					' ', tab[i], f->minus, '\n');
	return (f->size);
}
