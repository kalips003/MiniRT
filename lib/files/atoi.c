/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 20:34:49 by agallon           #+#    #+#             */
/*   Updated: 2024/11/28 16:52:08 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

int			atoi_v(const char *str);
int			ft_atoi(char *str, int *error);
int			ft_atoi_v2(char *str, int *i, int *error);
static int	what_is_it(char c);

///////////////////////////////////////////////////////////////////////////////]
//  atoi strict
// 	set error (-1) if no number
// 	set error (-2) if too big for int
// 	set error (-3) if 2a
int	ft_atoi(char *str, int *error)
{
	long long	rtrn;
	int			i;
	int			sign;

	i = 0;
	rtrn = 0;
	sign = 1;
	if (!str || wii(str[0], "0123456789+-") < 0)
		*error = -1;
	if (((str[0] == '-' && ++(*error)) || str[0] == '+') && ++i)
		sign -= 2 * (str[0] == '-');
	if (!(str[i] >= '0' && str[i] <= '9'))
		*error = -4;
	while (str[i] >= '0' && str[i] <= '9')
		rtrn = rtrn * 10 + (str[i++] - '0');
	rtrn *= sign;
	if (rtrn > 2147483647 || rtrn < -2147483648)
		*error = -2;
	if (!(!str[i] || str[i] == ' ' || str[i] == '\n'))
		*error = -3;
	return ((int)rtrn);
}

//  atoi int, simple, for small num
//  handle multiple white spaces and multiple signs
int	atoi_v(const char *str)
{
	int	i;
	int	result;
	int	sign;

	sign = 1;
	result = 0;
	i = 0;
	while (str[i] && what_is_it(str[i]) == 1)
		i++;
	while (str[i] && what_is_it(str[i]) >= 3)
	{
		if (what_is_it(str[i]) == 4)
			sign = -sign;
		i++;
	}
	while (what_is_it(str[i]) == 2)
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

//  atoi iterate on its own
int	ft_atoi_v2(char *str, int *i, int *error)
{
	long long	rtrn;
	int			sign;

	*i = 0;
	rtrn = 0;
	sign = 1;
	if (!str || wii(str[0], "0123456789+-") < 0)
		*error = -1;
	if (str[0] == '-' && ++*i)
		sign = -1;
	else if (str[0] == '+')
		i++;
	while (str[*i] >= '0' && str[*i] <= '9')
		rtrn = rtrn * 10 + (str[*i++] - '0');
	rtrn *= sign;
	if (rtrn > 2147483647 || rtrn < -2147483648)
		*error = -2;
	if (!(!str[*i] || str[*i] == ' ' || str[*i] == '\n'))
		*error = -3;
	return ((int)rtrn);
}

// wii special atoi
static int	what_is_it(char c)
{
	int	result;

	result = 0;
	if ((c >= 9 && c <= 13) || c == ' ')
		result = 1;
	if (c >= '0' && c <= '9')
		result = 2;
	if (c == '+')
		result = 3;
	if (c == '-')
		result = 4;
	return (result);
}
