/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strfind.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/28 20:49:04 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/18 17:08:40 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_is_str_in_args(const char *str, int amount_args, ...)
{
	va_list		ap;
	int			i;

	if (amount_args < 0)
		return (0);
	va_start(ap, amount_args);
	i = -1;
	while (++i != amount_args)
		if (ft_strequ(str, va_arg(ap, char *)))
			break ;
	va_end(ap);
	if (i == amount_args)
		return (0);
	return (1);
}

int		ft_is_char_in_str(const char a, const char *str)
{
	if (!str)
		return (0);
	while (*str)
		if (a == *str++)
			return (1);
	return (0);
}
