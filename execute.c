/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:45 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:44:44 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	m_fork(t_data *data, int *fd, t_cmd *pnt, t_cmd *prev)
{
	if (fd[O_READ] != STDIN_FILENO)
		dup2(fd[O_READ], STDIN_FILENO);
	if (fd[O_WRITE] != STDOUT_FILENO)
		dup2(fd[O_WRITE], STDOUT_FILENO);
	if (pnt->next)
	{
		close(pnt->pip_fd[O_READ]);
		close(pnt->pip_fd[O_WRITE]);
	}
	if (prev)
		(close(prev->pip_fd[O_READ]), close(prev->pip_fd[O_WRITE]));
	if (data->value)
	{
		execve(pnt->cmd_path, pnt->arg, data->env_copy);
		perror("execve");
		m_exit(data);
	}
	else
	{
		data->exit_status = m_is_built_in(data, pnt);
		m_exit(data);
	}
}

int	m_execute_in_child(t_data *data, t_cmd *pnt, t_cmd *prev, bool value)
{
	int	fd[2];

	fd[O_READ] = STDIN_FILENO;
	fd[O_WRITE] = STDOUT_FILENO;
	if (pnt->fd_inp != -1)
		fd[O_READ] = pnt->fd_inp;
	else if (prev)
		fd[O_READ] = prev->pip_fd[O_READ];
	if (pnt->fd_out != -1)
		fd[O_WRITE] = pnt->fd_out;
	else if (pnt->next)
		fd[O_WRITE] = pnt->pip_fd[O_WRITE];
	data->last_pid = fork();
	if (data->last_pid == -1)
		return (perror("fork"), FORK_FAILURE);
	if (data->last_pid == 0)
	{
		(signal(SIGINT, signal_child), signal(SIGQUIT, signal_child));
		data->value = value;
		m_fork(data, fd, pnt, prev);
	}
	if (prev)
		(close(prev->pip_fd[O_READ]), close(prev->pip_fd[O_WRITE]));
	(signal(SIGINT, signal_child), signal(SIGQUIT, signal_child));
	return (SUCCESS);
}

void	check_error_message(t_data *data)
{
	char	*message;
	t_cmd	*pnt;

	pnt = data->cmd;
	while (pnt->next)
		pnt = pnt->next;
	if (pnt->cmd)
	{
		message = ft_strjoin(pnt->arg[0], ": command not found\n");
		if (m_strcmp(pnt->cmd, message) == 0)
			data->exit_status = 127;
		else if (data->exit_status != 127 && data->exit_status != 126)
			data->exit_status = 1;
		free(message);
	}
}

void	m_arg_check(t_data *data, t_cmd *pnt)
{
	int	i;

	i = 0;
	while (pnt->arg[0][i] && !ft_isalnum(pnt->arg[0][i]))
		i++;
	if (pnt->arg[0][i] == '\0' && ft_strchr(pnt->arg[0], '/') == NULL)
	{
		pnt->cmd = ft_strjoin(pnt->arg[0], ": command not found\n");
		if (!pnt->cmd)
			m_malloc_exit(data);
		data->exit_status = 127;
		write(2, pnt->cmd, ft_strlen(pnt->cmd));
	}
}

int	m_execute_pipeline(t_data *data, t_cmd *pnt, t_cmd *prev)
{
	if (pnt->arg && pnt->arg[0])
	{
		if (!pnt->cmd)
			m_arg_check(data, pnt);
		if (!pnt->cmd && pnt->type)
		{
			m_put_path(data, &pnt);
			if (pnt->cmd)
				write(STDERR_FILENO, pnt->cmd, ft_strlen(pnt->cmd));
		}
		if (pnt->next)
			if (pipe(pnt->pip_fd) == -1)
				return (perror("pipe"), PIPE_FAILURE);
		if (!pnt->cmd)
		{
			if (!pnt->cmd && (pnt->next || prev || pnt->type))
				m_execute_in_child(data, pnt, prev, pnt->type);
			else if (!pnt->cmd && !pnt->type)
				data->exit_status = m_is_built_in(data, pnt);
		}
	}
	return (data->status);
}
