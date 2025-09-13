/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 13:54:47 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/18 13:54:48 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*pnt;

	pnt = *lst;
	if (lst != NULL && new != NULL)
	{
		if (*lst == NULL)
		{
			*lst = new;
		}
		else
		{
			while (pnt -> next != NULL)
			{
				pnt = pnt -> next;
			}
			pnt -> next = new;
		}
	}
}
