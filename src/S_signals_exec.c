/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   S_signals_exec.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: umosse <umosse@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:57:24 by umosse            #+#    #+#             */
/*   Updated: 2024/09/07 17:08:04 by umosse           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ini_signal_exec(t_data *data);
void	h_sigquit_exec(int sig);
void	h_sigint_exec(int sig);

///////////////////////////////////////////////////////////////////////////////]
void	ini_signal_exec(t_data *data)
{
	data->sig_quit.sa_handler = (void *)h_sigquit_exec;
	sigaction(SIGQUIT, &data->sig_quit, NULL);
	data->sig_int.sa_handler = (void *)h_sigint_exec;
	sigaction(SIGINT, &data->sig_int, NULL);
}

// SIGQUIT = ^\.
void	h_sigquit_exec(int sig)
{
	(void)sig;
	rl_done = 1;
	write(2, "Quit (core dumped)\n", 19);
	exit(131);
}

// SIGINT = ^C
void	h_sigint_exec(int sig)
{
	(void)sig;
	rl_done = 1;
	write(2, "\n", 1);
	exit(130);
}
