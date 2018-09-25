/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_nbr_size.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 13:26:34 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/01 13:42:20 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_nbr_size(intmax_t n)
{
	size_t i;

	i = 1;
	while (n / 10)
	{
		++i;
		n /= 10;
	}
	return (i);
}

size_t	ft_nbr_basesize(intmax_t n, char base, char alternative)
{
	size_t i;

	i = 1;
	while (n / base)
	{
		++i;
		n /= base;
	}
	if (alternative)
	{
		if (base == 8)
			++i;
		else if (base == 16)
			i += 2;
	}
	return (i);
}

size_t	ft_unbr_basesize(uintmax_t n, char base, char alternative)
{
	size_t i;

	i = 1;
	while (n / base)
	{
		++i;
		n /= base;
	}
	if (alternative)
	{
		if (base == 8)
			++i;
		else if (base == 16)
			i += 2;
	}
	return (i);
}
