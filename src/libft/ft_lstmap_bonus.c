/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rghazary <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 16:08:39 by rghazary          #+#    #+#             */
/*   Updated: 2024/09/18 16:08:42 by rghazary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;
	void	*pnt;

	new_list = NULL;
	if (lst == NULL || f == NULL || !del)
	{
		return (NULL);
	}
	while (lst != NULL)
	{
		pnt = f(lst -> content);
		if (pnt != NULL)
			new_node = ft_lstnew(pnt);
		if (new_node == NULL || pnt == NULL)
		{
			free(pnt);
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst -> next;
	}
	return (new_list);
}
