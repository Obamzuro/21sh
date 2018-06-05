/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 15:01:36 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 19:35:28 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const char	*cs;
	size_t		i;

	cs = (const char *)s;
	i = 0;
	while (i != n)
		if (cs[i++] == (char)c)
			return ((void *)(cs + i - 1));
	return (NULL);
}
