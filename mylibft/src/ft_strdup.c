/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/21 19:57:00 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 18:03:37 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char *r;

	r = (char *)malloc(ft_strlen(s) + 1);
	if (!r)
		return (NULL);
	ft_memcpy(r, s, ft_strlen(s) + 1);
	return (r);
}
