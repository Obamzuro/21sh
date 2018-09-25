/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_backspace.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 13:18:35 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/25 13:32:59 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void				line_editing_backspace_select(t_lineeditor *lineeditor,
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
	temp = ft_strsub(lineeditor->buffer, 0, left);
	print_buffer(lineeditor, history,
			ft_strjoin(temp, lineeditor->buffer + right), offset);
	free(temp);
	lineeditor->selected[0] = -1;
	lineeditor->selected[1] = -1;
}

void				line_editing_backspace(t_lineeditor *lineeditor,
		t_history *history)
{
	char	*temp;
	int		i;

	if (lineeditor->selectedmode)
		line_editing_backspace_select(lineeditor, history);
	else if (lineeditor->seek)
	{
		le_unselect(lineeditor, history, 0);
		i = 1;
		while (lineeditor->seek - i >= 0 &&
				(unsigned char)(lineeditor->buffer[lineeditor->seek - i])
				>> 6 == 0b10)
			++i;
		temp = ft_strdup(lineeditor->buffer);
		temp[lineeditor->seek - i] = 0;
		print_buffer(lineeditor, history,
				ft_strjoin(temp, temp + lineeditor->seek), -i);
		free(temp);
	}
}
