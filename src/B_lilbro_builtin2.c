/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   B_lilbro_builtin2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agallon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:20:11 by umosse            #+#    #+#             */
/*   Updated: 2024/09/07 20:16:17 by agallon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int		cmd_miaou(t_data *data, t_cmd *cmd);
int		cmd_pwd(t_data *data, t_cmd *cmd);

#define MSG_ERR_97 "pwd: error retrieving current directory: getcwd:cannot"
#define MSG_ERR_98 "access parent directories: No such file or directory\n"

int	cmd_pwd(t_data *data, t_cmd *cmd)
{
	char	*temp;

	(void)cmd;
	temp = rtrn_var_v2(data->env, "PWD=");
	if (!temp)
		return (end(data, 0), 0);
	if (temp && access(temp, X_OK) == -1)
	{
		perror(ERR2 "access:");
		put(ERR2 MSG_ERR_97 MSG_ERR_98);
		end(data, 1);
	}
	put("%s\n", rtrn_var_v2(data->env, "PWD="));
	end(data, 0);
	return (0);
}

///////////////////////////////////////////////////////////////////////////////]
int	cmd_miaou(t_data *data, t_cmd *cmd)
{
	(void)cmd;
	ft_print_cat(data->exit_code, "tis memory", 0b10);
	put("% 1000S\n", data->history[0]);
	end(data, 0);
	return (0);
}
