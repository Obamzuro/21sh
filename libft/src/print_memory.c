/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: exam <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 15:45:05 by exam              #+#    #+#             */
/*   Updated: 2018/06/12 18:36:29 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	print_hex(unsigned char a)
{
	char *base;

	base = "0123456789abcdef";
	write(1, base + (a / 16), 1);
	write(1, base + (a % 16), 1);
}

void	my_bzero(unsigned char *buf)
{
	int i;

	i = 0;
	while (i < 16)
	{
		buf[i] = 0;
		i++;
	}
}

void	print_buf(unsigned char *buf, unsigned int size)
{
	unsigned int i;

	if (!size)
		return ;
	i = 0;
	while (i < size)
	{
		if (buf[i] < 32 || buf[i] > 126)
			write(1, ".", 1);
		else
			write(1, buf + i, 1);
		i++;
	}
	write(1, "\n", 1);
}

void	print_offset(unsigned int i)
{
	int j;

	j = i % 16;
	if (!j)
		return ;
	while (j < 16)
	{
		write(1, "  ", 2);
		j++;
		if (j % 2 == 0)
			write(1, " ", 1);
	}
}

void	print_memory(const void *addr, unsigned int size)
{
	unsigned int		i;
	const unsigned char	*a;
	unsigned char		buf[16];

	i = 0;
	a = (const unsigned char *)addr;
	my_bzero(buf);
	while (i < size)
	{
		print_hex(*a);
		buf[i % 16] = *a;
		a++;
		i++;
		if (i % 2 == 0)
			write(1, " ", 1);
		if (i % 16 == 0)
		{
			print_buf(buf, 16);
			my_bzero(buf);
		}
	}
	print_offset(i);
	print_buf(buf, i % 16);
}
