/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alschnei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:45 by alschnei          #+#    #+#             */
/*   Updated: 2025/06/17 17:04:48 by alschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 1;

int	m_do_redirects(t_data *data, t_cmd **pnt, bool val)
{
	t_token	*p;

	p = (*pnt)->c_list;
	while (p)
	{
		if (p->type == 'H' && m_heredoc(data, p, pnt, val))
			return (1);
		else if (p->type == 'I' && !(*pnt)->cmd)
		{
			m_input(data, pnt, p, val);
			if (data->exit_status != SUCCESS)
			{
				m_heredoc_error(data, (*pnt));
				write(2, (*pnt)->cmd, ft_strlen((*pnt)->cmd));
			}
		}
		else if ((p->type == 'O' || p->type == 'A') && !(*pnt)->cmd)
		{
			m_append_and_out(data, pnt, p);
			if (data->exit_status != SUCCESS)
				write(2, (*pnt)->cmd, ft_strlen((*pnt)->cmd));
		}
		p = p->next;
	}
	return (g_signal = 1, SUCCESS);
}

int	m_execute(t_data *data)
{
	t_cmd	*pnt;
	t_cmd	*prev;
	int		status;

	prev = NULL;
	pnt = data->cmd;
	while (pnt)
	{
		m_execute_pipeline(data, pnt, prev);
		prev = pnt;
		pnt = pnt->next;
	}
	while ((wait(&status)) > 0)
	{
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status) && status == 2)
			data->exit_status = 128 + WTERMSIG(status);
		else if (status == 131)
			data->exit_status = status;
	}
	return (handle_signals_main(), SUCCESS);
}

int	pars_and_tokenizing(t_data *data)
{
	t_cmd	*pnt;

	if (m_count_quote(data->input))
		return (data->exit_status = 128);
	m_split_pips(data);
	if (m_check_commands(data))
		return (SUCCESS);
	if (m_find_quote(data))
		return (SUCCESS);
	pnt = data->cmd;
	while (pnt)
	{
		if (m_create_cmd(data, &pnt))
			return (SUCCESS);
		if (m_check_built_in(data, pnt))
			pnt->type = false;
		if (m_do_redirects(data, &pnt, pnt->type))
			return (SUCCESS);
		pnt = pnt->next;
	}
	m_execute(data);
	check_error_message(data);
	return (data->status = SUCCESS, 0);
}

char	*m_get_prompt(void)
{
	char	*cwd;

	cwd = m_str_concat(m_str_concat(ft_strdup("minishell:~"),
				getcwd(NULL, 1024)), ft_strdup("$ "));
	if (!cwd)
		return (NULL);
	return (cwd);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;

	((void)argc, (void)argv);
	data.value = true;
	data.status = -1;
	data.exit_status = 0;
	data.prompt = m_get_prompt();
	if (!data.prompt)
		(write(2, "Malloc allocation failed\n", 25), exit(MALLOC));
	m_get_exit_status(&data.exit_status);
	handle_signals_main();
	while (1)
	{
		data.input = readline(data.prompt);
		if (data.input && data.input[0])
		{
			(add_history(data.input), m_init_data(&data, env));
			(pars_and_tokenizing(&data), m_free_data(&data));
		}
		if (data.input == NULL && data.status == -1)
			(free(data.prompt), write(2, "exit\n", 5), exit(data.exit_status));
		else if (data.input == NULL)
			(write(2, "exit\n", 5), m_exit(&data));
	}
	return (data.exit_status);
}
