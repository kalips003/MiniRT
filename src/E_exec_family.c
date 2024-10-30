/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_exec_family.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:25 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:25 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int			ft_minishell(t_data *data);
int			ft_father(t_data *data);
int			ft_big_brother(t_data *data, t_pipeline *pipeline);
static void	h_bigbro(t_data *data, t_cmd *ptr_cmd);
int			ft_lil_brother(t_data *data, t_cmd *cmd);

#define INPUT_TEXT "\001 \e[38;5;%dmm\e[38;5;%dmi\e[38;5;%dma"
#define INPUT_TEXT2 "\e[38;5;%dmo\e[38;5;%dmu\e[0m (\e[0;3%dm%d\e[0m): \002"
#define INPUT_TEXT_CLEAN "\001 mianishell(%d): \002"

// #define INPUT_TEXT "\001input:\002"
///////////////////////////////////////////////////////////////////////////////]
// input_txt = str(INPUT_TEXT_CLEAN, WEXITSTATUS(data->exit_code));
// 1 loop per input
int	ft_minishell(t_data *data)
{
	char	*input;
	char	*input_txt;

	ini_signal(data);
	input_txt = str(INPUT_TEXT INPUT_TEXT2 RESET,
			rand() % 256, rand() % 256, rand() % 256,
			rand() % 256, rand() % 256,
			1 + !(WEXITSTATUS(data->exit_code)), WEXITSTATUS(data->exit_code));
	input = readline(input_txt);
	free_s(input_txt);
	if (!input)
		(put("exit\n"), end(data, 0));
	input = trim(input, " \n\t");
	if (!input)
		return (0);
	clear_cmd(data);
	if (ft_parsing_v2(data, input))
	{
		data->exit_code = 0x0200;
		return (free_s(input), 0);
	}
	data->history = expand_tab(data->history, input);
	add_history(input);
	return (ft_father(data));
}

///////////////////////////////////////////////////////////////////////////////]
/*******************************************************************************
	takes data->cmd with arguments,
		create the big_brother that will do 1 pipeline
	return only:
		0 on succcess and -1 to exit (for now)
******************************************************************************/
int	ft_father(t_data *data)
{
	t_pipeline	*pipeline;
	int			rtrn;

	pipeline = data->pipeline;
	if (!data->pipeline)
		return (perror(ERR3"empty args struct"), 1);
	while (pipeline)
	{
		sublim_pipe_v2(data, pipeline);
		rtrn = WEXITSTATUS(data->exit_code);
		if (!pipeline->and_or || (pipeline->and_or == '&' && !rtrn)
			|| (pipeline->and_or == '|' && rtrn))
			rtrn = ft_big_brother(data, pipeline);
		pipeline = pipeline->next;
	}
	return (clear_cmd(data), 0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		does 1 chained list
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int	ft_big_brother(t_data *data, t_pipeline *pipeline)
{
	pid_t	pid;

	ini_signal_v2(data);
	if (!pipeline->cmd->next && brother_builtin_v2(data, pipeline->cmd))
		return (0);
	pid = fork();
	if (pid == -1)
		return (perror(ERR7"fork"), 1);
	if (!pid)
		h_bigbro(data, pipeline->cmd);
	waitpid(pid, &data->exit_code, 0);
	if (WIFSIGNALED(data->exit_code))
		data->exit_code = WTERMSIG(data->exit_code) << 8;
	return (0);
}

static void	h_bigbro(t_data *data, t_cmd *ptr_cmd)
{
	t_cmd	*first_cmd;

	first_cmd = ptr_cmd;
	ini_signal_exec(data);
	while (ptr_cmd)
	{
		ft_lil_brother(data, ptr_cmd);
		ptr_cmd = ptr_cmd->next;
	}
	close(STDIN);
	ptr_cmd = first_cmd;
	while (ptr_cmd)
	{
		waitpid(ptr_cmd->pid, &data->exit_code, 0);
		ptr_cmd = ptr_cmd->next;
	}
	end(data, WEXITSTATUS(data->exit_code));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
		takes a cmd node, return the return value of child
		echo a | sleep 5 | echo b
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int	ft_lil_brother(t_data *data, t_cmd *cmd)
{
	int		fd[2];

	if (cmd->next)
	{
		if (pipe(fd) == -1)
			(perror(ERR7"pipe"), end(data, 66));
	}
	cmd->pid = fork();
	if (cmd->pid == -1)
		(perror(ERR7"fork"), close(fd[0]), close(fd[1]), end(data, 66));
	if (!cmd->pid)
	{
		if (cmd->next)
			(close(fd[0]), dup_close(fd[1], STDOUT));
		ft_exec_v2(data, cmd, data->env);
	}
	else if (cmd->next)
		(close(fd[1]), dup_close(fd[0], STDIN));
	return (0);
}
