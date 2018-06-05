/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 12:36:34 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/09 11:34:01 by obamzuro         ###   ########.fr       */
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

static void	print_for_two(int c, t_buffer *buff)
{
	char	b;

	b = (char)(((c >> 6) & 0xDF) | 0xC0);
	pf_write(b, buff);
	b = (char)((c & 0xBF) | 0x80);
	pf_write(b, buff);
}

static void	print_for_three(int c, t_buffer *buff)
{
	char	b;

	b = (char)(((c >> 12) & 0xEF) | 0xE0);
	pf_write(b, buff);
	b = (char)(((c >> 6) & 0xBF) | 0x80);
	pf_write(b, buff);
	b = (char)((c & 0xBF) | 0x80);
	pf_write(b, buff);
}

static void	print_for_four(int c, t_buffer *buff)
{
	char	b;

	b = (char)(((c >> 18) & 0xF7) | 0xF0);
	pf_write(b, buff);
	b = (char)(((c >> 12) & 0xBF) | 0x80);
	pf_write(b, buff);
	b = (char)(((c >> 6) & 0xBF) | 0x80);
	pf_write(b, buff);
	b = (char)((c & 0xBF) | 0x80);
	pf_write(b, buff);
}

int			pf_putchar(wchar_t c, t_buffer *buff)
{
	char	b;

	b = count_bytes(c);
	if (b == 1)
		pf_write(c, buff);
	else if (b == 2)
		print_for_two(c, buff);
	else if (b == 3)
		print_for_three(c, buff);
	else if (b == 4)
		print_for_four(c, buff);
	return (b);
}
