/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_home_end_del.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:10:21 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/25 13:16:22 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void				line_editing_home(t_lineeditor *lineeditor,
		t_history *history)
{
	left_shift_cursor(lineeditor->seek, lineeditor, history);
}

void				line_editing_end(t_lineeditor *lineeditor,
		t_history *history)
{
	int		i;

	i = ft_strlen(lineeditor->buffer) - lineeditor->seek;
	while (i)
	{
		line_editing_right(lineeditor, history);
		--i;
	}
}

void				line_editing_del(t_lineeditor *lineeditor,
		t_history *history)
{
	if (lineeditor->seek == (int)ft_strlen(lineeditor->buffer))
		return ;
	lineeditor->buffer[lineeditor->seek] = 0;
	print_buffer(lineeditor, history, ft_strjoin(lineeditor->buffer,
				lineeditor->buffer + lineeditor->seek + 1), 0);
}

void				line_editing_eot(t_lineeditor *lineeditor,
		t_history *history)
{
	if (lineeditor->buffer && *lineeditor->buffer)
		line_editing_del(lineeditor, history);
	else
	{
		if (lineeditor->shell->reading_mode == BASIC)
		{
			ft_printf("\n");
			change_termios(&lineeditor->shell->initfd, 1);
			exit(0);
		}
		else
			lineeditor->shell->reading_mode = READEND;
	}
}
