/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:45 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:45:48 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	m_add_cmd_list_back(t_cmd **list, t_cmd *new_node)
{
	t_cmd	*pnt;

	if (!list || !new_node)
		return ;
	if (*list == NULL)
	{
		*list = new_node;
		return ;
	}
	pnt = *list;
	while (pnt->next)
		pnt = pnt->next;
	pnt->next = new_node;
}

t_cmd	*m_new_cmd(t_data *data, char *str)
{
	t_cmd	*new_node;

	str = m_trim(str);
	if (!str)
		m_malloc_exit(data);
	new_node = malloc(sizeof(t_cmd));
	if (new_node == NULL)
		(free(str), m_malloc_exit(data));
	new_node->next = NULL;
	new_node->c_list = NULL;
	new_node->arg = NULL;
	new_node->fd_out = -1;
	new_node->fd_inp = -1;
	new_node->type = true;
	new_node->cmd_path = NULL;
	new_node->pip_fd[O_READ] = -1;
	new_node->pip_fd[O_WRITE] = -1;
	new_node->h_name = NULL;
	new_node->cmd = ft_strdup(str);
	if (!new_node->cmd)
		(free(str), free(new_node), m_malloc_exit(data));
	m_add_cmd_list_back(&data->cmd, new_node);
	return (free(str), str = NULL, new_node);
}

int	m_create_cmd_1(t_data *data, t_cmd *t_pnt)
{
	t_token		*pnt;
	int			count;

	pnt = t_pnt->c_list;
	count = 0;
	while (pnt)
	{
		if (pnt->type == 'q' || pnt->type == 't')
		{
			if (pnt->str)
			{
				if (pnt->type == 'q' || count != 0)
					m_clear_qoute_loop(data, pnt);
				t_pnt->arg[count] = ft_strdup(pnt->str);
				if (!t_pnt->arg[count])
					return (m_malloc_exit(data));
				count++;
			}
		}
		pnt = pnt->next;
	}
	return (t_pnt->arg[count] = NULL, SUCCESS);
}

int	m_create_cmd(t_data *data, t_cmd **pnt)
{
	t_token		*p;
	t_cmd		*t_pnt;
	int			count;

	t_pnt = *pnt;
	count = 0;
	p = t_pnt->c_list;
	while (p)
	{
		if (p->type == 't' || p->type == 'f' || p->type == 'q')
		{
			if (p->str)
				count++;
		}
		p = p->next;
	}
	t_pnt->arg = (char **)malloc((count + 1) * sizeof(char *));
	if (!t_pnt->arg)
		return (m_malloc_exit(data));
	m_create_cmd_1(data, t_pnt);
	return (SUCCESS);
}
