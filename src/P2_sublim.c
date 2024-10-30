/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P2_sublim.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agallon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 16:26:09 by kalipso           #+#    #+#             */
/*   Updated: 2024/09/07 21:26:34 by agallon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void		sublim_pipe_v2(t_data *data, t_pipeline *pipe);
char		*ft_sublim_v2(t_data *data, char *string, int bit);
char		*sublim_dollar_v2(t_data *data, char *raw_arg, int bit);
char		*ft_dollar_v2(t_data *data, char *raw_dollar, int *i);
static char	*extract_word(char *raw_line, int *i);

///////////////////////////////////////////////////////////////////////////////]
// sublim one pipe
void	sublim_pipe_v2(t_data *data, t_pipeline *pipe)
{
	char	**ptr3;
	t_cmd	*cmd;

	cmd = pipe->cmd;
	while (cmd)
	{
		ptr3 = cmd->cmd_arg;
		while (ptr3 && *ptr3)
		{
			*ptr3 = ft_sublim_v2(data, *ptr3, 1);
			ptr3++;
		}
		if (cmd->in_bit != 2)
			cmd->in_file = ft_sublim_v2(data, cmd->in_file, 0);
		cmd->out_file = ft_sublim_v2(data, cmd->out_file, 1);
		cmd = cmd->next;
	}
}

///////////////////////////////////////////////////////////////////////////////]
// takes: "abc"'ab$5"4'baaac"$5"$5
// >	abcab$5"4baaac($5)($5)
// if bit, sublim $
char	*ft_sublim_v2(t_data *data, char *string, int bit)
{
	char	*sublim;
	int		i;

	if (!string)
		return (NULL);
	sublim = NULL;
	i = 0;
	while (string[i])
	{
		if (string[i] == '\'')
			sublim = join(sublim, trim(ft_extract_quotes_v2(&string[i],
							&i, '\''), "\'"), 0b11, 0);
		else if (string[i] == '\"')
			sublim = join(sublim, sublim_dollar_v2(data,
						trim(ft_extract_quotes_v2(&string[i], &i,
								'\"'), "\""), bit), 0b11, 0);
		else
			sublim = join(sublim, sublim_dollar_v2(data,
						extract_word(string, &i), bit), 0b11, 0);
	}
	if (!sublim)
		return (print_fd(2, ERR4"some error occured honey!\n"), string);
	free_s(string);
	return (sublim);
}

///////////////////////////////////////////////////////////////////////////////]
// if !bit, dont sublim dollar
// recieve: ecg$5321= i > ecg'($5) i
char	*sublim_dollar_v2(t_data *data, char *raw_arg, int bit)
{
	int		j;
	int		b;
	char	*rtrn;

	rtrn = NULL;
	j = 0;
	if (!bit || !raw_arg)
		return (raw_arg);
	while (raw_arg[j])
	{
		if (raw_arg[j] == '$')
			rtrn = join(rtrn, ft_dollar_v2(data, raw_arg, &j), 0b11, 0);
		else
		{
			b = len_m(&raw_arg[j], "$");
			rtrn = join(rtrn, &raw_arg[j], 0b10, b);
			j += b;
		}
	}
	free_s(raw_arg);
	return (rtrn);
}

///////////////////////////////////////////////////////////////////////////////]
// recieve: raw, and pointer to i
// 	return the value malloced of $
// 		advance i;
char	*ft_dollar_v2(t_data *data, char *raw_dollar, int *i)
{
	char	*rtrn;
	int		b;
	char	*var;

	rtrn = NULL;
	(*i)++;
	if (raw_dollar[*i] == '$' && ++(*i))
		return (str("%d", getpid()));
	else if (raw_dollar[*i] == '?' && ++(*i))
		return (str("%d", WEXITSTATUS(data->exit_code)));
	else
	{
		b = len_m(&raw_dollar[*i], " \t\n\r\v\f;|&=()[]{}!@#^*+-/\\~%?:,<>$\'\"");
		if (!b)
			return (str("$"));
		var = str("%1.*s=", b, &raw_dollar[*i]);
		*i += b;
		rtrn = str("%1s", rtrn_var_v2(data->env, var));
		free_s(var);
		return (rtrn);
	}
}

///////////////////////////////////////////////////////////////////////////////]
static char	*extract_word(char *raw_line, int *i)
{
	int		len;
	char	*rtrn;

	len = len_m(&raw_line[*i], "\'\"");
	rtrn = str("%1.*s", len, &raw_line[*i]);
	*i += len;
	return (rtrn);
}
