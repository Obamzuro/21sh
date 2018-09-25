/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 16:55:10 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 19:19:30 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*cs[2];
	size_t				i;

	cs[0] = (const unsigned char *)s1;
	cs[1] = (const unsigned char *)s2;
	i = 0;
	if ((!cs[0] && !cs[1] && !n) || (!cs[0][0] && !cs[1][0]))
		return (0);
	while (i != n && cs[0][i] == cs[1][i])
		i++;
	if (n == i)
		return (0);
	return (cs[0][i] - cs[1][i]);
}
