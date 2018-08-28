/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getvarg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/15 08:16:19 by obamzuro          #+#    #+#             */
/*   Updated: 2018/08/28 20:49:58 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		get_unsigned(t_special *spec, va_list *ap, uintmax_t *n)
{
	t_size size;

	size = spec->size->type;
	if (size == LONG_LONG_INT)
		*n = va_arg(*ap, unsigned long long int);
	else if (size == LONG_INT)
		*n = va_arg(*ap, unsigned long int);
	else if (size == INTMAX_T)
		*n = va_arg(*ap, uintmax_t);
	else if (size == SIZE_T)
		*n = va_arg(*ap, ptrdiff_t);
	else
		*n = va_arg(*ap, unsigned int);
	if (size == SHORT_INT)
		*n = (unsigned short)*n;
	else if (size == CHAR)
		*n = (unsigned char)*n;
}

void		get_decimal(t_special *spec, va_list *ap, intmax_t *n)
{
	t_size size;

	size = spec->size->type;
	if (size == LONG_LONG_INT)
		*n = va_arg(*ap, long long);
	else if (size == LONG_INT)
		*n = va_arg(*ap, long);
	else if (size == INTMAX_T)
		*n = va_arg(*ap, intmax_t);
	else if (size == SIZE_T)
		*n = va_arg(*ap, size_t);
	else
		*n = va_arg(*ap, int);
	if (size == SHORT_INT)
		*n = (short)*n;
	else if (size == CHAR)
		*n = (signed char)*n;
}
