/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   S_signals.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 01:29:49 by kalipso           #+#    #+#             */
/*   Updated: 2024/09/07 17:08:14 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ini_signal(t_data *data);
void	ini_signal_v2(t_data *data);
void	h_sigint(int sig);
void	fakesig(void);

///////////////////////////////////////////////////////////////////////////////]
// CTRL C = SIGINT
// CTRL \ = SIGQUIT
// si '\' dans main, ignor == SIG_IGN
// si '\' dans un process, kill process + print quit code dump
///////////////////////////////////////////////////////////////////////////////]
// 	Function hande signals
// initialiize tout les loop, pour nuttured actions in readline
void	ini_signal(t_data *data)
{
	data->sig_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &data->sig_quit, NULL);
	data->sig_int.sa_handler = (void *)h_sigint;
	sigaction(SIGINT, &data->sig_int, NULL);
}

// whiile executing
void	ini_signal_v2(t_data *data)
{
	data->sig_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &data->sig_quit, NULL);
	data->sig_int.sa_handler = (void *)SIG_IGN;
	sigaction(SIGINT, &data->sig_int, NULL);
}

// SIGINT = ^C
// PARSING
void	h_sigint(int sig)
{
	(void)sig;
	rl_done = 1;
	rl_replace_line("", 0);
}

void	fakesig(void)
{
}
