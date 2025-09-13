/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:08:14 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/09 15:08:16 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(const char *s, unsigned start, size_t end)
{
	char			*new_str;
	unsigned int	i;
	size_t			s_len;

	if (s == NULL)
		return (NULL);
	i = 0;
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (end > s_len - start)
		end = s_len - start;
	new_str = (char *)malloc((end + 1) * sizeof(char));
	if (new_str == NULL)
		return (NULL);
	while (s[i + start] && end)
	{
		new_str[i] = s[i + start];
		i++;
		end--;
	}
	new_str[i] = '\0';
	return (new_str);
}
