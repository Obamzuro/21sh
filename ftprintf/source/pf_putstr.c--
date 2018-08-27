/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 13:23:41 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/09 11:30:24 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	count_bytes(int a)
{
	if (a <= 0x7F)
		return (1);
	if (a <= 0x7FF)
		return (2);
	if (a <= 0xFFFF)
		return (3);
	return (4);
}

void		pf_putstr(const char *s, size_t size, t_buffer *buff)
{
	size_t i;

	if (!s)
		return ;
	i = 0;
	while (*s && i < size)
	{
		pf_write(*s, buff);
		++s;
		++i;
	}
}

size_t		pf_wputstr(const wchar_t *s, t_buffer *buff)
{
	size_t res;

	if (!s)
		return (0);
	res = 0;
	while (*s)
	{
		pf_putchar(*s, buff);
		res += count_bytes(*s);
		++s;
	}
	return (res);
}
