/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/10 15:51:20 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/09 15:12:19 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		pf_filling_globals(void)
{
	fill_sizes();
	fill_convs();
	fill_flags();
}

int			ft_printf(const char *src, ...)
{
	va_list		ap;
	t_buffer	buff;

	buff.cur = 0;
	buff.ret = 0;
	buff.fd = 1;
	buff.line = malloc(PRINTF_BUFF_SIZE);
	va_start(ap, src);
	fill_sizes();
	fill_convs();
	fill_flags();
	while (*src)
	{
		if (*src == '%')
		{
			++src;
			print_special(&src, &ap, &buff);
			continue;
		}
		pf_write(*(src++), &buff);
	}
	va_end(ap);
	pf_write_tail(&buff);
	free(buff.line);
	return (buff.ret * PRINTF_BUFF_SIZE + buff.cur);
}

void		ft_fprintf(int fd, const char *src, ...)
{
	va_list		ap;
	t_buffer	buff;

	buff.cur = 0;
	buff.fd = fd;
	buff.line = malloc(PRINTF_BUFF_SIZE);
	va_start(ap, src);
	while (*src)
	{
		if (*src == '%')
		{
			++src;
			print_special(&src, &ap, &buff);
			continue;
		}
		pf_write(*src, &buff);
		++src;
	}
	va_end(ap);
	pf_write_tail(&buff);
	free(buff.line);
}

size_t		ft_snprintf(char *line, size_t cur, const char *src, ...)
{
	va_list		ap;
	t_buffer	buff;

	buff.line = line;
	buff.cur = cur;
	buff.fd = 1;
	va_start(ap, src);
	while (*src)
	{
		if (*src == '%')
		{
			++src;
			print_special(&src, &ap, &buff);
			continue;
		}
		pf_write(*src, &buff);
		++src;
	}
	va_end(ap);
	return (buff.cur);
}
