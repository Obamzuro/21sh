/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_alts_help.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:10:17 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/04 14:37:00 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void				line_editing_altup(t_lineeditor *lineeditor,
		t_history *history)
{
	int		x;

	if (!lineeditor->seek)
		return ;
	x = lineeditor->curpos[1];
	line_editing_left(lineeditor, history);
	while (lineeditor->seek && lineeditor->curpos[1] != x)
		line_editing_left(lineeditor, history);
}

void				line_editing_altdown(t_lineeditor *lineeditor,
		t_history *history)
{
	int		x;
	int		buflen;

	buflen = ft_strlen(lineeditor->buffer);
	if (lineeditor->seek == buflen)
		return ;
	x = lineeditor->curpos[1];
	line_editing_right(lineeditor, history);
	while (lineeditor->seek != buflen && lineeditor->curpos[1] != x)
		line_editing_right(lineeditor, history);
}

void				line_editing_begword(t_lineeditor *lineeditor,
		t_history *history)
{
	if (lineeditor->seek)
	{
		line_editing_left(lineeditor, history);
		while (lineeditor->seek > 0 &&
				ft_is_char_in_str(lineeditor->buffer[lineeditor->seek], " \n"))
			line_editing_left(lineeditor, history);
		while (lineeditor->seek > 0 &&
			!ft_is_char_in_str(lineeditor->buffer[lineeditor->seek - 1], " \n"))
			line_editing_left(lineeditor, history);
	}
}

void				line_editing_endword(t_lineeditor *lineeditor,
		t_history *history)
{
	int				buflen;

	buflen = ft_strlen(lineeditor->buffer);
	if (lineeditor->seek != buflen)
	{
		line_editing_right(lineeditor, history);
		while (lineeditor->seek != buflen &&
				ft_is_char_in_str(lineeditor->buffer[lineeditor->seek], " \n"))
			line_editing_right(lineeditor, history);
		while (lineeditor->seek != buflen &&
			!ft_is_char_in_str(lineeditor->buffer[lineeditor->seek], " \n"))
			line_editing_right(lineeditor, history);
	}
}

void				line_editing_help(t_lineeditor *lineeditor,
		t_history *history)
{
	(void)lineeditor;
	(void)history;
	ft_printf("seek = %d\n", lineeditor->seek);
	ft_printf("curpos[0] = %d\n", lineeditor->curpos[0]);
	ft_printf("curpos[1] = %d\n", lineeditor->curpos[1]);
}
