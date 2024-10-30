/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P2_extract_w.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 08:28:22 by kalipso           #+#    #+#             */
/*   Updated: 2024/09/07 16:18:10 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char		*ft_extract_words_v2(char *raw_line, int *i);
char		*ft_extract_quotes_v2(char *raw_line, int *i, char quote);

///////////////////////////////////////////////////////////////////////////////]
// started at the beginning of a word
// "      "' sdaf a'"dklfj ;a:'"$ABV
// takes the START of the raw, and work with the &i
// increment i
// stops when find: " \t\n|&<>;"
char	*ft_extract_words_v2(char *raw_line, int *i)
{
	int		len;
	char	*rtrn;

	rtrn = NULL;
	while (raw_line[*i])
	{
		if (wii(raw_line[*i], " \t\n|&<>;") >= 0)
			break ;
		if (raw_line[*i] == '\'' || raw_line[*i] == '"')
			rtrn = join(rtrn, ft_extract_quotes_v2(&raw_line[*i],
						i, raw_line[*i]), 0b11, 0);
		else
		{
			len = len_m(&raw_line[*i], " \n\t|&<>;\"\'");
			rtrn = join(rtrn, str("%1.*s", len, &raw_line[*i]), 0b11, 0);
			*i += len;
		}
	}
	return (rtrn);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	prend pointer to the beginning of a quote
		> "text
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
char	*ft_extract_quotes_v2(char *raw_line, int *i, char quote)
{
	char	*rtrn;
	char	*rtrn2;
	int		size;
	int		size1;
	int		i_bidon;

	size = wii(quote, &raw_line[1]);
	size1 = len_c(&raw_line[1], quote) + 2 - (size == -1);
	if (size1 == 2 && !size)
		return (++(*i), ++(*i), NULL);
	rtrn = str("%1.*s", size1, raw_line);
	if (size == -1)
	{
		(*i) += len(raw_line);
		rtrn2 = readline(C_413"finish your quotes: "RESET);
		if (!rtrn2)
			print_fd(2, "unexpected EOF while looking for matching`\""
				"'\nsyntax error: unexpected end of file\n");
		rtrn = fjoin(3, 0b101, rtrn, "\n", rtrn2);
		rtrn2 = ft_extract_quotes_v2(rtrn, &i_bidon, quote);
		free_s(rtrn);
		return (rtrn2);
	}
	(*i) += size1;
	return (rtrn);
}
