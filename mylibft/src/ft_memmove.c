/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 14:20:00 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 18:01:38 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*cdst;
	const char	*csrc;
	size_t		i;
	char		a;

	cdst = (char *)dst;
	csrc = (const char *)src;
	a = 1;
	if (dst > src)
	{
		cdst += len - 1;
		csrc += len - 1;
		a = -1;
	}
	i = -1;
	while (++i < len)
	{
		*cdst = *csrc;
		cdst += a;
		csrc += a;
	}
	return (dst);
}
