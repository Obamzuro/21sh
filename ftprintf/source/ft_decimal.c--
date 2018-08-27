/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prints.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/11 23:00:51 by obamzuro          #+#    #+#             */
/*   Updated: 2018/08/12 18:37:31 by obamzuro         ###   ########.fr       */
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

static void	print_sign(intmax_t n, t_buffer *buff)
{
	if (n < 0)
		pf_write('-', buff);
	else if (g_flags[plus].exist)
		pf_write('+', buff);
	else if (g_flags[space].exist)
		pf_write(' ', buff);
}

static void	calc_diffs(t_special *spec,
		ssize_t *nsize, t_diffs *diffs, t_number *number)
{
	diffs->diffprec = 0;
	diffs->diffwidth = 0;
	*nsize = pf_nbr_size(number->num, 10, 0);
	number->nbrsize = *nsize;
	if (!number->num && !spec->precision)
		*nsize = 0;
	if (spec->precision > *nsize)
		diffs->diffprec = spec->precision - *nsize;
	*nsize += diffs->diffprec;
	if (g_flags[plus].exist || number->num < 0 || g_flags[space].exist)
		++*nsize;
	if ((ssize_t)spec->width > *nsize)
		diffs->diffwidth = spec->width - *nsize;
}

static void	stabilize_width(t_special *spec, intmax_t n, t_buffer *buff)
{
	ssize_t			nsize;
	t_diffs			diffs;
	t_number		number;

	number.num = n;
	calc_diffs(spec, &nsize, &diffs, &number);
	if (g_flags[minus].exist || (g_flags[zero].exist
		&& spec->precision == -1 && (g_flags[plus].exist
		|| number.num < 0 || g_flags[space].exist)))
		print_sign(number.num, buff);
	if (!g_flags[minus].exist && g_flags[zero].exist
			&& spec->precision == -1)
		print_nsymb(diffs.diffwidth, '0', buff);
	else if (!g_flags[minus].exist)
		print_nsymb(diffs.diffwidth, ' ', buff);
	if (!g_flags[minus].exist && (!g_flags[zero].exist
				|| (spec->precision != -1 && (g_flags[plus].exist
						|| number.num < 0 || g_flags[space].exist))))
		print_sign(number.num, buff);
	print_nsymb(diffs.diffprec, '0', buff);
	if (number.num || spec->precision)
		pf_putnbr_common(&number, 10, 0, buff);
	if (g_flags[minus].exist)
		print_nsymb(diffs.diffwidth, ' ', buff);
}

void		print_decimal(t_special *spec, va_list *ap, t_buffer *buff)
{
	intmax_t	n;

	get_decimal(spec, ap, &n);
	stabilize_width(spec, n, buff);
}
