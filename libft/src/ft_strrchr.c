/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 20:44:53 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/06 22:14:16 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char *e;

	e = s + ft_strlen(s);
	while (*e != (unsigned char)c)
		if (e-- == s)
			return (NULL);
	return (char*)(e);
}
