/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 14:07:16 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 18:05:58 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_memncmp(const void *s1, const void *s2, size_t n, size_t l)
{
	const char	*cs[2];
	size_t		i;

	cs[0] = (const char *)s1;
	cs[1] = (const char *)s2;
	i = 0;
	if (!cs[0][0] && !cs[1][0])
		return (0);
	while (i != n && cs[0][i] == cs[1][i])
		i++;
	if (i > l)
		return (1);
	if (n == i)
		return (0);
	return (cs[0][i] - cs[1][i]);
}

char		*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t littlelen;

	littlelen = ft_strlen(little);
	if (!littlelen)
		return (char *)(big);
	while (*big && len)
		if (!ft_memncmp(big++, little, littlelen, len--))
			return (char *)(big - 1);
	return (NULL);
}
