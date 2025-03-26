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

void	*ft_memcpy(void *dest, void *src, unsigned n)
{
	unsigned	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	while (i++ < n)
		((char *)dest)[i - 1] = ((char *)src)[i - 1];
	return (dest);
}

///////////////////////////////////////////////////////////////////////////////]
// int	ft_cat_timing(void *data, int sublim)
// {
// 	// clock_t	start;
// 	// clock_t	end;
// 	struct timeval start;
// 	struct timeval end;
// 	double time;

// // start = clock();
// 	gettimeofday(&start, NULL);
// 	// ft_render_frame_multi(data, 2);
// 	gettimeofday(&end, NULL);
// // end = clock();

// 	time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
// 	printf(C_423"Run-time: "C_135"%.4f"C_423" seconds\n", time);
// 	return (0);
// }