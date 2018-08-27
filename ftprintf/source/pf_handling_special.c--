/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pf_handling_special.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 12:07:49 by obamzuro          #+#    #+#             */
/*   Updated: 2018/08/11 12:02:53 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	read_width(const char **src, va_list *ap)
{
	if (**src == '*')
	{
		++(*src);
		return (va_arg(*ap, int));
	}
	else
		return (ft_positive_atoi(src));
}

void	fix_conversion(t_special *spec)
{
	if (spec->conversion->ascii == 'D' || spec->conversion->ascii == 'U' ||
			spec->conversion->ascii == 'O')
		spec->size = g_sizes + LONG_INT;
	else if (spec->conversion->ascii == 'p')
	{
		g_flags[sharp].exist = 1;
		spec->conversion->ascii = 'x';
		spec->size = g_sizes + LONG_INT;
	}
}

void	print_special(const char **src, va_list *ap, t_buffer *buff)
{
	t_special	special;

	read_flags(src);
	special.width = read_width(src, ap);
	special.precision = read_precision(src, ap);
	special.size = read_size(src);
	special.conversion = read_conversion(src);
	fix_conversion(&special);
	special.conversion->f(&special, ap, buff);
}
