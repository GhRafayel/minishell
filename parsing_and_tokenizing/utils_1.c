/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:26:58 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:47:16 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	m_put_path_1(t_data *data, t_cmd **pnt)
{
	int			i;
	int			res;

	i = 0;
	while (data->path && (*pnt)->arg[0] && data->path[i])
	{
		(*pnt)->cmd_path = ft_strjoin(data->path[i], (*pnt)->arg[0]);
		if (!(*pnt)->cmd_path)
			return (m_malloc_exit(data));
		res = access((*pnt)->cmd_path, F_OK);
		if (res == 0)
		{
			free((*pnt)->cmd_path);
			(*pnt)->cmd_path = ft_strjoin(data->path[i], (*pnt)->arg[0]);
			if (!(*pnt)->cmd_path)
				return (m_malloc_exit(data));
			return (1);
		}
		free((*pnt)->cmd_path);
		i++;
	}
	return (SUCCESS);
}

int	m_put_path(t_data *data, t_cmd **pnt)
{
	if ((*pnt)->arg[0] && ft_strchr((*pnt)->arg[0], '/') != NULL)
	{
		data->status = m_check_slesh(data, pnt);
		if (data->status != SUCCESS)
		{
			if (data->status == -1)
				return (m_o_a_error(data, pnt, (*pnt)->arg[0], 2),
					data->status = 0, 1);
			return (1);
		}
	}
	if (m_put_path_1(data, pnt))
		return (SUCCESS);
	if (data->path == NULL && access((*pnt)->arg[0], F_OK) == 0)
	{
		(*pnt)->cmd_path = ft_strdup((*pnt)->arg[0]);
		if (!(*pnt)->cmd_path)
			return (m_malloc_exit(data));
		return (SUCCESS);
	}
	(*pnt)->cmd_path = NULL;
	m_o_a_error(data, pnt, (*pnt)->arg[0], 3);
	return (data->exit_status = 127, X_OK);
}

int	m_check_next_token(t_token *pnt, bool val)
{
	t_token	*p;

	p = pnt->next;
	if (val == false)
		return (false);
	while (p)
	{
		if (p->type == 'H' || p->type == 'I')
			return (false);
		p = p->next;
	}
	return (true);
}

char	*m_replace_variable_1(t_data *data, char *value)
{
	char	*var_name;

	if (ft_strchr(value, '\''))
		var_name = m_str_concat(ft_strjoin("\"", value), ft_strdup("\""));
	else if (ft_strchr(value, '\"'))
		var_name = m_str_concat(ft_strjoin("\'", value), ft_strdup("\'"));
	else
		var_name = ft_strdup(value);
	if (!var_name)
		return (m_malloc_exit(data), NULL);
	return (var_name);
}

char	*m_replace_variable(t_data *data, char *var_name)
{
	t_env_list	*pnt;
	char		*num;

	if (var_name == NULL)
		return (m_malloc_exit(data), NULL);
	pnt = data->env_list;
	if (m_strcmp(var_name, "$?") == 0)
	{
		free(var_name);
		num = ft_itoa(data->exit_status);
		if (!num)
			return (m_malloc_exit(data), NULL);
		return (num);
	}
	while (pnt)
	{
		if (m_strcmp(pnt->key, var_name) == 0)
		{
			free(var_name);
			var_name = m_replace_variable_1(data, pnt->value);
			return (var_name);
		}
		pnt = pnt->next;
	}
	return (free(var_name), NULL);
}
