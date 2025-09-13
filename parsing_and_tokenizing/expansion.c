/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:40:10 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:45:58 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*m_find_variable_2(t_data *data, char *str, int *start)
{
	char	*temp;

	if (str[*start] == '$' && str[*start + 1] == '?')
	{
		temp = m_str_concat(ft_substr(str, 0, *start),
				m_replace_variable(data, ft_substr(str, *start, 2)));
		if (!temp)
			m_malloc_exit(data);
		str = m_str_concat(temp, m_cut_until_end(str, *start + 2));
		if (!str)
			m_malloc_exit(data);
		*start = -1;
	}
	else if (str[*start] == '$' && (str[*start + 1] == ' '
			|| str[*start + 1] == '\''
			|| str[*start + 1] == '/'
			|| str[*start + 1] == '\"'
			|| str[*start + 1] == '$'))
	{
		*start += 1;
	}
	else if (str[*start] == '$' && str[*start + 1])
		str = m_find_variable_1(data, str, start);
	return (str);
}

char	*m_find_variable_1(t_data *data, char *str, int *start)
{
	char	*temp;
	char	*temp1;
	int		end;

	end = 0;
	temp1 = ft_substr(str, 0, *start);
	*start += 1;
	while (ft_isalnum(str[*start + end]) || str[*start + end] == '_')
		end++;
	temp = m_replace_variable(data, ft_substr(str, *start, end));
	str = m_str_concat(ft_strdup(""),
			m_cut_until_end(str, *start + end));
	*start = -1;
	if (temp)
	{
		temp1 = m_str_concat(temp1, temp);
		str = m_str_concat(temp1, str);
		if (!str)
			return (m_malloc_exit(data), NULL);
	}
	else
		str = m_str_concat(temp1, str);
	if (str && !str[0])
		return (free(str), str = NULL, NULL);
	return (str);
}

char	*m_find_variable(t_data *data, char *str)
{
	int			start;
	bool		value;

	value = true;
	start = -1;
	if (!str)
		m_malloc_exit(data);
	while (str && str[++start])
	{
		if (str[start] == '\"')
			value = !value;
		if (value && str[start] == '\'')
			while (str[++start] && str[start] != '\'')
				start += 0;
		else if (str[start] != '\"' && str[start] == '$')
		{
			str = m_find_variable_2(data, str, &start);
			if (value == false)
				value = true;
		}
	}
	return (str);
}

int	expansion_1(t_data *data, t_cmd *pnt, int *start)
{
	char	*temp;
	bool	quote;

	quote = false;
	temp = ft_substr(pnt->cmd, 0, *start);
	if (!temp)
		m_malloc_exit(data);
	pnt->cmd = m_cut_until_end(pnt->cmd, *start);
	if (!pnt->cmd)
		return (free(temp), m_malloc_exit(data));
	*start = 0;
	data->status = m_found_redirect(data, pnt, start, &quote);
	if (!quote && data->status != SUCCESS)
		return (free(temp), data->status);
	pnt->cmd = m_str_concat(temp, pnt->cmd);
	if (!pnt->cmd)
	{
		free(temp);
		m_malloc_exit(data);
	}
	return (SUCCESS);
}

int	expansion(t_data *data, t_cmd *pnt)
{
	int		start;
	char	*str;
	char	c;

	start = -1;
	while (pnt->cmd && pnt->cmd[++start])
	{
		if (pnt->cmd[start] == '\"' || pnt->cmd[start] == '\'')
		{
			c = pnt->cmd[start];
			while (pnt->cmd && pnt->cmd[++start] != c)
				;
		}
		else if (pnt->cmd[start] == '<' || pnt->cmd[start] == '>')
			if (expansion_1(data, pnt, &start) != SUCCESS)
				return (1);
	}
	str = ft_strdup(pnt->cmd);
	free(pnt->cmd);
	pnt->cmd = NULL;
	pnt->cmd = m_find_variable(data, str);
	if (pnt->cmd && pnt->cmd[0] == '\0')
		return (free(pnt->cmd), pnt->cmd = NULL, SUCCESS);
	return (SUCCESS);
}
