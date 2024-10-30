/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 20:38:07 by agallon           #+#    #+#             */
/*   Updated: 2024/07/16 02:22:02 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

char	*ft_memset(void *s, char c, int size);
void	*mem(char c, int size);

///////////////////////////////////////////////////////////////////////////////]
//  #   set memory block
char	*ft_memset(void *s, char c, int size)
{
	char	*str;
	int		i;

	i = -1;
	if (!s)
		return (NULL);
	str = (char *)s;
	while (++i < size)
		str[i] = c;
	return (str);
}

//  #   better malloc, that set memory
void	*mem(char c, int size)
{
	void	*str;
	int		i;

	if (!size)
		return (NULL);
	str = malloc(size);
	if (!str)
		return (NULL);
	i = -1;
	while (++i < size)
		((char *)str)[i] = c;
	return (str);
}
