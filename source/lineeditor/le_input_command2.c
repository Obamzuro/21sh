/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_input_command2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:06:40 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/25 13:31:28 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static void			check_trash(t_lineeditor *lineeditor, char *esc_command)
{
	int		esc_len;

	esc_len = ft_strlen(esc_command);
	if (esc_len >= 8)
		return ;
	if (lineeditor->letter[esc_len])
		ft_memcpy(lineeditor->trash,
				lineeditor->letter + esc_len, 8 - esc_len);
	lineeditor->letter[esc_len] = 0;
}

void				le_unselect(t_lineeditor *lineeditor,
		t_history *history, t_esc_corr *esc)
{
	if (((esc && !esc->is_selecting) || !esc) &&
		lineeditor->selectedmode)
	{
		lineeditor->selectedmode = 0;
		lineeditor->selected[0] = -1;
		lineeditor->selected[1] = -1;
		if (esc && !esc->is_printing)
			print_buffer(lineeditor, history, 0, 0);
	}
}

int					le_ignore_special_chars(t_lineeditor *lineeditor)
{
	extern char		g_ascii_ignore_chars[AM_ASCII_IGNORE + 1];

	if (!lineeditor->letter[1] &&
		lineeditor->letter[0] < 040 &&
		!ft_strchr(g_ascii_ignore_chars,
			lineeditor->letter[0]))
		return (1);
	return (0);
}

void				le_zero_hist_research(t_lineeditor *lineeditor,
		t_history *history)
{
	--history->last;
	free(history->commands[history->last]);
	history->commands[history->last] = 0;
	lineeditor->is_history_searched = 0;
	history->current = history->last;
}

int					le_line_editing(t_lineeditor *lineeditor,
		t_history *history)
{
	int					i;
	extern t_esc_corr	g_esc[AM_ESC];

	i = 0;
	while (i < AM_ESC)
	{
		if (ft_strnstr(lineeditor->letter, g_esc[i].str,
					ft_strlen(g_esc[i].str)))
		{
			check_trash(lineeditor, g_esc[i].str);
			le_unselect(lineeditor, history, g_esc + i);
			g_esc[i].func(lineeditor, history);
			return (1);
		}
		++i;
	}
	if (lineeditor->letter[0] == *ESC)
		return (1);
	return (0);
}
