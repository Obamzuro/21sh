/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 21:58:25 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 18:09:26 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	char	*a;
	size_t	i;

	if (!s || !f)
		return (NULL);
	a = (char *)malloc(ft_strlen(s) + 1);
	if (!a)
		return (NULL);
	i = -1;
	while (s[++i])
		a[i] = f(s[i]);
	a[i] = 0;
	return (a);
}
