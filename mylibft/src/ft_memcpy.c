/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 21:49:18 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 23:02:50 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	long	*larrs[2];
	char	*arrs[2];
	size_t	i;

	larrs[0] = (long *)dst;
	larrs[1] = (long *)src;
	arrs[0] = (char *)dst;
	arrs[1] = (char *)src;
	i = 0;
	while (i < n / sizeof(long))
	{
		larrs[0][i] = larrs[1][i];
		i++;
	}
	i *= sizeof(long) / sizeof(char);
	while (i < n / sizeof(char))
	{
		arrs[0][i] = arrs[1][i];
		i++;
	}
	return (dst);
}
