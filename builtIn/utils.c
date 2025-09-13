/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:39:56 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:44:25 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	m_add_env_var(t_data *data, char *arg, bool value)
{
	t_env_list	*pnt;

	if (data->status != SUCCESS)
		return (m_malloc_exit(data));
	pnt = m_new_env_node(arg);
	if (m_strcmp(pnt->key, "PATH") == 0)
		m_create_path(data, pnt->value);
	if (value == false)
	{
		free(pnt->value);
		pnt->value = NULL;
	}
	m_add_env_back(&data->env_list, pnt);
	return (SUCCESS);
}

int	m_is_built_in(t_data *data, t_cmd *t_pnt)
{
	if (m_strcmp(t_pnt->arg[0], "echo") == 0)
		data->status = m_echo(&t_pnt);
	else if (m_strcmp(t_pnt->arg[0], "export") == 0)
		return (m_export(data, t_pnt), data->exit_status);
	else if (m_strcmp(t_pnt->arg[0], "unset") == 0)
		data->status = m_unset(data, t_pnt);
	else if (m_strcmp(t_pnt->arg[0], "pwd") == 0)
		data->status = m_pwd(t_pnt);
	else if (m_strcmp(t_pnt->arg[0], "env") == 0)
		data->status = m_env(data, t_pnt);
	else if (m_strcmp(t_pnt->arg[0], "exit") == 0)
		return (m_builtin_exit(data, t_pnt), data->exit_status);
	else if (m_strcmp(t_pnt->arg[0], "cd") == 0)
		return (m_cd(data, t_pnt));
	return (data->status);
}
