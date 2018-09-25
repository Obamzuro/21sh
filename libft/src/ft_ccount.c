/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ccount.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 17:41:34 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 14:34:41 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_ccount(const char *s, char c)
{
	size_t i;

	i = 0;
	while (*s && *s++ != c)
		i++;
	return (i);
}

size_t	ft_uccount(const char *p, char c)
{
	size_t			i;
	unsigned char	*s;

	s = (unsigned char *)p;
	i = 0;
	while (*s && *s != c)
	{
		if (*s >> 5 == 0b110 ||
			*s >> 4 == 0b1110 ||
			*s >> 3 == 0b11110)
		{
			++s;
			while (*s && *s >> 6 == 0b10)
				++s;
		}
		else
			++s;
		++i;
	}
	return (i);
}

size_t	ft_ccount2(const char *s, const char *c)
{
	size_t i;

	i = 0;
	while (*s && !ft_strchr(c, *s++))
		i++;
	return (i);
}
