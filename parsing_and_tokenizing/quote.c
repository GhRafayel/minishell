/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 11:14:43 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:46:18 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	m_found_space(t_cmd *t_pnt, int *start, bool *quote, char c)
{
	int		end;

	end = 0;
	if (*start > 0)
	{
		if (m_new_token_node(&t_pnt->c_list,
				ft_substr(t_pnt->cmd, 0, *start), c, true) == NULL)
			return (MALLOC);
	}
	while (t_pnt->cmd[*start + end] && (t_pnt->cmd[*start + end] == ' '
			|| t_pnt->cmd[*start + end] == '\t'))
		end++;
	t_pnt->cmd = m_cut_until_end(t_pnt->cmd, *start + end);
	if (t_pnt->cmd == NULL)
		return (MALLOC);
	*start = -1;
	*quote = false;
	return (SUCCESS);
}

int	m_found_quote_loop(char *s, int *start)
{
	char	symbol;
	int		i;

	i = *start;
	while (s[i])
	{
		if (s[i] == '\"' || s[i] == '\'')
		{
			symbol = s[i];
			i++;
			while (s[i] && s[i] != symbol)
				i++;
			i++;
		}
		if (!s[i] || s[i] == ' ' || s[i] == '\t' || s[i] == '<' || s[i] == '>')
			return (i);
		else
		{
			while (s[i] && s[i] != ' ' && s[i] != '\t' && s[i] != '<'
				&& s[i] != '>' && s[i] != '\"' && s[i] != '\'')
				i++;
		}
	}
	return (i);
}

int	m_found_quote(t_cmd *t_pnt, int *start, bool *quote)
{
	int		i;

	i = m_found_quote_loop(t_pnt->cmd, start);
	if (m_new_token_node(&t_pnt->c_list,
			ft_substr(t_pnt->cmd, 0, i), 'q', true) == NULL)
		return (MALLOC);
	t_pnt->cmd = m_cut_until_end(t_pnt->cmd, i);
	if (t_pnt->cmd == NULL)
		return (MALLOC);
	return (*start = -1, *quote = false, SUCCESS);
}

int	m_find_quote_2(t_data *data, t_cmd *t_pnt, bool quote)
{
	if (t_pnt->cmd == NULL)
		return (data->status);
	if (quote == true)
	{
		if (t_pnt->cmd[0] == '\0')
			return (free(t_pnt->cmd), t_pnt->cmd = NULL, 0);
		if (m_new_token_node(&t_pnt->c_list, t_pnt->cmd, 't', true) == NULL)
			return (m_malloc_exit(data));
	}
	else if (t_pnt->cmd[0] && !quote)
	{
		if (m_new_token_node(&t_pnt->c_list, t_pnt->cmd, 't', true) == NULL)
			return (m_malloc_exit(data));
	}
	else if (!t_pnt->cmd[0] && !quote)
		free(t_pnt->cmd);
	t_pnt->cmd = NULL;
	return (data->status);
}

int	m_find_quote(t_data *data)
{
	t_cmd			*t_pnt;
	int				start;
	bool			quote;

	t_pnt = data->cmd;
	while (t_pnt)
	{
		start = -1;
		quote = true;
		if (expansion(data, t_pnt))
			return (data->status);
		while (t_pnt->cmd && t_pnt->cmd[++start])
		{
			if (t_pnt->cmd[start] == '\'' || t_pnt->cmd[start] == '\"')
				data->status = m_found_quote(t_pnt, &start, &quote);
			else if (t_pnt->cmd[start] == ' ' || t_pnt->cmd[start] == '\t')
				data->status = m_found_space(t_pnt, &start, &quote, 't');
			if (!quote && data->status != SUCCESS)
				return (data->status);
		}
		data->status = m_find_quote_2(data, t_pnt, quote);
		t_pnt = t_pnt->next;
	}
	return (data->value = false, data->status);
}
