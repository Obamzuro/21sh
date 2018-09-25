/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 22:04:15 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 18:09:49 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*a;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	a = (char *)malloc(ft_strlen(s) + 1);
	if (!a)
		return (NULL);
	i = -1;
	while (s[++i])
		a[i] = f(i, s[i]);
	a[i] = 0;
	return (a);
}
