/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 19:57:00 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/02 18:55:43 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char *r;

	if (!s)
		return (NULL);
	r = (char *)malloc(ft_strlen(s) + 1);
	if (!r)
		return (NULL);
	ft_memcpy(r, s, ft_strlen(s) + 1);
	return (r);
}
