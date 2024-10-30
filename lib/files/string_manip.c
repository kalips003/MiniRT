/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_manip.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 20:34:49 by agallon           #+#    #+#             */
/*   Updated: 2024/09/03 17:31:09 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

char	*fjoin(int str_num, int bit_32, ...);
int		find_str(char *str, char *str_pattern);
int		same_str(char *str, char *str_pattern);
char	*trim(char *input, char *dico);

///////////////////////////////////////////////////////////////////////////////]
//  #   join the read bits free with info from bit
char	*fjoin(int str_num, int bit_32, ...)
{
	va_list		args;
	int			i;
	char		*rtrn;

	va_start(args, bit_32);
	rtrn = NULL;
	i = -1;
	while (++i < str_num && i < 32)
		rtrn = join(rtrn, (char *)va_arg(args, char *), \
		((bit_32 >> (str_num - i - 1)) & 1) + 2 * !!rtrn, 0);
	va_end(args);
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
//  #   FIND SUBSTRING, RETURN POSI, -1 IF NOT FOUND
int	find_str(char *str, char *str_pattern)
{
	int	i;
	int	j;
	int	k;
	int	len_max;

	i = -1;
	if (!str || !str_pattern)
		return (-1);
	len_max = len(str) - len(str_pattern);
	while (str[++i] && i <= len_max)
	{
		j = -1;
		k = i;
		while (str_pattern[++j] && str[k] == str_pattern[j])
			k++;
		if (!str_pattern[j])
			return (i);
	}
	return (-1);
}

///////////////////////////////////////////////////////////////////////////////]
//  #   FIND EXACT STRING return 1
// return 0 if not
int	same_str(char *str, char *str_pattern)
{
	int	lenght;
	int	lenght2;

	lenght = len(str);
	lenght2 = len(str_pattern);
	if (lenght == lenght2 && !find_str(str, str_pattern))
		return (1);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// trim leading and trailing dico
// free the original
// 	" \n\t"
char	*trim(char *input, char *dico)
{
	char	*rtrn;
	char	*start;
	int		lenght;

	if (!input)
		return (NULL);
	start = input;
	while (wii(*start, dico) != -1)
		start++;
	if (!*start)
		return (free_s(input), NULL);
	lenght = len(start);
	while (lenght > 0 && wii(start[lenght - 1], dico) >= 0)
		lenght--;
	rtrn = str("%1.*s", lenght, start);
	free_s(input);
	return (rtrn);
}
