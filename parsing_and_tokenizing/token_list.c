/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:46:30 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:46:49 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*m_new_token_node(t_token **list, char *str, char c, bool val)
{
	t_token	*new_node;

	str = m_trim(str);
	if (!str)
		return (NULL);
	new_node = malloc(sizeof(t_token));
	if (new_node == NULL)
		return (free(str), NULL);
	new_node->str = ft_strdup(str);
	if (!new_node->str)
		return (free(new_node), free(str), NULL);
	new_node->type = c;
	new_node->next = NULL;
	if (val)
		m_add_token_list_back(list, new_node);
	return (free(str), new_node);
}

void	m_free_token_list(t_token **c_list)
{
	t_token	*pnt;
	t_token	*next;

	pnt = *c_list;
	while (pnt)
	{
		next = pnt->next;
		free(pnt->str);
		free(pnt);
		pnt = next;
	}
}

void	m_add_token_list_back(t_token **list, t_token *new_node)
{
	t_token	*pnt;

	if (!list || !new_node)
		return ;
	if (!new_node->str[0])
	{
		free(new_node);
		return ;
	}
	if (*list == NULL)
	{
		*list = new_node;
		return ;
	}
	pnt = *list;
	while (pnt->next != NULL)
		pnt = pnt->next;
	pnt->next = new_node;
}

void	m_delete_token_node(t_token **list, t_token *node)
{
	t_token	*temp;
	t_token	*prev;

	if (!list || !*list || !node)
		return ;
	temp = *list;
	prev = NULL;
	while (temp)
	{
		if (temp == node || temp->type == '\0')
		{
			if (prev)
				prev->next = temp->next;
			else
				*list = temp->next;
			free(temp->str);
			free(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}
