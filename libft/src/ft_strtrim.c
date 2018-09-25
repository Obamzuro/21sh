/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 22:38:11 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 18:10:35 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(const char *s)
{
	const char	*a;
	char		*b;
	size_t		i;

	if (!s)
		return (NULL);
	a = s + ft_strlen(s);
	i = 0;
	while ((s[i] == ' ' || s[i] == '\n' || s[i] == '\t') && s[++i])
		;
	while ((*a == ' ' || *a == '\n' || *a == '\t' || !*a) && a != s + i)
		a--;
	if (*a != ' ' && *a != '\n' && *a != '\t' && *a)
		b = ft_strsub(s, i, a - s - i + 1);
	else
		b = ft_strsub(s, i, a - s - i);
	return (b);
}
