/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 19:49:31 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 18:08:12 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	void *a;

	a = malloc(size);
	if (!a)
		return (NULL);
	while (size)
		(*((char *)a + --size)) = 0;
	return (a);
}
