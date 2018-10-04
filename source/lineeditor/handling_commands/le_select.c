/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:10:27 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/04 17:05:14 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void				line_editing_shiftleft(t_lineeditor *lineeditor,
		t_history *history)
{
	if (lineeditor->seek)
	{
		lineeditor->selectedmode = 1;
		if (lineeditor->selected[1] == -1)
			lineeditor->selected[1] = lineeditor->seek;
		line_editing_left(lineeditor, history);
		lineeditor->selected[0] = lineeditor->seek;
		print_buffer(lineeditor, history, 0, 0);
	}
}

void				line_editing_shiftright(t_lineeditor *lineeditor,
		t_history *history)
{
	if (lineeditor->seek != (int)ft_strlen(lineeditor->buffer))
	{
		lineeditor->selectedmode = 1;
		if (lineeditor->selected[1] == -1)
			lineeditor->selected[1] = lineeditor->seek;
		line_editing_right(lineeditor, history);
		lineeditor->selected[0] = lineeditor->seek;
		print_buffer(lineeditor, history, 0, 0);
	}
}

void				line_editing_altx(t_lineeditor *lineeditor,
		t_history *history)
{
	int		offset;
	int		left;
	int		right;
	char	*temp;
	int		prevseek;

	prevseek = lineeditor->seek;
	if (!lineeditor->selectedmode)
		return ;
	lineeditor->selectedmode = 0;
	left = lineeditor->selected[1] > lineeditor->selected[0] ?
		lineeditor->selected[0] : lineeditor->selected[1];
	right = lineeditor->selected[1] > lineeditor->selected[0] ?
		lineeditor->selected[1] : lineeditor->selected[0];
	offset = lineeditor->selected[1] < lineeditor->selected[0] ?
		-right + left : 0;
	lineeditor->cpbuf ? free(lineeditor->cpbuf) : 0;
	lineeditor->cpbuf = ft_strsub(lineeditor->buffer, left, right - left);
	temp = ft_strsub(lineeditor->buffer, 0, left);
	print_buffer(lineeditor, history,
			ft_strjoin(temp, lineeditor->buffer + right), offset);
	free(temp);
	lineeditor->selected[0] = -1;
	lineeditor->selected[1] = -1;
}

void				line_editing_altv(t_lineeditor *lineeditor,
		t_history *history)
{
	size_t		i;

	if (!lineeditor->cpbuf || !*lineeditor->cpbuf)
		return ;
	i = 0;
	print_buffer(lineeditor, history, ft_strjoin_inner(lineeditor->buffer,
			lineeditor->cpbuf, lineeditor->seek), 0);
	while (i < ft_ustrlen(lineeditor->cpbuf) - 1)
	{
		line_editing_right(lineeditor, history);
		++i;
	}
	line_editing_right(lineeditor, history);
}

void				line_editing_altc(t_lineeditor *lineeditor,
		t_history *history)
{
	int		offset;
	int		left;
	int		right;

	(void)history;
	if (!lineeditor->selectedmode)
		return ;
	left = lineeditor->selected[0];
	right = lineeditor->selected[1];
	if (left > right)
	{
		left = lineeditor->selected[1];
		right = lineeditor->selected[0];
	}
	offset = right - left;
	if (lineeditor->cpbuf)
		free(lineeditor->cpbuf);
	lineeditor->cpbuf = ft_strsub(lineeditor->buffer, left, offset);
}
