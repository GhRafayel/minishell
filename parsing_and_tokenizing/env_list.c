/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:39:56 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:45:53 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env_list	*m_new_env_node(char *str)
{
	int			i;
	t_env_list	*new_node;

	i = 0;
	new_node = (t_env_list *)malloc(sizeof(t_env_list));
	if (new_node == NULL)
		return (NULL);
	while (str[i] && str[i] != '=')
		i++;
	new_node->key = ft_substr(str, 0, i);
	if (!new_node->key)
		return (free(new_node), NULL);
	new_node->value = ft_substr(str, i + 1, ft_strlen(str));
	if (!new_node->value)
		return (free(new_node->key), free(new_node), NULL);
	new_node->next = NULL;
	return (new_node);
}

void	m_add_env_back(t_env_list **lst, t_env_list *new)
{
	t_env_list	*pnt;

	if (!new)
		return ;
	pnt = *lst;
	if (pnt == NULL)
	{
		*lst = new;
		return ;
	}
	while (pnt->next)
		pnt = pnt->next;
	pnt->next = new;
	new->next = NULL;
}

void	m_free_env_list(t_env_list **env_list)
{
	t_env_list	*pnt;
	t_env_list	*next;

	pnt = *env_list;
	while (pnt)
	{
		next = pnt->next;
		free(pnt->key);
		free(pnt->value);
		free(pnt);
		pnt = next;
	}
}

t_env_list	*m_find_env_node(t_data *data, char *key, bool val)
{
	t_env_list	*p;
	char		*temp;
	int			i;

	p = data->env_list;
	i = 0;
	if (val)
	{
		while (key[i] && key[i] != '=')
			i++;
		temp = ft_substr(key, 0, i);
		if (!temp)
			return (m_malloc_exit(data), NULL);
	}
	else
		temp = ft_strdup(key);
	if (!temp)
		return (m_malloc_exit(data), NULL);
	while (p)
	{
		if (m_strcmp(p->key, temp) == 0)
			return (free(temp), p);
		p = p->next;
	}
	return (free(temp), NULL);
}

int	m_env_list_length(t_env_list *list)
{
	int			count;
	t_env_list	*p;

	count = 0;
	p = list;
	while (p)
	{
		count++;
		p = p->next;
	}
	return (count);
}
