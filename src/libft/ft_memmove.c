/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memove.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 16:20:50 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/04 16:20:52 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*pdest;
	const unsigned char	*psrc;

	pdest = dest;
	psrc = src;
	if (pdest == psrc || n == 0)
		return (dest);
	if (pdest > psrc && pdest < psrc + n)
	{
		pdest += n;
		psrc += n;
		while (n--)
			*--pdest = *--psrc;
	}
	else
	{
		while (n--)
			*pdest++ = *psrc++;
	}
	return (dest);
}
