/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 10:06:53 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/09 10:06:57 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int chr)
{
	char	*p;
	char	*s;

	s = (char *)str;
	p = NULL;
	if (chr > 255)
		chr = chr % 256;
	while (*s)
	{
		if (*s == (char)chr)
		{
			p = (char *)s;
		}
		s++;
	}
	if (chr == '\0')
	{
		return ((char *)s);
	}
	return ((char *)p);
}
