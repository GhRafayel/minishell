/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:02:37 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:46:07 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*m_shlvl(t_data *data, char *str, bool val)
{
	int	res;

	res = ft_atoi(str);
	if (val)
		res++;
	else if (res > 1)
		res--;
	else
		return (str);
	free(str);
	str = ft_itoa(res);
	if (!str)
		m_malloc_exit(data);
	return (str);
}

int	m_empty_env(t_data *data)
{
	t_env_list	*pnt;
	char		**env;
	int			i;

	i = 0;
	env = ft_calloc(sizeof(char *), 4);
	if (!env)
		m_malloc_exit(data);
	env[0] = ft_strdup("_=/bin/env");
	env[1] = ft_strdup("SHLVL=1");
	env[2] = ft_strjoin("PWD=", getcwd(NULL, 1024));
	if (!env[0] || !env[1] || !env[2])
		m_malloc_exit(data);
	while (i < 3)
	{
		pnt = m_new_env_node(env[i]);
		if (!pnt)
			m_malloc_exit(data);
		m_add_env_back(&data->env_list, pnt);
		i++;
	}
	m_copy_env_from_list(data);
	return (m_free(env), SUCCESS);
}

int	m_copy_env(t_data *data, char **env)
{
	t_env_list	*pnt;
	int			i;

	i = 0;
	while (env[i])
	{
		pnt = m_new_env_node(env[i]);
		if (!pnt)
			m_malloc_exit(data);
		if (env[i][0] == '_')
		{
			free(pnt->value);
			pnt->value = ft_strdup("/bin/env");
			if (!pnt->value)
				m_malloc_exit(data);
		}
		else if (m_strcmp(pnt->key, "PATH") == 0)
			m_create_path(data, pnt->value);
		else if (m_strcmp(pnt->key, "SHLVL") == 0)
			pnt->value = m_shlvl(data, pnt->value, true);
		m_add_env_back(&data->env_list, pnt);
		i++;
	}
	m_copy_env_from_list(data);
	return (SUCCESS);
}

int	m_create_path(t_data *data, char *str)
{
	int	j;

	if (data->path)
		m_free(data->path);
	data->path = NULL;
	if (str && str[0])
	{
		if (m_check_new_path(data, str))
			return (SUCCESS);
		data->path = ft_split(str, ':');
		if (!data->path)
			return (m_malloc_exit(data));
		j = -1;
		while (data->path[++j])
		{
			data->path[j] = m_str_concat(data->path[j], ft_strdup("/"));
			if (!data->path[j])
				return (m_malloc_exit(data));
		}
	}
	return (SUCCESS);
}

int	m_init_data(t_data *data, char **env)
{
	data->cmd = NULL;
	data->status = 0;
	if (data->value)
	{
		data->env_list = NULL;
		data->env_copy = NULL;
		data->path = NULL;
		data->value = false;
	}
	if (data->env_list == NULL)
	{
		if (!env || !env[0])
			m_empty_env(data);
		else
			m_copy_env(data, env);
	}
	return (data->status);
}
