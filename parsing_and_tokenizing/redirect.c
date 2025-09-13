/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alschnei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:02:51 by alschnei          #+#    #+#             */
/*   Updated: 2025/06/17 16:46:27 by alschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	m_stop_redirect(t_cmd *p, int *s, bool *quote)
{
	char	c;

	if (p->cmd[*s] == '<' || p->cmd[*s] == '>'
		|| p->cmd[*s] == '&' || p->cmd[*s] == ';'
		|| p->cmd[*s] == '(' || p->cmd[*s] == ')'
		|| p->cmd[*s] == '\0')
		return (*quote = false, SYNTAX_ERROR);
	while (p->cmd[*s])
	{
		if (p->cmd[*s] == '\'' || p->cmd[*s] == '\"')
		{
			c = p->cmd[*s];
			*s += 1;
			while (p->cmd[*s] && p->cmd[*s] != c)
				*s += 1;
		}
		*s += 1;
		if (p->cmd[*s] && (p->cmd[*s] == ' ' || p->cmd[*s] == '\t'
				|| p->cmd[*s] == '<' || p->cmd[*s] == '>'
				|| p->cmd[*s] == '\n'))
			break ;
	}
	return (SUCCESS);
}

int	m_count_quote(char *input)
{
	bool	val;
	int		i;
	int		count;
	char	c;

	i = 0;
	val = true;
	count = 0;
	while (input[i])
	{
		if (val && (input[i] == '\'' || input[i] == '\"'))
		{
			count++;
			val = false;
			c = input[i];
		}
		else if (!val && input[i] == c)
		{
			val = true;
			count++;
		}
		i++;
	}
	return (count % 2);
}

int	m_cut_redirect(t_cmd *t_pnt, int *start, bool *quote, char c)
{
	if (*start != 0 && t_pnt->cmd[*start - 1] != ' '
		&& t_pnt->cmd[*start - 1] != '\t')
	{
		if (m_new_token_node(&t_pnt->c_list,
				ft_substr(t_pnt->cmd, 0, *start), 't', true) == NULL)
			return (MALLOC);
		t_pnt->cmd = m_cut_until_end(t_pnt->cmd, *start);
		if (!t_pnt->cmd)
			return (MALLOC);
	}
	*start = 0;
	if (c == 'H' || c == 'A')
		*start += 2;
	else if (c == 'I' || c == 'O')
		*start += 1;
	while (t_pnt->cmd[*start]
		&& (t_pnt->cmd[*start] == ' ' || t_pnt->cmd[*start] == '\t'))
		*start += 1;
	if (m_stop_redirect(t_pnt, start, quote) != SUCCESS)
		return (SYNTAX_ERROR);
	return (m_found_space(t_pnt, start, quote, c));
}

void	m_found_redirect_1(t_cmd *t_pnt, int *start)
{
	if (!t_pnt->cmd[*start] && t_pnt->next)
		write(STDERR_FILENO, "syntax error near unexpected token `|'\n", 39);
	else if (t_pnt->cmd[*start])
	{
		write(STDERR_FILENO, "syntax error near unexpected token `", 36);
		write(STDERR_FILENO, &t_pnt->cmd[*start], 1);
		if (t_pnt->cmd[*start + 1] != ' ' && t_pnt->cmd[*start + 1] != '\t')
			write(STDERR_FILENO, &t_pnt->cmd[*start + 1], 1);
		write(2, "'\n", 2);
	}
	else if (!t_pnt->cmd[*start] && !t_pnt->next)
		write(STDERR_FILENO,
			"syntax error near unexpected token `newline'\n", 45);
}

int	m_found_redirect(t_data *data, t_cmd *t_pnt, int *start, bool *quote)
{
	if (t_pnt->cmd[*start] == '<' && t_pnt->cmd[*start + 1] == '<')
		data->status = m_cut_redirect(t_pnt, start, quote, 'H');
	else if (t_pnt->cmd[*start] == '>' && t_pnt->cmd[*start + 1] == '>')
		data->status = m_cut_redirect(t_pnt, start, quote, 'A');
	else if (t_pnt->cmd[*start] == '>')
		data->status = m_cut_redirect(t_pnt, start, quote, 'O');
	else if (t_pnt->cmd[*start] == '<')
		data->status = m_cut_redirect(t_pnt, start, quote, 'I');
	if (data->status != SUCCESS)
	{
		if (data->status == MALLOC)
			return (m_malloc_exit(data));
		m_heredoc_error(data, t_pnt);
		m_found_redirect_1(t_pnt, start);
		return (data->exit_status = 2, SYNTAX_ERROR);
	}
	return (SUCCESS);
}
