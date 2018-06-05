/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_nbr_size.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 13:26:34 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/09 11:47:48 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

unsigned char	pf_nbr_size(intmax_t n, char base, char alternative)
{
	size_t i;

	i = 1;
	n /= base;
	while (n)
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

unsigned char	pf_unbr_size(uintmax_t n, char base, char alternative)
{
	size_t i;

	i = 1;
	n /= base;
	while (n)
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
