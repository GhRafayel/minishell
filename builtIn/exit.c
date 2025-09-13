/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 13:04:31 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:43:43 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long long	m_exit_atoi(const char *str)
{
	long long	result;
	int			sign;
	int			digit;
	int			n;

	result = 0;
	sign = 1;
	n = 7;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (sign != 1)
			n = 8;
		if (result > LLONG_MAX / 10 || (result == LLONG_MAX / 10 && digit > n))
			return (255);
		result = result * 10 + digit;
		str++;
	}
	return (result * sign);
}

void	m_exit_print(t_data *data, char *arg, char *message, int n)
{
	t_env_list	*pnt;

	if (data->cmd->next == NULL)
		write(STDERR_FILENO, "exit\n", 5);
	if (arg)
	{
		write(2, "minishell: exit: ", 17);
		write(STDERR_FILENO, arg, ft_strlen(arg));
	}
	if (message)
		write(STDERR_FILENO, message, ft_strlen(message));
	data->exit_status = n;
	pnt = m_find_env_node(data, "SHLVL", false);
	if (pnt)
		pnt->value = m_shlvl(data, pnt->value, false);
	if (!data->cmd->next)
		m_exit(data);
}

bool	m_exit_arg_check(char *s)
{
	int		i;
	bool	val;

	i = 0;
	val = true;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
		{
			if (val && (s[i] == '+' || s[i] == '-'))
			{
				val = false;
				if (!ft_isdigit(s[i + 1]))
					return (false);
			}
			else
				return (false);
		}
		i++;
	}
	return (true);
}

void	m_builtin_exit_1(t_data *data, t_cmd *pnt)
{
	int				i;
	int				j;
	unsigned char	code;
	long long int	res;

	i = 0;
	while (pnt->cmd[i] && ft_isdigit(pnt->cmd[i]))
		i++;
	if (pnt->cmd[i] != '\0' && i != 0)
		return (m_exit_print(data, pnt->arg[1],
				": numeric argument required\n", 2));
	res = m_exit_atoi(pnt->cmd);
	code = res;
	i = 0;
	if (pnt->arg[1] == NULL)
		code = data->exit_status;
	while (pnt->cmd[i] == '0')
		i++;
	j = ft_strlen(pnt->cmd) - i;
	if (j == 0 && pnt->cmd[j] == '\0')
		m_exit_print(data, pnt->arg[1], ": numeric argument required\n", 2);
	else if (j >= 19 && res == 255)
		m_exit_print(data, pnt->arg[1], ": numeric argument required\n", 2);
	else
		m_exit_print(data, NULL, NULL, code);
}

int	m_builtin_exit(t_data *data, t_cmd *pnt)
{
	bool			val;

	if (pnt->arg[1])
	{
		pnt->cmd = ft_strtrim(pnt->arg[1], " \t");
		if (!pnt->cmd)
			return (m_malloc_exit(data));
		val = m_exit_arg_check(pnt->cmd);
		if (val == false)
			return (m_exit_print(data, pnt->arg[1],
					": numeric argument required\n", 2), 0);
		else if (val && pnt->arg[2])
			return (write(2, "exit: too many arguments\n", 25),
				data->exit_status = 1, 0);
		m_builtin_exit_1(data, pnt);
	}
	return (m_exit_print(data, NULL, NULL, data->exit_status), 0);
}
