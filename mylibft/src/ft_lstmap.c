/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 02:53:35 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/27 11:00:51 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		dellstcontent(void *a, size_t b)
{
	b = 0;
	free(a);
}

static t_list	*ft_lstmap_after_begin_filler(t_list *dop[3],
		t_list *lst, t_list *(*f)(t_list *elem))
{
	while (lst)
	{
		dop[2] = f(lst);
		if (!dop[2])
		{
			ft_lstdel(&(dop[0]), dellstcontent);
			return (NULL);
		}
		dop[1]->next = ft_lstnew(dop[2]->content, dop[2]->content_size);
		free(dop[2]);
		dop[1] = dop[1]->next;
		if (!dop[1])
		{
			ft_lstdel((&dop[0]), dellstcontent);
			return (NULL);
		}
		lst = lst->next;
	}
	return (dop[0]);
}

t_list			*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *dop[3];

	if (!lst || !f)
		return (NULL);
	dop[2] = f(lst);
	if (!dop[2])
		return (NULL);
	dop[0] = ft_lstnew(dop[2]->content, dop[2]->content_size);
	free(dop[2]);
	if (!dop[0])
		return (NULL);
	dop[1] = dop[0];
	lst = lst->next;
	return (ft_lstmap_after_begin_filler(dop, lst, f));
}
