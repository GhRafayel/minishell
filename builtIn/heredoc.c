/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:45 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:44:03 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	m_heredoc_4(t_data *data, t_cmd **t_pnt, t_token *pnt, bool val)
{
	if (data->value && val)
		data->status = m_heredoc_variable(data, pnt);
	pnt->str = m_str_concat(pnt->str, ft_strdup("\n"));
	if (!pnt->str)
		m_malloc_exit(data);
	if (val)
		write((*t_pnt)->fd_inp, pnt->str, ft_strlen(pnt->str));
	free(pnt->str);
}

int	m_heredoc_variable(t_data *data, t_token *pnt)
{
	int	i;

	i = 0;
	while (pnt->str && pnt->str[i])
	{
		if (pnt->str[i] == '$')
		{
			pnt->str = m_find_variable_2(data, pnt->str, &i);
			if (!pnt->str)
				pnt->str = ft_strdup("");
			if (!pnt->str)
				m_malloc_exit(data);
		}
		i++;
	}
	if (pnt->str == NULL)
	{
		pnt->str = ft_strdup("");
		if (!pnt->str)
			return (m_malloc_exit(data));
	}
	return (SUCCESS);
}

char	*m_heredoc_2(t_data *data, t_token *pnt, bool value)
{
	char	*delimiter;
	int		i;

	i = 2;
	data->value = true;
	while (pnt->str[i] && (pnt->str[i] == ' ' || pnt->str[i] == '\t'))
		i++;
	if (ft_strchr(pnt->str, '\'') != NULL || ft_strchr(pnt->str, '\"') != NULL)
	{
		value = false;
		data->value = false;
	}
	pnt->type = 't';
	m_clear_qoute_loop(data, pnt);
	delimiter = ft_strdup(pnt->str + i);
	if (value)
		delimiter = m_trim(delimiter);
	if (!delimiter)
		return (m_malloc_exit(data), NULL);
	free(pnt->str);
	return (pnt->type = '\0', delimiter);
}

int	m_heredoc_1(t_data *data, t_token *pnt, t_cmd **t_pnt, bool val)
{
	char		*delim;

	delim = m_heredoc_2(data, pnt, val);
	val = m_check_next_token(pnt, val);
	m_create_heredoc_file(data, t_pnt, val);
	while (data->status == SUCCESS && delim)
	{
		handle_signals_heredoc();
		pnt->str = readline(">");
		if (g_signal == 3 || !pnt->str || m_strcmp(pnt->str, delim) == 0)
			break ;
		m_heredoc_4(data, t_pnt, pnt, val);
	}
	if (pnt->str == NULL)
	{
		write(2, "warning: here-document at line ", 31);
		write(2, " delimited by end-of-file (wanted `", 35);
		write(2, delim, ft_strlen(delim));
		write(2, "')\n", 3);
	}
	return (data->value = false, free(delim), data->status);
}

int	m_heredoc(t_data *data, t_token *p, t_cmd **pnt, bool val)
{
	m_heredoc_1(data, p, pnt, val);
	if (g_signal == 3)
		return (g_signal = 1, data->exit_status = 130, 1);
	if ((*pnt)->h_name)
	{
		close((*pnt)->fd_inp);
		(*pnt)->fd_inp = open((*pnt)->h_name, O_RDONLY);
		if ((*pnt)->fd_inp == -1)
			return (g_signal = 1, FILE_ERROR);
	}
	return (SUCCESS);
}
