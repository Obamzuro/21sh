/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/26 01:10:52 by obamzuro          #+#    #+#             */
/*   Updated: 2018/03/26 22:32:11 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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

static void	print_for_two(int c, int fd)
{
	char	b;

	b = (char)(((c >> 6) & 0xDF) | 0xC0);
	write(fd, &b, 1);
	b = (char)((c & 0xBF) | 0x80);
	write(fd, &b, 1);
}

static void	print_for_three(int c, int fd)
{
	char	b;

	b = (char)(((c >> 12) & 0xEF) | 0xE0);
	write(fd, &b, 1);
	b = (char)(((c >> 6) & 0xBF) | 0x80);
	write(fd, &b, 1);
	b = (char)((c & 0xBF) | 0x80);
	write(fd, &b, 1);
}

static void	print_for_four(int c, int fd)
{
	char	b;

	b = (char)(((c >> 18) & 0xF7) | 0xF0);
	write(fd, &b, 1);
	b = (char)(((c >> 12) & 0xBF) | 0x80);
	write(fd, &b, 1);
	b = (char)(((c >> 6) & 0xBF) | 0x80);
	write(fd, &b, 1);
	b = (char)((c & 0xBF) | 0x80);
	write(fd, &b, 1);
}

void		ft_putchar_fd(int c, int fd)
{
	char	b;

	b = count_bytes(c);
	if (b == 1)
		write(fd, &c, 1);
	else if (b == 2)
		print_for_two(c, fd);
	else if (b == 2)
		print_for_three(c, fd);
	else if (b == 3)
		print_for_four(c, fd);
}
