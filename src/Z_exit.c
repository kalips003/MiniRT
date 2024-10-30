/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Z_exit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2024/09/07 16:26:15 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void			clear_cmd(t_data *data);
static t_cmd	*h_clean_cmd(t_cmd *cmd);
void			end(t_data *data, int exit_code);

///////////////////////////////////////////////////////////////////////////////]
// free double chain data->cmd
void	clear_cmd(t_data *data)
{
	t_pipeline	*ptr1;
	t_cmd		*ptr2;
	void		*ptr_temp;

	if (!data->pipeline)
		return ;
	ptr1 = data->pipeline;
	while (ptr1)
	{
		ptr2 = ptr1->cmd;
		while (ptr2)
			ptr2 = h_clean_cmd(ptr2);
		ptr_temp = ptr1->next;
		ptr1 = free_s(ptr1);
		ptr1 = (t_pipeline *)ptr_temp;
	}
	data->pipeline = NULL;
}

static t_cmd	*h_clean_cmd(t_cmd *cmd)
{
	t_cmd	*ptr_temp;

	free_tab(cmd->cmd_arg);
	free_s(cmd->in_file);
	free_s(cmd->out_file);
	if (cmd->fd_in > 0)
		close(cmd->fd_in);
	if (cmd->fd_out > 0)
		close(cmd->fd_out);
	ptr_temp = cmd->next;
	free_s(cmd);
	return (ptr_temp);
}

///////////////////////////////////////////////////////////////////////////////]
void	end(t_data *data, int exit_code)
{
	free_tab(data->history);
	free_tab(data->env);
	clear_cmd(data);
	rl_clear_history();
	if (data->fd_in_original >= 0)
		close(data->fd_in_original);
	exit(exit_code);
}
