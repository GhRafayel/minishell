/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 11:01:55 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:45:34 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	m_print_exprot_error(t_data *data, char *s1, char *s2)
{
	write(2, s1, ft_strlen(s1));
	write(2, ": `", 3);
	write(2, s2, ft_strlen(s2));
	write(2, "': not a valid identifier\n", 26);
	data->exit_status = 1;
}

int	m_malloc_exit(t_data *data)
{
	data->exit_status = 1;
	write(STDERR_FILENO, "Malloc allocation failed\n", 25);
	return (m_exit(data), 0);
}

void	m_o_a_error(t_data *data, t_cmd **pnt, char *error, int index)
{
	char	*message[5];
	int		i;

	message[0] = ": Is a directory\n";
	message[1] = ": Permission denied\n";
	message[2] = ": No such file or directory\n";
	message[3] = ": command not found\n";
	message[4] = "warning: here-document delimited by end-of-file";
	if (ft_strchr(error, '\'') || ft_strchr(error, '\"'))
	{
		i = 0;
		while (error[i] && (error[i] == ' ' || error[i] == '\t'))
			i++;
		while (error[i] && error[i] != ' ' && error[i] != '\t')
			i++;
		(*pnt)->cmd = m_str_concat(ft_substr(error, 0, i),
				ft_strdup(message[index]));
	}
	else
		(*pnt)->cmd = ft_strjoin(error, message[index]);
	if (!(*pnt)->cmd)
		m_malloc_exit(data);
	if (index == 2)
		data->exit_status = 127;
}

void	m_heredoc_error(t_data *data, t_cmd *pnt)
{
	t_token	*p;

	p = pnt->c_list;
	while (p)
	{
		if (p->type == 'H')
		{
			m_heredoc_1(data, p, &pnt, false);
		}
		p = p->next;
	}
	data->exit_status = 2;
}

int	m_check_commands(t_data *data)
{
	t_cmd	*pnt;
	int		i;

	i = 0;
	pnt = data->cmd;
	if (!pnt->next)
	{
		while (pnt->cmd[i] && (pnt->cmd[i] == ' ' || pnt->cmd[i] == '\t'))
			i++;
		if (pnt->cmd[i] == '\0')
			return (1);
	}
	while (pnt)
	{
		if (!pnt->cmd || pnt->cmd[0] == '\0')
		{
			write(2, "syntax error near unexpected token `|'\n", 39);
			return (data->exit_status = 2, SYNTAX_ERROR);
		}
		pnt = pnt->next;
	}
	return (SUCCESS);
}
