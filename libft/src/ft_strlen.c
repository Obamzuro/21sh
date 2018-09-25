/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 19:45:39 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/17 11:08:07 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	int i;

	if (!s)
		return (0);
	i = -1;
	while (s[++i])
		;
	return (i);
}

size_t	ft_ustrlen(const char *p)
{
	size_t			i;
	size_t			ret;
	unsigned char	*s;

	s = (unsigned char *)p;
	if (!s)
		return (0);
	i = 0;
	ret = 0;
	while (s[i])
	{
		if (s[i] >> 5 == 0b110 ||
			s[i] >> 4 == 0b1110 ||
			s[i] >> 3 == 0b11110)
		{
			++i;
			while (s[i] && s[i] >> 6 == 0b10)
				++i;
		}
		else
			++i;
		++ret;
	}
	return (ret);
}
