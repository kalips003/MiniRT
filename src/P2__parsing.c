/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   P2__parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 08:28:22 by kalipso           #+#    #+#             */
/*   Updated: 2024/09/07 17:19:11 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int				ft_parsing_v2(t_data *data, char *input);
t_cmd			*ft_mario_v2(t_data *data, char *raw_line, int *i, t_cmd *cmd);
static t_cmd	*ft_mario_v3(t_data *data, t_cmd *cmd, t_vtf *v);
void			ft_extract_redirection_v2(char *raw_line, int *i, t_cmd *cmd);

///////////////////////////////////////////////////////////////////////////////]
/*******************************************************************************

******************************************************************************/
int	ft_parsing_v2(t_data *data, char *input)
{
	int		i;
	t_cmd	*ptr;

	if (input[0] == '|' || input[0] == '&' || input[0] == ';')
		return (put(MSG_REDI, input[0]), 1);
	data->pipeline = new_cmd(data->pipeline, 0);
	if (!data->pipeline)
		return (put(ERRM), 1);
	ptr = data->pipeline->cmd;
	i = 0;
	while (input[i])
	{
		if (wii(input[i], " \n\t") >= 0 && ++i)
			continue ;
		else if (input[i] == '|' || input[i] == '&' || input[i] == ';')
			ptr = ft_mario_v2(data, input, &i, ptr);
		else if (input[i] == '<' || input[i] == '>')
			ft_extract_redirection_v2(input, &i, ptr);
		else
			ptr->cmd_arg = expand_tab(ptr->cmd_arg,
					ft_extract_words_v2(input, &i));
		if (!ptr || ptr->error)
			return (1);
	}
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// sorry MARIO, the princess is in another command
// takes the START of the raw, and work with the &i
// nombre de '|'
// nombre de '&'
// nombre de ';'
t_cmd	*ft_mario_v2(t_data *data, char *raw_line, int *i, t_cmd *cmd)
{
	t_vtf	v;

	v.abc = 0;
	v.count = 0;
	v.wii_rtrn = wii(raw_line[*i], "|&;");
	v.i = i;
	v.raw_line = raw_line;
	while (v.wii_rtrn >= 0 && ++(*i))
	{
		v.abc |= 1 << v.wii_rtrn;
		v.count++;
		v.wii_rtrn = wii(raw_line[*i], "|&;");
	}
	if (__builtin_popcount(v.abc) > 1 || v.count > 2 || (v.abc & (1 << 1)
			&& v.count != 2) || (v.abc & (1 << 2) && v.count != 1))
		return (put(MSG_REDI, raw_line[*i - 1]), NULL);
	while (wii(raw_line[*i], " \t\n") >= 0)
		(*i)++;
	return (ft_mario_v3(data, cmd, &v));
}

static t_cmd	*ft_mario_v3(t_data *data, t_cmd *cmd, t_vtf *v)
{
	if (wii(v->raw_line[*v->i], "|&;") >= 0 || !v->raw_line[*v->i])
		return (put(MSG_REDI, '|'), NULL);
	if ((v->abc & 1) && v->count == 1)
		return (new_node(cmd));
	else if ((v->abc & 1) && v->count == 2)
		return (new_cmd(data->pipeline, '|')->cmd);
	else if ((v->abc & (1 << 1)) && v->count == 1)
		return (put(ERR2"single '&' will be available "
				"next patch, coming next 202....\n"), NULL);
	else if ((v->abc & (1 << 1)) && v->count == 2)
		return (new_cmd(data->pipeline, '&')->cmd);
	else if (v->abc & (1 << 2))
		return (new_cmd(data->pipeline, '\0')->cmd);
	return (NULL);
}

///////////////////////////////////////////////////////////////////////////////]
// input: ">>>> ;alsdkfj;a"
// ab[0] = '<'
// ab[1] = '>'
void	ft_extract_redirection_v2(char *raw_line, int *i, t_cmd *cmd)
{
	int	ab[2];

	ft_memset(ab, 0, sizeof(int) * 2);
	while (raw_line[*i] == '<' && ++(*i))
		ab[0]++;
	while (raw_line[*i] == '>' && ++(*i))
		ab[1]++;
	if ((ab[0] && ab[1]) || ab[0] + ab[1] > 2)
		return (put(MSG_REDI, raw_line[*i - 1]), cmd->error++, (void)0);
	while (wii(raw_line[*i], " \t\n") >= 0)
		(*i)++;
	if (wii(raw_line[*i], "><|&;") >= 0)
		return (put(MSG_REDI, raw_line[*i]), cmd->error++, (void)0);
	if (ab[0])
	{
		cmd->in_bit = ab[0];
		cmd->in_file = free_s(cmd->in_file);
		cmd->in_file = ft_extract_words_v2(raw_line, i);
	}
	else if (ab[1])
	{
		cmd->out_bit = ab[1];
		cmd->out_file = free_s(cmd->out_file);
		cmd->out_file = ft_extract_words_v2(raw_line, i);
	}
}
