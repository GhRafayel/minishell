/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 09:47:14 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/10 09:47:18 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*ps;
	unsigned char	cc;

	ps = (unsigned char *)s;
	cc = (unsigned int)c;
	while (n--)
	{
		if (*ps == cc)
		{
			return ((void *)ps);
		}
		ps++;
	}
	return (0);
}
