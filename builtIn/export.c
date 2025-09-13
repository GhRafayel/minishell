/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:45 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:43:52 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	m_export_sort(t_data *data, char **env)
{
	int		i;
	int		j;
	char	*tmp;

	(void)data;
	if (env == NULL)
		return ;
	i = -1;
	while (env[++i])
	{
		j = 0;
		while (env[j + 1])
		{
			if (m_strcmp(env[j], env[j + 1]) > 0)
			{
				tmp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = tmp;
			}
			j++;
		}
	}
}

void	m_print_export_1(char *temp)
{
	int		j;

	if (ft_strchr(temp, '=') == NULL)
		(write(1, "declare -x", 10),
			write(1, temp, ft_strlen(temp)), write(1, "\n", 1));
	else
	{
		j = 0;
		write(1, "declare -x ", 11);
		while (temp[j] && temp[j] != '=')
			write(1, &temp[j++], 1);
		(write(1, "=\"", 2), write(1, temp + j + 1,
				ft_strlen(temp + j + 1)), write(1, "\"\n", 2));
	}
}

int	m_print_export(t_data *data)
{
	char	**temp;
	int		count;
	int		i;

	count = 0;
	while (data->env_copy[count])
		count++;
	temp = ft_calloc(sizeof(char *), count + 1);
	if (!temp)
		m_malloc_exit(data);
	count = -1;
	i = -1;
	while (data->env_copy[++count])
	{
		temp[++i] = ft_strdup(data->env_copy[count]);
		if (!temp[i])
			m_malloc_exit(data);
	}
	m_export_sort(data, temp);
	i = -1;
	while (temp && temp[++i])
		if (temp[i][0] != '_')
			m_print_export_1(temp[i]);
	return (m_free(temp), SUCCESS);
}

int	m_export_change_env(t_data *data, char *arg, bool value)
{
	t_env_list	*pnt;
	char		**key_val;

	if (data->status != SUCCESS)
		return (m_malloc_exit(data));
	pnt = m_find_env_node(data, arg, value);
	key_val = ft_split(arg, '=');
	if (!key_val)
		return (m_malloc_exit(data));
	if (m_strcmp(key_val[0], "PATH") == 0)
		m_create_path(data, key_val[1]);
	if (key_val[1])
	{
		free(pnt->value);
		pnt->value = ft_strdup(key_val[1]);
	}
	else if (value)
	{
		free(pnt->value);
		pnt->value = ft_strdup("");
	}
	if (!pnt->value)
		return (m_malloc_exit(data));
	return (m_free(key_val), SUCCESS);
}

int	m_export(t_data *data, t_cmd *t_pnt)
{
	bool		value;
	int			i;

	i = 0;
	if (t_pnt->arg[1] == NULL)
		return (m_print_export(data));
	while (t_pnt->arg[++i])
	{
		value = true;
		if (ft_strchr(t_pnt->arg[i], '=') == NULL)
			value = false;
		if (m_check_key(t_pnt->arg[i]))
			m_print_exprot_error(data, t_pnt->arg[0], t_pnt->arg[i]);
		else
		{
			if (!m_find_env_node(data, t_pnt->arg[i], value))
				data->status = m_add_env_var(data, t_pnt->arg[i], value);
			else
				data->status = m_export_change_env(data, t_pnt->arg[i], value);
			if (data->status != SUCCESS)
				return (data->status);
		}
	}
	return (m_copy_env_from_list(data));
}
