/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_right_up_down.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:10:25 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/25 14:22:28 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static void			line_editing_right_incrseek(t_lineeditor *lineeditor,
		t_history *history)
{
	(void)history;
	if ((unsigned char)lineeditor->buffer[lineeditor->seek] >> 5
		== 0b110 ||
		(unsigned char)lineeditor->buffer[lineeditor->seek] >> 4
		== 0b1110 ||
		(unsigned char)lineeditor->buffer[lineeditor->seek] >> 3
		== 0b11110)
	{
		++lineeditor->seek;
		while (lineeditor->buffer[lineeditor->seek] &&
				(unsigned char)lineeditor->buffer[lineeditor->seek] >> 6
				== 0b10)
			++lineeditor->seek;
	}
	else
		++(lineeditor->seek);
}

void				line_editing_right(t_lineeditor *lineeditor,
		t_history *history)
{
	if (lineeditor->seek == (int)ft_strlen(lineeditor->buffer))
		return ;
	if (lineeditor->curpos[1] >= lineeditor->ws.ws_col - 1 &&
		(lineeditor->buffer[lineeditor->seek + 1] == '\n' ||
		!lineeditor->buffer[lineeditor->seek + 1]))
	{
		++lineeditor->curpos[1];
		tputs(tgoto(tgetstr("cm", 0),
			lineeditor->curpos[1], lineeditor->curpos[0]), 1, ft_putc);
	}
	else if (lineeditor->curpos[1] >= lineeditor->ws.ws_col - 1 ||
		lineeditor->buffer[lineeditor->seek] == '\n')
	{
		++lineeditor->curpos[0];
		lineeditor->curpos[1] = 0;
		tputs(tgoto(tgetstr("cm", 0), 0, lineeditor->curpos[0]), 1, ft_putc);
	}
	else
	{
		++lineeditor->curpos[1];
		ft_printf(tgetstr("nd", 0));
		tputs(tgoto(tgetstr("cm", 0),
			lineeditor->curpos[1], lineeditor->curpos[0]), 1, ft_putc);
	}
	line_editing_right_incrseek(lineeditor, history);
}

void				line_editing_up(t_lineeditor *lineeditor,
		t_history *history)
{
	if (history->current)
	{
		left_shift_cursor(lineeditor->seek, lineeditor, history);
		ft_putstr(tgetstr("le", 0));
		ft_putstr(tgetstr("le", 0));
		ft_putstr(tgetstr("le", 0));
		ft_putstr(tgetstr("cd", 0));
		ft_printf("%c> ", lineeditor->prompt);
		if (!lineeditor->is_history_searched &&
				history->current == history->last)
		{
			history_append(lineeditor->buffer, history, 0);
			--history->current;
		}
		--history->current;
		free(lineeditor->buffer);
		lineeditor->buffer =
			ft_strdup((char *)history->commands[history->current]);
		write_line(lineeditor);
		lineeditor->seek = ft_strlen(lineeditor->buffer);
		lineeditor->is_history_searched = 1;
	}
}

void				line_editing_down(t_lineeditor *lineeditor,
		t_history *history)
{
	if (history->current < history->last - 1)
	{
		left_shift_cursor(lineeditor->seek, lineeditor, history);
		ft_putstr(tgetstr("le", 0));
		ft_putstr(tgetstr("le", 0));
		ft_putstr(tgetstr("le", 0));
		ft_putstr(tgetstr("cd", 0));
		ft_printf("%c> ", lineeditor->prompt);
		++history->current;
		free(lineeditor->buffer);
		lineeditor->buffer = ft_strdup(history->commands[history->current]);
		write_line(lineeditor);
		lineeditor->seek = ft_strlen(lineeditor->buffer);
	}
}
