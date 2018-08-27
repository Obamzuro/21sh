/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsigned_hex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/15 06:53:13 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/09 12:02:38 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	print_nsymb(size_t diff, char symb, t_buffer *buff)
{
	size_t		i;

	i = 0;
	while (i < diff)
	{
		pf_write(symb, buff);
		i++;
	}
}

static void	print_prefix(t_special *spec, t_buffer *buff)
{
	if (g_flags[sharp].exist)
	{
		if (spec->conversion->ascii == 'x')
		{
			pf_write('0', buff);
			pf_write('x', buff);
		}
		if (spec->conversion->ascii == 'X')
		{
			pf_write('0', buff);
			pf_write('X', buff);
		}
	}
}

static void	calc_diffs(t_special *spec, ssize_t *nsize,
		t_diffs *diffs, t_unumber *number)
{
	diffs->diffprec = 0;
	diffs->diffwidth = 0;
	*nsize = pf_unbr_size(number->num, 16, 0);
	number->nbrsize = *nsize;
	if (!number->num && spec->precision != -1)
		*nsize = 0;
	if (spec->precision > *nsize)
		diffs->diffprec = spec->precision - *nsize;
	*nsize += diffs->diffprec;
	if (g_flags[sharp].exist && number->num)
		*nsize += 2;
	if ((ssize_t)spec->width > *nsize)
		diffs->diffwidth = spec->width - *nsize;
}

static void	stabilize_width(t_special *spec, uintmax_t n, t_buffer *buff)
{
	ssize_t		nsize;
	t_diffs		diffs;
	t_unumber	number;

	number.num = n;
	calc_diffs(spec, &nsize, &diffs, &number);
	if (number.num && spec->precision == -1 && g_flags[zero].exist)
		print_prefix(spec, buff);
	if (!g_flags[minus].exist)
	{
		if (g_flags[zero].exist && spec->precision == -1)
			print_nsymb(diffs.diffwidth, '0', buff);
		else
			print_nsymb(diffs.diffwidth, ' ', buff);
	}
	if (number.num && (spec->precision != -1 || !g_flags[zero].exist))
		print_prefix(spec, buff);
	print_nsymb(diffs.diffprec, '0', buff);
	if (number.num || (!number.num && spec->precision == -1))
		pf_uputnbr_common(&number, 16, spec->conversion->ascii == 'X', buff);
	if (g_flags[minus].exist)
		print_nsymb(diffs.diffwidth, ' ', buff);
}

void		print_unsigned_hex(t_special *spec, va_list *ap, t_buffer *buff)
{
	uintmax_t	n;

	get_unsigned(spec, ap, &n);
	stabilize_width(spec, n, buff);
}
