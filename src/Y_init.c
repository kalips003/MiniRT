/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Y_init.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 06:21:51 by kalipso           #+#    #+#             */
/*   Updated: 2024/09/07 16:35:11 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int			initialization(int ac, char **av, char **env, t_data *data);
static void	copy_env(t_data *data, char **env);

///////////////////////////////////////////////////////////////////////////////]
// ini
int	initialization(int ac, char **av, char **env, t_data *data)
{
	srand(time(NULL));
	ft_memset(data, 0, sizeof(t_data));
	copy_env(data, env);
	data->fd_in_original = dup(STDIN_FILENO);
	if (ac == 3 && same_str(av[1], "-c"))
	{
		if (ft_parsing_v2(data, av[2]))
			data->exit_code = 0x0100;
		ft_father(data);
		end(data, (WEXITSTATUS(data->exit_code)));
	}
	else if (ac != 1)
		return (put(ERR"Wrong number of arguments\n"), exit(0), 1);
	rl_event_hook = (void *)fakesig;
	sigemptyset(&data->sig_int.sa_mask);
	sigemptyset(&data->sig_quit.sa_mask);
	return (0);
}

static void	copy_env(t_data *data, char **env)
{
	int		i;

	if (!env)
		return ;
	i = -1;
	while (env[++i])
		data->env = expand_tab(data->env, str("%1s", env[i]));
}
