/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/18 13:03:03 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/18 13:07:32 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char		**del_prev(char **a, size_t failed)
{
	size_t i;

	i = -1;
	while (++i < failed)
		free(a[i]);
	free(a);
	return (NULL);
}

char			**ft_strsplit2(const char *s, const char *c)
{
	char	**a;
	size_t	i;

	if (!s)
		return (NULL);
	a = (char **)malloc((ft_wcount2(s, c) + 1) * sizeof(char *));
	if (!a)
		return (NULL);
	i = 0;
	while (1)
	{
		while (ft_strchr(c, *s) && *s)
			s++;
		if (!*s)
			break ;
		a[i] = ft_strsub(s, 0, ft_ccount(s, c));
		if (!a[i])
			return (del_prev(a, i));
		i++;
		s += ft_ccount(s, c);
	}
	a[i] = 0;
	return (a);
}
