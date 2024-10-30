/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   E_ft_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:57 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:57 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void		ft_exec_v2(t_data *data, t_cmd *cmd, char **env);
static void	h_734_redirection(t_data *data, t_cmd *cmd);
static void	ft_heredoc(t_data *data, t_cmd *cmd);
static char	*find_cmd(char *command, char **env);
static char	*find_parsing(char *command, char **env);

///////////////////////////////////////////////////////////////////////////////]
/*******************************************************************************
	takes a cmd link, redirect to correct fd, execute it, end the child
******************************************************************************/
void	ft_exec_v2(t_data *data, t_cmd *cmd, char **env)
{
	char	*cmd_exe;

	if (!cmd)
		end(data, 0);
	h_734_redirection(data, cmd);
	sublim_wildcard(cmd);
	if (child_builtin_v2(data, cmd))
		end(data, data->exit_code);
	cmd_exe = find_cmd(cmd->cmd_arg[0], data->env);
	if (!cmd_exe)
	{
		print_fd(2, ERR6"%s: not found (%s)\n",
			cmd->cmd_arg[0], strerror(errno));
		end(data, 127);
	}
	if (execve(cmd_exe, cmd->cmd_arg, env) == -1)
	{
		print_fd(2, ERR6"error execve (%s)\n", strerror(errno));
		free_s(cmd_exe);
		end(data, 1);
	}
}

///////////////////////////////////////////////////////////////////////////////]
static void	h_734_redirection(t_data *data, t_cmd *cmd)
{
	if (cmd->in_file && cmd->out_file && same_str(cmd->in_file, cmd->out_file))
		return (print_fd(2, ERR"same input\\output file '%s'\n",
				cmd->in_file), end(data, 3));
	if (cmd->in_file && cmd->in_bit == 1)
	{
		cmd->fd_in = open(cmd->in_file, O_RDONLY);
		if (cmd->fd_in < 0)
			return (perror(cmd->in_file), end(data, 1));
		dup_close(cmd->fd_in, STDIN_FILENO);
	}
	else if (cmd->in_file && cmd->in_bit == 2)
		ft_heredoc(data, cmd);
	if (cmd->out_file && cmd->out_bit)
		h_redirec_out(data, cmd);
	if (data->fd_in_original >= 0)
		close(data->fd_in_original);
	data->fd_in_original = -1;
}

///////////////////////////////////////////////////////////////////////////////]
// create child that will pipe the input
static void	ft_heredoc(t_data *data, t_cmd *cmd)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		return (perror(ERR7"pipe"), end(data, 4));
	pid = fork();
	if (pid == -1)
		return (perror(ERR7"fork"), close(fd[0]), close(fd[1]), end(data, 4));
	if (!pid)
		ft_heredoc2(data, cmd, fd);
	else
	{
		close(data->fd_in_original);
		close(fd[1]);
		dup_close(fd[0], STDIN);
		waitpid(pid, NULL, 0);
	}
}

///////////////////////////////////////////////////////////////////////////////]
/*
 mianishell(0): export ABC="hello"
 mianishell(0): <<$ABC | cat
heredoc:this is how you say hi $ABC
*/
// return full path of command > /path/to/command
// 		./command/path > ./command/path
static char	*find_cmd(char *command, char **env)
{
	char	*rtrn;

	rtrn = NULL;
	if (!command)
	{
		return (NULL);
	}
	if (find_str(command, "/") >= 0)
	{
		if (access(command, X_OK) == 0)
			return (str("%1s", command));
		return (NULL);
	}
	rtrn = find_parsing(command, env);
	return (rtrn);
}

static char	*find_parsing(char *command, char **env)
{
	char	**paths;
	char	*cmd;
	int		i;

	paths = split(rtrn_var_v2(env, "PATH="), ":");
	if (!paths)
		return (put(RED"ERROR PATH NOT FOUND\n"RESET), NULL);
	cmd = NULL;
	i = -1;
	while (paths[++i])
	{
		cmd = str("%1s/%1s", paths[i], command);
		if (!access(cmd, X_OK))
			break ;
		cmd = free_s(cmd);
	}
	free_tab(paths);
	return (cmd);
}
