/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 16:38:37 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 18:06:47 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	returning(long int a, char n)
{
	if (a < 0)
	{
		if (n == 1)
			return (-1);
		else
			return (0);
	}
	return (n * a);
}

int			ft_atoi(const char *str)
{
	char		n;
	long int	t;

	n = 1;
	while (*str == ' ' || *str == '\t' || *str == '\v' ||
			*str == '\n' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
	{
		n = -1;
		str++;
	}
	if (!*str || *str < '0' || *str > '9')
		return (0);
	t = *str - '0';
	while ((*++str && *str >= '0') && *str <= '9' && t >= 0)
		t = t * 10 + (*str - 48);
	return (returning(t, n));
}
