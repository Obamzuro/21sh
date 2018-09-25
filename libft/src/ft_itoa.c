/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 00:24:01 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 18:23:08 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fill_it(char *a, int n, size_t len)
{
	size_t	r;
	char	b;

	if (n < 0)
		b = -1;
	else
		b = 1;
	a[len] = 0;
	r = 0;
	while (len - r)
	{
		a[len - ++r] = (b * (n % 10)) + 48;
		n /= 10;
	}
}

char		*ft_itoa(int n)
{
	char	*a;
	size_t	r;
	int		b;
	char	isneg;

	b = n;
	r = 1;
	isneg = 0;
	if (n < 0)
		isneg = 1;
	while (n / 10)
	{
		r++;
		n /= 10;
	}
	a = (char *)malloc(r + 1 + isneg);
	if (!a)
		return (NULL);
	if (isneg)
		*a = '-';
	fill_it(a + isneg, b, r);
	return (a);
}
