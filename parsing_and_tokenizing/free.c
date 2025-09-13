/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alschnei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:39:56 by alschnei          #+#    #+#             */
/*   Updated: 2025/06/17 16:46:02 by alschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	m_free_data(t_data *data)
{
	if (data->cmd)
	{
		m_free_cmd_list(&data->cmd);
		data->cmd = NULL;
	}
	if (data->input)
		free(data->input);
	if (data->value)
	{
		if (data->env_list)
			m_free_env_list(&data->env_list);
		if (data->env_copy)
			m_free(data->env_copy);
	}
}

int	m_exit(t_data *data)
{
	data->value = true;
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	if (data->path)
		m_free(data->path);
	if (data->prompt)
		free(data->prompt);
	m_free_data(data);
	rl_clear_history();
	data->status = 0;
	exit(data->exit_status);
	return (SUCCESS);
}

void	m_free(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i])
			free(str[i]);
		i++;
	}
	free(str);
}

void	m_free_cmd_list_1(t_cmd **cmd_list)
{
	if ((*cmd_list)->fd_out != -1)
		close((*cmd_list)->fd_out);
	if ((*cmd_list)->fd_inp != -1)
		close((*cmd_list)->fd_inp);
	if ((*cmd_list)->pip_fd[O_READ] != -1)
		close((*cmd_list)->pip_fd[O_READ]);
	if ((*cmd_list)->pip_fd[O_WRITE] != -1)
		close((*cmd_list)->pip_fd[O_WRITE]);
}

void	m_free_cmd_list(t_cmd **cmd_list)
{
	t_cmd	*next;

	while ((*cmd_list))
	{
		next = (*cmd_list)->next;
		m_free_cmd_list_1(cmd_list);
		if ((*cmd_list)->h_name)
			(unlink((*cmd_list)->h_name), free((*cmd_list)->h_name));
		if ((*cmd_list)->cmd)
			free((*cmd_list)->cmd);
		if ((*cmd_list)->c_list)
			m_free_token_list(&(*cmd_list)->c_list);
		if ((*cmd_list)->arg)
			m_free((*cmd_list)->arg);
		if ((*cmd_list)->cmd_path)
			free((*cmd_list)->cmd_path);
		free((*cmd_list));
		(*cmd_list) = next;
	}
}
