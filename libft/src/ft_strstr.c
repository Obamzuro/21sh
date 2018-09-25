/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 20:55:41 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 18:05:20 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *big, const char *little)
{
	size_t littlelen;

	littlelen = ft_strlen(little);
	if (!littlelen)
		return (char*)(big);
	while (*big)
		if (!ft_memcmp(big++, little, littlelen))
			return (char *)(big - 1);
	return (NULL);
}
