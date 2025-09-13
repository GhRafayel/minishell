/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:45 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:44:48 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	m_create_heredoc_file(t_data *data, t_cmd **t_pnt, bool val)
{
	if (val == false)
		return (data->status = SUCCESS, SUCCESS);
	(*t_pnt)->h_name = ft_strdup(".heredoc");
	if (!(*t_pnt)->h_name)
		return (m_malloc_exit(data));
	while (access((*t_pnt)->h_name, F_OK) == 0)
	{
		(*t_pnt)->h_name = m_str_concat((*t_pnt)->h_name,
				ft_strdup((*t_pnt)->h_name));
		if (!(*t_pnt)->h_name)
			return (m_malloc_exit(data));
	}
	(*t_pnt)->fd_inp = open((*t_pnt)->h_name, O_CREAT | O_RDWR | O_TRUNC, 0600);
	if ((*t_pnt)->fd_inp == -1)
		return (data->exit_status = 1, FILE_ERROR);
	return (data->status = SUCCESS, SUCCESS);
}

int	m_input_1(t_data *data, t_cmd **pnt, t_token *p, bool val)
{
	struct stat	buf;
	mode_t		permissions;

	if (stat(p->str, &buf) == 0)
	{
		permissions = buf.st_mode;
		if (S_ISDIR(buf.st_mode))
			return (m_o_a_error(data, pnt, p->str, 0), 1);
		if (!(permissions & S_IRUSR))
			return (m_o_a_error(data, pnt, p->str, 3), 1);
		if (val)
		{
			(*pnt)->fd_inp = open(p->str, O_RDONLY);
			if ((*pnt)->fd_inp == -1)
				return (data->status = UNKNOWN_ERROR, UNKNOWN_ERROR);
		}
	}
	else
		return (m_o_a_error(data, pnt, p->str, 2), 1);
	return (data->status = SUCCESS, data->status);
}

int	*m_get_exit_status(int *exit_status)
{
	static int	*exit_status_pnt = NULL;

	if (exit_status)
		exit_status_pnt = exit_status;
	return (exit_status_pnt);
}
