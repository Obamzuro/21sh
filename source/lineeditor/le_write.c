/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_write.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:06:47 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 14:09:57 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static void			write_line_chcurpos_kernel(t_lineeditor *lineeditor,
		char *str)
{
	int		linewidth;

	if (str != lineeditor->buffer)
	{
		lineeditor->curpos[1] = 0;
		++lineeditor->curpos[0];
	}
	linewidth = lineeditor->curpos[1] + ft_uccount(str, '\n');
	if (linewidth && !(linewidth % lineeditor->ws.ws_col) &&
		!(str[-1] == '\n' && !*str))
	{
		--lineeditor->curpos[0];
		lineeditor->curpos[1] = lineeditor->ws.ws_col;
	}
	else
		lineeditor->curpos[1] = linewidth % (lineeditor->ws.ws_col);
	lineeditor->curpos[0] += linewidth / (lineeditor->ws.ws_col);
	if (lineeditor->curpos[0] >= lineeditor->ws.ws_row)
		lineeditor->curpos[0] = lineeditor->ws.ws_row - 1;
}

static void			write_line_chcurpos(t_lineeditor *lineeditor)
{
	char	*str;

	if (!lineeditor->buffer)
		return ;
	str = lineeditor->buffer - 1;
	while (1)
	{
		++str;
		write_line_chcurpos_kernel(lineeditor, str);
		str += ft_ccount(str, '\n');
		if (!*str)
			break ;
	}
}

void				write_line(t_lineeditor *lineeditor)
{
	int		offset;
	int		left;
	int		right;
	char	*reverse;

	reverse = tgetstr("mr", 0);
	left = lineeditor->selected[0];
	right = lineeditor->selected[1];
	if (left > right)
	{
		left = lineeditor->selected[1];
		right = lineeditor->selected[0];
	}
	offset = right - left;
	if (lineeditor->selectedmode)
	{
		ft_printf("%.*s%s%.*s%s%s", left, lineeditor->buffer,
			reverse, offset, lineeditor->buffer + left,
			DEFAULT, lineeditor->buffer + left + offset);
	}
	else
		ft_putstr(lineeditor->buffer);
	write_line_chcurpos(lineeditor);
}
