/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alschnei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:34:12 by alschnei          #+#    #+#             */
/*   Updated: 2025/06/17 16:46:35 by alschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_handler(int sig)
{
	int	*exit_status;

	exit_status = m_get_exit_status(NULL);
	if (sig == SIGINT)
	{
		if (g_signal == 1)
		{
			write(1, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
			if (exit_status)
				*exit_status = 130;
		}
		else if (g_signal == 2)
		{
			g_signal = 3;
			if (exit_status)
				*exit_status = 130;
		}
	}
}

int	heredoc_event_hook(void)
{
	if (g_signal == 3)
	{
		rl_done = 1;
	}
	return (0);
}

void	handle_signals_main(void)
{
	g_signal = 1;
	rl_event_hook = NULL;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_signals_heredoc(void)
{
	g_signal = 2;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	rl_event_hook = heredoc_event_hook;
}

void	signal_child(int sig)
{
	if (sig == SIGINT)
	{
		write(STDERR_FILENO, "\n", 1);
	}
	else if (sig == SIGQUIT)
	{
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
	}
}
