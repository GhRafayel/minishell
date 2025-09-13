/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:45 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:44:38 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	m_cd_print(char *s, int index)
{
	char		*mess[4];

	mess[0] = ": No such file or directory\n";
	mess[1] = ": stat failed\n";
	mess[2] = ": Not a directory\n";
	mess[3] = ": Permission denied\n";
	write(2, "minishell: cd: ", 15);
	write(2, s, ft_strlen(s));
	write(2, mess[index], ft_strlen(mess[index]));
}

int	m_add_oldpwd(t_data *data, char *oldpwd)
{
	char		*cwd;

	cwd = NULL;
	oldpwd = m_str_concat(ft_strdup("OLDPWD="), oldpwd);
	if (!oldpwd)
		return (m_malloc_exit(data));
	if (m_find_env_node(data, oldpwd, true))
		m_export_change_env(data, oldpwd, true);
	else
		m_add_env_var(data, oldpwd, true);
	cwd = m_str_concat(ft_strdup("PWD="), getcwd(cwd, 1024));
	if (!cwd)
		(free(oldpwd), m_malloc_exit(data));
	if (m_find_env_node(data, cwd, true))
		m_export_change_env(data, cwd, true);
	else
		m_add_env_var(data, cwd, true);
	free(data->prompt);
	data->prompt = m_get_prompt();
	if (!data->prompt)
		m_malloc_exit(data);
	m_copy_env_from_list(data);
	return (free(oldpwd), free(cwd), SUCCESS);
}

int	m_change_directory(t_data *data, char *path, char *oldpwd)
{
	struct stat	statbuf;

	if (oldpwd == NULL)
		m_malloc_exit(data);
	if (access(path, F_OK) != 0)
		return (free(oldpwd), m_cd_print(path, 0), free(path), 1);
	if (stat(path, &statbuf) != 0)
		return (free(oldpwd), m_cd_print(path, 1), free(path), 1);
	if (!S_ISDIR(statbuf.st_mode))
		return (free(oldpwd), m_cd_print(path, 2), free(path), 1);
	if (access(path, X_OK) != 0)
		return (free(oldpwd), m_cd_print(path, 3), free(path), 1);
	if (chdir(path) != 0)
		return (free(oldpwd), m_cd_print(path, 2), free(path), 1);
	free(path);
	m_add_oldpwd(data, oldpwd);
	return (0);
}

char	*m_get_env(t_data *data, char *s)
{
	char		*path;
	t_env_list	*p;

	p = NULL;
	path = NULL;
	p = m_find_env_node(data, s, false);
	if (p == NULL)
		return (NULL);
	if (p->value)
	{
		path = ft_strdup(p->value);
		if (!path)
			m_malloc_exit(data);
	}
	return (path);
}

int	m_cd(t_data *data, t_cmd *pnt)
{
	char		*path;
	char		cwd[1024];

	if (pnt->arg[1] && pnt->arg[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), 1);
	if (!getcwd(cwd, 1024))
		return (perror("getcwd"), data->exit_status = 1, m_exit(data));
	if (!pnt->arg[1] || pnt->arg[1][0] == '\0'
			|| m_strcmp(pnt->arg[1], "~") == 0)
	{
		path = m_get_env(data, "HOME");
		return (m_change_directory(data, path, ft_strdup(cwd)));
	}
	else if (m_strcmp(pnt->arg[1], "-") == 0)
	{
		path = m_get_env(data, "OLDPWD");
		if (!path)
			return (write(2, "minishell: cd: OLDPWD not set\n", 30), 1);
		(write(1, path, ft_strlen(path)), write(1, "\n", 1));
		return (m_change_directory(data, path, ft_strdup(cwd)));
	}
	else if (!pnt->arg[1])
		return (write(2, "minishell: cd: HOME not set\n", 28), 1);
	return (m_change_directory(data, ft_strdup(pnt->arg[1]), ft_strdup(cwd)));
}
