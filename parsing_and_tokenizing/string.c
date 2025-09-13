/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:43:06 by rghazary          #+#    #+#             */
/*   Updated: 2025/06/17 16:46:41 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*m_trim(char *input)
{
	char	*temp;

	if (!input)
		return (NULL);
	if (input[0] == '\0')
		return (input);
	temp = ft_strtrim(input, " 	");
	free(input);
	return (input = NULL, temp);
}

char	*m_cut_until_end(char *str, int start)
{
	char	*temp;

	if (str == NULL)
		return (NULL);
	temp = ft_substr(str, start, ft_strlen(str) - start);
	if (str)
	{
		free(str);
		str = NULL;
	}
	if (!temp)
		return (NULL);
	return (temp);
}

char	*m_str_concat(char *str1, char *str2)
{
	char	*temp;
	int		i;
	int		j;

	i = -1;
	j = 0;
	if (!str1 || !str2)
	{
		if (str2)
			return (free(str2), str2 = NULL, NULL);
		else if (str1)
			return (free(str1), str1 = NULL, NULL);
		else
			return (NULL);
	}
	temp = malloc(ft_strlen(str1) + ft_strlen(str2) + 1);
	if (!temp)
		return (free(str1), free(str2), str1 = NULL, str2 = NULL, NULL);
	while (str1[++i])
		temp[i] = str1[i];
	while (str2[j])
		temp[i++] = str2[j++];
	temp[i] = '\0';
	(free(str1), free(str2));
	return (str1 = NULL, str2 = NULL, temp);
}

t_cmd	*m_split_pips(t_data *data)
{
	char	c;
	int		i;

	i = -1;
	while (data->input[++i])
	{
		if (data->input[i] == '\"' || data->input[i] == '\'')
		{
			c = data->input[i++];
			while (data->input[i] && data->input[i] != c)
				i++;
		}
		else if (data->input[i] == '|')
		{
			m_new_cmd(data, ft_substr(data->input, 0, i));
			data->input = m_cut_until_end(data->input, i + 1);
			if (!data->input || data->cmd == NULL)
				m_malloc_exit(data);
			i = -1;
		}
		else if (data->input[i] == '$' && (data->input[i + 1] == '\"'
				|| data->input[i + 1] == '\''))
			data->input[i] = ' ';
	}
	return (m_new_cmd(data, ft_substr(data->input, 0, i)));
}

int	m_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (s1 != s2);
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
