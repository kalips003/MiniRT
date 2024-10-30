/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   W_wildcard2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 11:29:53 by kalipso           #+#    #+#             */
/*   Updated: 2024/09/08 13:29:42 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	**cut_pattern(char *pattern);
char	**return_all_match(t_folder *first, int size_w);
int		is_a_match(char *pattern, char *string);
char	**expand_tab_tab(char **input, char **to_add, int bit);

///////////////////////////////////////////////////////////////////////////////]
/// cut */*/*.c
// 	abc/hello/de*f/ghi.* > {abc, hello, les, ami, de*f, ghi.*}
// cut the path arg:
// 		 path/*/to_some*/file/that/may/exist/m*a*y*b*e.*
// return un tableau avec les folders qui contiennent des * separe:
// 		> {path, *, to_some*, file/that/may/exist, m*a*y*b*e.*}
char	**cut_pattern(char *pattern)
{
	char	**rtrn;
	int		i;

	i = -1;
	rtrn = split(pattern, "/");
	if (!rtrn)
		return (NULL);
	// if (*pattern == '/')
	// 	rtrn[0] = join("/", rtrn[0], 0b01, 0);
	while (rtrn[++i])
	{
		if (wii('*', rtrn[i]) >= 0)
			continue ;
		else if (wii('*', rtrn[i + 1]) == -1)
		{
			rtrn[i] = fjoin(3, 0b100, rtrn[i], "/", rtrn[i + 1]);
			rtrn = pop_tab(rtrn, rtrn[i + 1]);
			i--;
		}
	}
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// return the tab of all files with matching name
char	**return_all_match(t_folder *first, int size_w)
{
	char		**rtrn;
	t_folder	**leaf;

	rtrn = NULL;
	if (!first)
		return (NULL);
	leaf = first->next;
	if (leaf)
	{
		leaf = first->next - 1;
		while (*(++leaf))
			rtrn = expand_tab_tab(rtrn, return_all_match(*leaf, size_w), 0b11);
	}
	else if (first->floor == size_w - 1)
	{
		rtrn = first->all_files;
		first->all_files = NULL;
	}
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
/// return 1 if is a match, 0 if not
/// we match 'ab*cd*f*' with each file in current directory 'ab....cd....f...'
// 		'a*b' > 'a..................b'
int	is_a_match(char *pattern, char *string)
{
	if (!pattern || !string)
		return (pattern == string);
	if (!*pattern && !*string)
		return (1);
	if (*pattern == '*')
		return (is_a_match(pattern + 1, string)
			|| (*string && is_a_match(pattern, string + 1)));
	if (*pattern == *string)
		return (is_a_match(pattern + 1, string + 1));
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
/// if 0b10, free input
/// if 0b01, free to_add
char	**expand_tab_tab(char **input, char **to_add, int bit)
{
	char	**ptr;
	char	**rtrn;

	rtrn = NULL;
	ptr = input;
	if (!input || !to_add)
		return ((char **)((size_t)input + (size_t)to_add));
	if (ptr && ptr--)
		while (*(++ptr))
			rtrn = expand_tab(rtrn, *ptr);
	ptr = to_add;
	if (ptr && ptr--)
		while (*(++ptr))
			rtrn = expand_tab(rtrn, *ptr);
	if (bit & 1)
		free_s(to_add);
	if (bit >> 1 & 1)
		free_s(input);
	return (rtrn);
}
