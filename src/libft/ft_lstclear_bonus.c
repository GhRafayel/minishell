/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:43:13 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/18 14:43:16 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*pnt;
	t_list	*temp;

	if (*lst != NULL && lst != NULL)
	{
		pnt = *lst;
		while (pnt != NULL)
		{
			temp = pnt->next;
			if (pnt->content != NULL)
			{
				del(pnt->content);
			}
			free(pnt);
			pnt = temp;
		}
		*lst = NULL;
	}
}
