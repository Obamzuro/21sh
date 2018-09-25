/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wcount.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 17:44:28 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/18 13:10:21 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_wcount(const char *s, char c)
{
	size_t i;

	i = 0;
	while (1)
	{
		while (*s == c && *s)
			s++;
		if (!*s)
			break ;
		i++;
		while (*s != c && *s)
			s++;
		if (!*s)
			break ;
	}
	return (i);
}

size_t	ft_wcount2(const char *s, const char *c)
{
	size_t i;

	i = 0;
	while (1)
	{
		while (ft_strchr(c, *s) && *s)
			s++;
		if (!*s)
			break ;
		i++;
		while (!ft_strchr(c, *s) && *s)
			s++;
		if (!*s)
			break ;
	}
	return (i);
}
