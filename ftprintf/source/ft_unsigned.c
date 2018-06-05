/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsigned.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/13 00:40:19 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/09 15:28:28 by obamzuro         ###   ########.fr       */
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

static void	calc_nsize(t_special *spec, t_unumber *number, ssize_t *nsize)
{
	*nsize = pf_unbr_size(number->num, 10, 0);
	if (!number->num && !spec->precision)
		*nsize = 0;
	number->nbrsize = *nsize;
}

static void	stabilize_width(t_special *spec, uintmax_t n, t_buffer *buff)
{
	ssize_t			nsize;
	t_diffs			diffs;
	t_unumber		number;

	diffs.diffprec = 0;
	diffs.diffwidth = 0;
	number.num = n;
	calc_nsize(spec, &number, &nsize);
	if (spec->precision > nsize)
		diffs.diffprec = spec->precision - nsize;
	nsize += diffs.diffprec;
	if ((ssize_t)spec->width > nsize)
		diffs.diffwidth = spec->width - nsize;
	if (!g_flags[minus].exist)
	{
		if (g_flags[zero].exist && spec->precision == -1)
			print_nsymb(diffs.diffwidth, '0', buff);
		else
			print_nsymb(diffs.diffwidth, ' ', buff);
	}
	print_nsymb(diffs.diffprec, '0', buff);
	if (number.num || spec->precision)
		pf_uputnbr_common(&number, 10, 0, buff);
	if (g_flags[minus].exist)
		print_nsymb(diffs.diffwidth, ' ', buff);
}

void		print_unsigned(t_special *spec, va_list *ap, t_buffer *buff)
{
	uintmax_t	n;

	get_unsigned(spec, ap, &n);
	stabilize_width(spec, n, buff);
}
