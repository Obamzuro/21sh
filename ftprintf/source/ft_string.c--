/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 19:42:28 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/09 15:08:50 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void		print_nsymb(size_t diff, char symb, t_buffer *buff)
{
	size_t		i;

	i = 0;
	while (i < diff)
	{
		pf_write(symb, buff);
		i++;
	}
}

static size_t	print_str(t_special *spec, char *n, size_t size, t_buffer *buff)
{
	size_t num;

	num = 0;
	if (spec->conversion->ascii == 's')
	{
		pf_putstr(n, size, buff);
		num = size;
	}
	else if (spec->conversion->ascii == 'S')
		num = pf_wputstr((wchar_t *)n, buff);
	return (num);
}

static void		stabilize_width(t_special *spec, char *n, t_buffer *buff)
{
	size_t		nsize;
	size_t		diffwidth;

	diffwidth = 0;
	nsize = 0;
	nsize = spec->conversion->ascii == 's' ? pf_strlen(n)
		: pf_wstrlen((wchar_t *)n);
	if (spec->precision >= 0 &&
			spec->precision < (ssize_t)nsize && spec->conversion->ascii == 's')
		nsize = spec->precision;
	if (spec->width > nsize)
		diffwidth = spec->width - nsize;
	if (!g_flags[minus].exist)
	{
		if (g_flags[zero].exist)
			print_nsymb(diffwidth, '0', buff);
		else
			print_nsymb(diffwidth, ' ', buff);
	}
	nsize = print_str(spec, n, nsize, buff);
	if (g_flags[minus].exist)
		print_nsymb(diffwidth, ' ', buff);
}

void			print_string(t_special *spec, va_list *ap, t_buffer *buff)
{
	char	*n;
	char	ret;

	n = va_arg(*ap, char*);
	ret = 0;
	if (spec->conversion->ascii == 's' && spec->size->type == LONG_INT)
	{
		spec->conversion->ascii = 'S';
		ret = 1;
	}
	if (!n)
	{
		if (spec->conversion->ascii == 's')
			n = "(null)";
		else if (spec->conversion->ascii == 'S')
			n = (char *)L"(null)";
	}
	stabilize_width(spec, n, buff);
	if (ret)
		spec->conversion->ascii = 's';
}
