/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_builtin_func.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kalipso <kalipso@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 04:12:38 by kalipso           #+#    #+#             */
/*   Updated: 2024/07/16 04:12:38 by kalipso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int			cmd_cd(t_data *data, t_cmd *cmd);
int			cmd_export(t_data *data, t_cmd *cmd);
static int	h_export(t_data *data, char **arg);
int			cmd_unset(t_data *data, t_cmd *cmd);
int			cmd_exit(t_data *data, t_cmd *cmd);

#define BUF_WD 1024

///////////////////////////////////////////////////////////////////////////////]
// HOME=/home/kalipso
// SHELL=/usr/bin/zsh
int	cmd_cd(t_data *data, t_cmd *cmd)
{
	char	*new_pwd;
	char	*new_oldpwd;
	char	buf_newpath[BUF_WD];

	if (!cmd->cmd_arg[1])
		cmd->cmd_arg = expand_tab(cmd->cmd_arg,
				str("%1s", rtrn_var_v2(data->env, "HOME=")));
	if (chdir(cmd->cmd_arg[1]))
		return (perror(ERR9"cd bad directory"), 1);
	ft_memset(buf_newpath, 0, BUF_WD);
	if (!getcwd(buf_newpath, BUF_WD))
		return (perror(ERR8"erreur test"), 1);
	if (!*buf_newpath)
		return (perror(ERR9"getcwd"), 1);
	new_pwd = str("PWD=%1s", buf_newpath);
	new_oldpwd = str("OLDPWD=%1s", rtrn_var_v2(data->env, "PWD="));
	replace_var(data, "PWD=", new_pwd);
	replace_var(data, "OLDPWD=", new_oldpwd);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
// export var="text"
// export var+="text"
int	cmd_export(t_data *data, t_cmd *cmd)
{
	char	**arg;
	int		rtrn;

	rtrn = 0;
	if (!cmd->cmd_arg[1])
		return (put("%+-t", data->env, "export "), 0);
	arg = cmd->cmd_arg;
	while (*(++arg))
		rtrn |= h_export(data, arg);
	return (rtrn);
}

static int	h_export(t_data *data, char **arg)
{
	char	*tmp_string;
	int		len_until_equal;
	int		len_plus;
	int		len_error;

	len_until_equal = wii('=', *arg);
	len_plus = len_m(*arg, "+");
	len_error = len_m(*arg, ";|&()[]{}!@#^*-/\\~%?:,<>$\'\"");
	if (len_until_equal < 0)
		return (1);
	if (len_until_equal > len_error || len_plus < len_until_equal - 1)
		return (put(ERR5"export: `%s': not a valid identifier\n", *arg), 1);
	if (len_until_equal == len_plus + 1)
	{
		tmp_string = str("%1.*s=", len_plus, *arg);
		replace_var(data, tmp_string, str("%1s%1s", rtrn_var(data->env,
					tmp_string), &(*arg)[len_until_equal + 1]));
	}
	else
	{
		tmp_string = str("%1.*s", len_until_equal + 1, *arg);
		replace_var(data, tmp_string, str("%1s", *arg));
	}
	free_s(tmp_string);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	cmd_unset(t_data *data, t_cmd *cmd)
{
	char	*first_arg;
	char	*var;

	first_arg = cmd->cmd_arg[1];
	if (!first_arg)
		return (0);
	var = str("%1s=", first_arg);
	data->env = pop_entry(data->env, var);
	free_s(var);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	cmd_exit(t_data *data, t_cmd *cmd)
{
	int	err;
	int	rtrn_atoi;

	err = 0;
	if (tab_size(cmd->cmd_arg) > 2)
		return (put("bash: exit: too many arguments\n"), 1);
	if (cmd->cmd_arg && cmd->cmd_arg[1])
	{
		rtrn_atoi = ft_atoi(cmd->cmd_arg[1], &err);
		if (err)
		{
			put("bash: exit: %s: numeric argument required\n", cmd->cmd_arg[1]);
			end(data, WEXITSTATUS(0x0200));
		}
		else
			end(data, WEXITSTATUS(rtrn_atoi << 8) & 0xff);
	}
	end(data, WEXITSTATUS(data->exit_code));
	return (0);
}
