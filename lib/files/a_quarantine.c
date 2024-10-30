/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_quarantine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agallon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 20:34:49 by agallon           #+#    #+#             */
/*   Updated: 2024/05/13 17:57:12 by agallon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

int	len_c(char *s, char c);

//////////////////////////////////////////////////////////// (%S)
//	%<size>S return ptr to copied memory
char	*pt_memcpy(char *str)
{
	return (str);
}

//////////////////////////////////////////////////////////// (%I)
//	%I return ptr to written int tab
// 	int **tab = str(1,2,3,-4,5\n3,-1,-5,1\n);
/*
	= [{3} ;
		(5) 1,2,3,-4,5 ;
		(4) 3,-1,-5,1 ;
		(0) ]
*/
int	**pt_numtab(char *num_tab)
{
	return ((int **)num_tab);
}

//  return lenght, stops when the char c is found
int	len_c(char *s, char c)
{
	int		i;

	i = 0;
	if (!s)
		return (-1);
	while (s[i] && s[i] != c)
		i++;
	return (i);
}
