/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_inp_out.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:45 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:43:25 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	m_check_input_name(t_data *data, t_token *p, int n)
{
	char	*temp;
	int		i;

	p->str = m_trim(m_cut_until_end(p->str, n));
	if (!p->str)
		return (m_malloc_exit(data));
	if (!ft_strchr(p->str, '\'') && !ft_strchr(p->str, '\"'))
		return (SUCCESS);
	m_clear_qoute_loop(data, p);
	i = 0;
	while (p->str[i] && (p->str[i] == '.' || p->str[i] == '/'))
		i++;
	temp = ft_substr(p->str, i, ft_strlen(p->str));
	if (!temp)
		m_malloc_exit(data);
	if (ft_strchr(temp, '/') != NULL)
		return (free(temp), SUCCESS);
	free(p->str);
	p->str = ft_strdup(temp);
	if (!p->str)
		m_malloc_exit(data);
	free(temp);
	return (SUCCESS);
}

int	m_output_and_append_1(t_data *data, t_cmd **t_pnt, t_token *pnt, int n)
{
	struct stat	buf;
	mode_t		permissions;

	m_check_input_name(data, pnt, n);
	if ((*t_pnt)->fd_out != -1)
		close((*t_pnt)->fd_out);
	if (stat(pnt->str, &buf) == 0)
	{
		permissions = buf.st_mode;
		if (S_ISDIR(buf.st_mode))
			return (m_o_a_error(data, t_pnt, pnt->str, 0), 1);
		if (!(permissions & S_IRUSR))
			return (m_o_a_error(data, t_pnt, pnt->str, 1), 1);
	}
	else if (ft_strchr(pnt->str, '/'))
		return (m_o_a_error(data, t_pnt, pnt->str, 2), 1);
	if (n == 1)
		(*t_pnt)->fd_out = open(pnt->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		(*t_pnt)->fd_out = open(pnt->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if ((*t_pnt)->fd_out == -1)
		return (UNKNOWN_ERROR);
	return (SUCCESS);
}

int	m_check_ambiguous(t_data *data, t_cmd **pnt, t_token *p, char *temp)
{
	int	i;

	i = 0;
	while (p->str && (p->str[i] == '<' || p->str[i] == '>'
			|| p->str[i] == ' ' || p->str[i] == '\t'))
		i++;
	if (!p->str || (p->str[i] == '\0' && (p->str[i] != '\"'
				|| p->str[i] != '\'')))
	{
		(*pnt)->cmd = ft_strjoin(temp, ": ambiguous redirect\n");
		if (!(*pnt)->cmd)
			return (free(temp), m_malloc_exit(data));
		data->exit_status = 1;
		return (1);
	}
	return (SUCCESS);
}

int	m_append_and_out(t_data *data, t_cmd **t_pnt, t_token *p)
{
	int			n;
	char		*temp;

	n = 1;
	if (p->type == 'A')
		n = 2;
	p->type = 't';
	temp = m_trim(ft_substr(p->str, n, ft_strlen(p->str)));
	if (!temp)
		return (m_malloc_exit(data));
	p->str = m_find_variable(data, p->str);
	if (temp && temp[0] != '\"' && temp[0] != '\''
		&& m_check_ambiguous(data, t_pnt, p, temp))
		return (free(temp), p->type = '\0', data->status);
	data->status = m_output_and_append_1(data, t_pnt, p, n);
	if (temp)
		free(temp);
	if (data->status == UNKNOWN_ERROR)
		return (data->status);
	if (data->status != SUCCESS)
		return (data->exit_status = 1, data->status = 0, 0);
	return (data->status);
}

int	m_input(t_data *data, t_cmd **pnt, t_token *p, bool val)
{
	p->str = m_trim(m_cut_until_end(p->str, 1));
	if (!p->str)
		m_malloc_exit(data);
	p->type = 't';
	p->str = m_find_variable(data, p->str);
	m_clear_qoute_loop(data, p);
	p->type = '\0';
	data->exit_status = m_input_1(data, pnt, p, val);
	if (data->status == UNKNOWN_ERROR)
		return (data->status);
	else if (data->status != SUCCESS)
		return (data->status);
	return (data->status = SUCCESS, data->status);
}
