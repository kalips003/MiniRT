/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expend_free.c	                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agallon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 20:34:49 by agallon           #+#    #+#             */
/*   Updated: 2023/11/09 20:35:24 by agallon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/libft.h"

void	*expand(void *ptr, int size, int add);
void	**expand_tab(void **tab, void *new_line);
void	*free_tab(char **tab);
void	*free_s(void *ptr);

///////////////////////////////////////////////////////////////////////////////]
void	*expand(void *ptr, int size, int add)
{
	void	*rtrn;
	int		i;

	rtrn = malloc(size + add);
	if (!rtrn)
		return (put(ERRM), NULL);
	i = -1;
	while (++i < size && ptr)
		((char *)rtrn)[i] = ((char *)ptr)[i];
	while (i++ < size + add)
		((char *)rtrn)[i - 1] = 0;
	if (ptr)
		free(ptr);
	return (rtrn);
}

void	**expand_tab(void **tab, void *new_line)
{
	void	**new_tab;
	int		i;

	if (!new_line)
		return (tab);
	new_tab = malloc(sizeof(void *) * (tab_size((char **)tab) + 2));
	if (!new_tab)
		return (printf("error malloc 1\n"), free_tab((char **)tab), NULL);
	i = 0;
	while (tab && tab[i])
	{
		new_tab[i] = tab[i];
		i++;
	}
	new_tab[i] = new_line;
	new_tab[i + 1] = NULL;
	free_s(tab);
	return (new_tab);
}

///////////////////////////////////////////////////////////////////////////////]
// yes, its secure for NULL tab
void	*free_tab(char **tab)
{
	int	i;

	if (!tab)
		return (NULL);
	i = -1;
	while (tab[++i])
		free_s(tab[i]);
	free_s(tab);
	return (NULL);
}

void	*free_s(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}
