/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_env_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alschnei <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:41:45 by alschnei          #+#    #+#             */
/*   Updated: 2025/06/17 16:43:34 by alschnei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"	

int	m_check_echo_args(char **arg, bool *value)
{
	int	i;
	int	j;

	i = 0;
	while (arg && arg[++i])
	{
		j = 0;
		if (ft_strchr(arg[i], '\'') || ft_strchr(arg[i], '\"'))
			return (i);
		while (arg[i][j] && (arg[i][j] == ' ' || arg[i][j] == '\t'))
			j++;
		if (arg[i][j] == '-' && arg[i][j + 1] == 'n')
		{
			j++;
			while (arg[i][j] && arg[i][j] == 'n')
				j++;
			if (arg[i][j] == '\0')
				*value = false;
			else
				return (i);
		}
		else
			return (i);
	}
	return (i);
}

int	m_echo(t_cmd **t_pnt)
{
	int		j;
	int		fd;
	bool	new_line;

	new_line = true;
	fd = STDOUT_FILENO;
	if ((*t_pnt)->fd_out != -1)
		fd = (*t_pnt)->fd_out;
	j = m_check_echo_args((*t_pnt)->arg, &new_line);
	while ((*t_pnt)->arg[j])
	{
		write(fd, (*t_pnt)->arg[j], ft_strlen((*t_pnt)->arg[j]));
		if ((*t_pnt)->arg[j + 1])
			write(fd, " ", 1);
		j++;
	}
	if (new_line)
		write(fd, "\n", 1);
	return (SUCCESS);
}

int	m_env(t_data *data, t_cmd *pnt)
{
	int	i;
	int	fd;

	i = 0;
	if (pnt->fd_out != -1)
		fd = pnt->fd_out;
	else
		fd = STDOUT_FILENO;
	while (data->env_copy && data->env_copy[i])
	{
		write(fd, data->env_copy[i], ft_strlen(data->env_copy[i]));
		write(fd, "\n", 1);
		i++;
	}
	return (SUCCESS);
}

int	m_pwd(t_cmd *t_pnt)
{
	int		fd;
	char	path[1024];

	if (t_pnt->fd_out != -1)
		fd = t_pnt->fd_out;
	else
		fd = STDOUT_FILENO;
	if (!getcwd(path, sizeof(path)))
		return (4);
	write(fd, path, ft_strlen(path));
	write(fd, "\n", 1);
	return (SUCCESS);
}
