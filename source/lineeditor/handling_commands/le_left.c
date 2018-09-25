/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_left.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:10:23 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 14:12:29 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void				line_editing_left(t_lineeditor *lineeditor,
		t_history *history)
{
	line_editing_left_notmove(lineeditor, history);
	tputs(tgoto(tgetstr("cm", 0), lineeditor->curpos[1],
			lineeditor->curpos[0]), 1, &ft_putc);
}

static void			line_editing_left_notmove_lbreak(t_lineeditor *lineeditor)
{
	int				i;

	--lineeditor->curpos[0];
	i = lineeditor->seek - 2;
	while (i >= 0 && lineeditor->buffer[i] != '\n')
	{
		while (lineeditor->buffer[i] & 0x80 &&
				~lineeditor->buffer[i] & 0x40)
			--i;
		++lineeditor->curpos[1];
		--i;
	}
	if (i < 0)
		lineeditor->curpos[1] += 3;
	if (lineeditor->curpos[1] >= lineeditor->ws.ws_col)
	{
		lineeditor->curpos[1] = lineeditor->curpos[1] %
			lineeditor->ws.ws_col;
		if (!lineeditor->curpos[1])
			lineeditor->curpos[1] = lineeditor->ws.ws_col;
	}
}

void				line_editing_left_notmove(t_lineeditor *lineeditor,
		t_history *history)
{
	(void)history;
	if (lineeditor->seek)
	{
		if (lineeditor->curpos[1] == 0)
		{
			if (lineeditor->buffer[lineeditor->seek - 1] == '\n')
				line_editing_left_notmove_lbreak(lineeditor);
			else
			{
				--lineeditor->curpos[0];
				lineeditor->curpos[1] = lineeditor->ws.ws_col - 1;
			}
		}
		else
			--lineeditor->curpos[1];
		while (lineeditor->buffer[lineeditor->seek - 1] & 0x80 &&
				~lineeditor->buffer[lineeditor->seek - 1] & 0x40)
			--lineeditor->seek;
		--lineeditor->seek;
	}
}

void				left_shift_cursor(int amount, t_lineeditor *lineeditor,
		t_history *history)
{
	int				i;
	int				seek;

	if (!amount)
		return ;
	i = 0;
	while (i < amount)
	{
		seek = lineeditor->seek;
		while (lineeditor->buffer[seek - 1] & 0x80 &&
			~lineeditor->buffer[seek - 1] & 0x40)
		{
			++i;
			--seek;
		}
		line_editing_left_notmove(lineeditor, history);
		++i;
	}
	if (lineeditor->curpos[0] < 0)
	{
		lineeditor->curpos[0] = 0;
		lineeditor->curpos[1] = 3;
	}
	tputs(tgoto(tgetstr("cm", 0), lineeditor->curpos[1],
			lineeditor->curpos[0]), 1, &ft_putc);
}
