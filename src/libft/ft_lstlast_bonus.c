/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:47:54 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/18 13:47:55 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*pnt;

	pnt = lst;
	if (pnt != NULL)
	{
		while (pnt -> next != NULL)
		{
			pnt = pnt -> next;
		}
	}
	return (pnt);
}
