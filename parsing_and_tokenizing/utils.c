/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:26:58 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:47:04 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	m_clear_qoute(t_data *data, t_token **list)
{
	t_token	*pnt;
	char	type;

	pnt = *list;
	while (pnt)
	{
		if (pnt->type == 'q' || pnt->type == 't')
		{
			type = pnt->type;
			if (pnt->type == 'q')
				m_clear_qoute_loop(data, pnt);
			pnt->type = type;
		}
		pnt = pnt->next;
	}
	return (SUCCESS);
}

int	m_clear_qoute_loop(t_data *data, t_token *pnt)
{
	int		i;
	int		j;
	char	*temp;
	char	chr;

	i = -1;
	while (pnt->str[++i])
	{
		if (pnt->str[i] == '\'' || pnt->str[i] == '\"')
		{
			chr = pnt->str[i];
			temp = ft_substr(pnt->str, 0, i++);
			j = 0;
			while (pnt->str[i + j] && pnt->str[i + j] != chr)
				j++;
			temp = m_str_concat(m_str_concat(temp, ft_substr(pnt->str, i, j)),
					ft_substr(pnt->str, i + j + 1, ft_strlen(pnt->str)));
			if (!temp)
				return (m_malloc_exit(data));
			free(pnt->str);
			pnt->str = temp;
			i = i + j - 2;
		}
	}
	return (SUCCESS);
}

int	m_check_built_in(t_data *data, t_cmd *pnt)
{
	char	**built_in;
	int		i;

	if (pnt->arg[0] == NULL)
		return (0);
	built_in = (char *[]){"echo", "cd", "pwd",
		"export", "unset", "env", "exit", NULL};
	i = 0;
	while (built_in[i])
	{
		if (m_strcmp(built_in[i], pnt->arg[0]) == 0)
		{
			pnt->cmd_path = ft_strdup(pnt->arg[0]);
			if (!pnt->cmd_path)
				return (m_malloc_exit(data));
			return (1);
		}
		i++;
	}
	return (0);
}

int	m_check_slesh(t_data *data, t_cmd **pnt)
{
	int			res;
	struct stat	info;

	res = access((*pnt)->arg[0], F_OK);
	if (res == 0)
	{
		if (lstat((*pnt)->arg[0], &info) != 0)
			return (data->exit_status = 1);
		else
		{
			if (S_ISDIR(info.st_mode))
			{
				(*pnt)->cmd = ft_strjoin((*pnt)->arg[0], ": Is a directory\n");
				if (!(*pnt)->cmd)
					return (m_malloc_exit(data));
				return (data->exit_status = 126, 1);
			}
		}
		(*pnt)->cmd_path = ft_strdup((*pnt)->arg[0]);
		if (!(*pnt)->cmd_path)
			m_malloc_exit(data);
		return (1);
	}
	return (res);
}

int	m_check_new_path(t_data *data, char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t' || str[i] == ':'))
		i++;
	if (str[i] == '\0')
	{
		data->path = malloc(sizeof(char *) * 2);
		if (!data->path)
			m_malloc_exit(data);
		data->path[0] = ft_strdup(str);
		if (!data->path[0])
			m_malloc_exit(data);
		data->path[1] = NULL;
		return (1);
	}
	return (0);
}
