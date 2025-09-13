/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:42:11 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:44:16 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	m_copy_env_from_list(t_data *data)
{
	t_env_list	*pnt;
	int			count;

	if (data->env_copy)
		m_free(data->env_copy);
	count = m_env_list_length(data->env_list);
	data->env_copy = malloc(sizeof(char *) * (count + 1));
	if (!data->env_copy)
		return (m_malloc_exit(data));
	pnt = data->env_list;
	data->env_copy[count] = NULL;
	count = 0;
	while (pnt)
	{
		if (pnt->value)
			data->env_copy[count] = m_str_concat(ft_strjoin(pnt->key, "="),
					ft_strdup(pnt->value));
		else
			data->env_copy[count] = ft_strdup(pnt->key);
		if (!data->env_copy[count])
			return (m_malloc_exit(data));
		pnt = pnt->next;
		count++;
	}
	return (SUCCESS);
}

int	m_check_key(char *key)
{
	int	i;

	i = -1;
	if (key[0] != '_' && ft_isalpha(key[0]) == 0)
		return (1);
	while (key[++i])
	{
		if (key[i] == '=')
			return (SUCCESS);
		if (ft_isalnum(key[i]) == 0)
			if (key[i] != '_')
				return (1);
	}
	return (SUCCESS);
}

void	m_delete_env_node(t_env_list **list, t_env_list *node)
{
	t_env_list	*temp;
	t_env_list	*prev;

	if (!list || !*list || !node)
		return ;
	temp = *list;
	prev = NULL;
	while (temp)
	{
		if (m_strcmp(temp->key, node->key) == 0)
		{
			if (prev)
				prev->next = temp->next;
			else
				*list = temp->next;
			free(temp->key);
			free(temp->value);
			free(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}

int	m_unset(t_data *data, t_cmd *t_pne)
{
	t_env_list	*node;
	int			i;

	i = 1;
	if (t_pne->arg[1] == NULL)
		return (SUCCESS);
	while (t_pne->arg[i])
	{
		node = m_find_env_node(data, t_pne->arg[i], false);
		if (node)
		{
			if (m_strcmp(node->key, "PATH") == 0)
			{
				m_free(data->path);
				data->path = NULL;
			}
			m_delete_env_node(&data->env_list, node);
			m_copy_env_from_list(data);
		}
		if (data->status != SUCCESS)
			return (data->status);
		i++;
	}
	return (SUCCESS);
}
