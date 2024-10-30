/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_ft_exec2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:54:29 by umosse            #+#    #+#             */
/*   Updated: 2024/09/07 18:05:30 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_heredoc2(t_data *data, t_cmd *cmd, int *fd);
void	h_redirec_out(t_data *data, t_cmd *cmd);

void	h_redirec_out(t_data *data, t_cmd *cmd)
{
	if (cmd->out_bit == 1)
		cmd->fd_out = open(cmd->out_file,
				(O_WRONLY | O_CREAT | O_TRUNC), 0777);
	else if (cmd->out_bit == 2)
		cmd->fd_out = open(cmd->out_file,
				(O_WRONLY | O_CREAT | O_APPEND), 0777);
	if (cmd->fd_out < 0)
		return (perror(cmd->out_file), end(data, 1));
	dup_close(cmd->fd_out, STDOUT_FILENO);
}

void	ft_heredoc2(t_data *data, t_cmd *cmd, int *fd)
{
	char	*tmp;

	close(fd[0]);
	dup_close(data->fd_in_original, STDIN);
	data->fd_in_original = -1;
	while (1)
	{
		tmp = readline(C_415"heredoc:"RESET);
		if (!tmp || same_str(tmp, cmd->in_file))
			(close(fd[1]), free_s(tmp), end(data, 0));
		print_fd(fd[1], "%s\n", tmp);
		tmp = free_s(tmp);
	}
}
