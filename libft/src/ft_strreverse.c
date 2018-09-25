/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreverse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 18:11:01 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/12 18:19:16 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_strreverse(char *str, uint64_t strlen)
{
	char		temp;
	uint64_t	i;

	if (!str || !strlen)
		return ;
	i = 0;
	--strlen;
	while (i < strlen)
	{
		temp = str[i];
		str[i] = str[strlen];
		str[strlen] = temp;
		++i;
		--strlen;
	}
}
