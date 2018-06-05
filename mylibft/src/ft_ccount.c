/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ccount.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 17:41:34 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/18 13:09:39 by obamzuro         ###   ########.fr       */
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

size_t	ft_ccount2(const char *s, const char *c)
{
	size_t i;

	i = 0;
	while (*s && !ft_strchr(c, *s++))
		i++;
	return (i);
}
