/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fillglob.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/15 09:33:28 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/09 12:17:44 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void	fill_convs_plus(void)
{
	g_convs[9].ascii = 'c';
	g_convs[9].f = print_char;
	g_convs[10].ascii = 'C';
	g_convs[10].f = print_char;
	g_convs[11].ascii = 's';
	g_convs[11].f = print_string;
	g_convs[12].ascii = 'S';
	g_convs[12].f = print_string;
	g_convs[13].ascii = '%';
	g_convs[13].f = print_char;
	g_convs[14].ascii = 'p';
	g_convs[14].f = print_unsigned_hex;
}

void		fill_convs(void)
{
	g_convs[0].ascii = 'd';
	g_convs[0].f = print_decimal;
	g_convs[1].ascii = 'i';
	g_convs[1].f = print_decimal;
	g_convs[2].ascii = 'u';
	g_convs[2].f = print_unsigned;
	g_convs[3].ascii = 'D';
	g_convs[3].f = print_decimal;
	g_convs[4].ascii = 'U';
	g_convs[4].f = print_unsigned;
	g_convs[5].ascii = 'o';
	g_convs[5].f = print_unsigned_octal;
	g_convs[6].ascii = 'O';
	g_convs[6].f = print_unsigned_octal;
	g_convs[7].ascii = 'X';
	g_convs[7].f = print_unsigned_hex;
	g_convs[8].ascii = 'x';
	g_convs[8].f = print_unsigned_hex;
	fill_convs_plus();
}

void		fill_sizes(void)
{
	g_sizes[INTMAX_T].str = "j";
	g_sizes[INTMAX_T].len = 1;
	g_sizes[INTMAX_T].type = INTMAX_T;
	g_sizes[SIZE_T].str = "z";
	g_sizes[SIZE_T].len = 1;
	g_sizes[SIZE_T].type = SIZE_T;
	g_sizes[LONG_LONG_INT].str = "ll";
	g_sizes[LONG_LONG_INT].len = 2;
	g_sizes[LONG_LONG_INT].type = LONG_LONG_INT;
	g_sizes[LONG_INT].str = "l";
	g_sizes[LONG_INT].len = 1;
	g_sizes[LONG_INT].type = LONG_INT;
	g_sizes[SHORT_INT].str = "h";
	g_sizes[SHORT_INT].len = 1;
	g_sizes[SHORT_INT].type = SHORT_INT;
	g_sizes[CHAR].str = "hh";
	g_sizes[CHAR].len = 2;
	g_sizes[CHAR].type = CHAR;
	g_sizes[INT].str = "";
	g_sizes[INT].len = 0;
	g_sizes[INT].type = INT;
}

void		fill_flags(void)
{
	g_flags[minus].ascii = '-';
	g_flags[minus].exist = 0;
	g_flags[plus].ascii = '+';
	g_flags[plus].exist = 0;
	g_flags[space].ascii = ' ';
	g_flags[space].exist = 0;
	g_flags[sharp].ascii = '#';
	g_flags[sharp].exist = 0;
	g_flags[zero].ascii = '0';
	g_flags[zero].exist = 0;
}

void		reset_flags(void)
{
	g_flags[minus].exist = 0;
	g_flags[plus].exist = 0;
	g_flags[space].exist = 0;
	g_flags[sharp].exist = 0;
	g_flags[zero].exist = 0;
}
