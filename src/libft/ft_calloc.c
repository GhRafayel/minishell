/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:13:44 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/09 11:13:50 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t num, size_t size)
{
	char		*pn;
	size_t		big;

	big = ((size_t) - 1);
	if (num != 0 && size > (big / num))
		return (NULL);
	pn = (char *)malloc (num * size);
	if (pn == NULL)
	{
		return (NULL);
	}
	ft_bzero(pn, (size * num));
	return (pn);
}
