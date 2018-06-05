/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsigned_octal.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/15 08:32:53 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/09 15:25:30 by obamzuro         ###   ########.fr       */
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

static void		fix_nsize(t_special *spec, uintmax_t n, ssize_t *nsize)
{
	if (!n)
	{
		if (!spec->precision)
		{
			if (g_flags[sharp].exist)
				*nsize = 1;
			else
				*nsize = 0;
		}
		else if (g_flags[sharp].exist)
			*nsize = 1;
	}
}

static void		calc_diffs(t_special *spec,
		ssize_t *nsize, t_diffs *diffs, t_unumber *number)
{
	diffs->diffprec = 0;
	diffs->diffwidth = 0;
	*nsize = pf_unbr_size(number->num, 8, g_flags[sharp].exist);
	number->nbrsize = *nsize;
	fix_nsize(spec, number->num, nsize);
	if (spec->precision > *nsize)
		diffs->diffprec = spec->precision - *nsize;
	*nsize += diffs->diffprec;
	if ((ssize_t)spec->width > *nsize)
		diffs->diffwidth = spec->width - *nsize;
	number->nbrsize -= g_flags[sharp].exist;
}

static void		stabilize_width(t_special *spec, uintmax_t n, t_buffer *buff)
{
	ssize_t		nsize;
	t_diffs		diffs;
	t_unumber	number;

	number.num = n;
	calc_diffs(spec, &nsize, &diffs, &number);
	if (!g_flags[minus].exist)
		g_flags[zero].exist && spec->precision == -1 ?
			print_nsymb(diffs.diffwidth, '0', buff) :
			print_nsymb(diffs.diffwidth, ' ', buff);
	print_nsymb(diffs.diffprec, '0', buff);
	if (g_flags[sharp].exist &&
			(number.num || (!number.num && !spec->precision)))
		pf_write('0', buff);
	if (number.num || spec->precision)
		pf_uputnbr_common(&number, 8, 0, buff);
	if (g_flags[minus].exist)
		print_nsymb(diffs.diffwidth, ' ', buff);
}

void			print_unsigned_octal(t_special *spec,
		va_list *ap, t_buffer *buff)
{
	uintmax_t	n;

	get_unsigned(spec, ap, &n);
	stabilize_width(spec, n, buff);
}
