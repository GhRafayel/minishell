/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 13:53:18 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/04 13:53:22 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*pdest;
	unsigned const char	*psrc;

	pdest = dest;
	psrc = src;
	if (pdest == NULL && psrc == NULL)
	{
		return (NULL);
	}
	while (n--)
	{
		*pdest++ = *psrc++;
	}
	return (dest);
}
