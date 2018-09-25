/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_input_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:06:37 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/25 14:29:56 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int					le_is_trash_exist(t_lineeditor *lineeditor)
{
	if (lineeditor->trash[0])
	{
		ft_strcpy(lineeditor->letter, lineeditor->trash);
		ft_bzero(lineeditor->trash, sizeof(lineeditor->trash));
		return (1);
	}
	return (0);
}

static int					after_read_unicode(t_lineeditor *lineeditor)
{
	unsigned char	firstletter;
	int				extra_bytes;

	firstletter = lineeditor->letter[0];
	if (firstletter & 0x80)
	{
		if (firstletter >> 5 == 0b110)
			extra_bytes = 1;
		else if (firstletter >> 4 == 0b1110)
			extra_bytes = 2;
		else if (firstletter >> 3 == 0b11110)
			extra_bytes = 3;
		else
			return (1);
		if (le_reading(lineeditor, 1, extra_bytes))
			return (-1);
	}
	else if (firstletter == 0x1b)
		if (le_reading(lineeditor, 1, 7))
			return (-1);
	return (0);
}

static int					input_command_kernel(t_lineeditor *lineeditor,
		t_history *history, int ret)
{
	if (le_is_trash_exist(lineeditor))
	{
		if (le_reading(lineeditor, ft_strlen(lineeditor->trash),
					READING_SIZE - ft_strlen(lineeditor->trash)))
			return (-1);
	}
	else
	{
		if (le_reading(lineeditor, 0, 1))
			return (-1);
		ret = after_read_unicode(lineeditor);
		if (ret > 0)
			return (1);
		else if (ret < 0)
			return (-1);
	}
	if (le_ignore_special_chars(lineeditor) ||
		le_line_editing(lineeditor, history))
	{
		if (ft_is_str_in_args(lineeditor->letter, 3, BACKSPACE, ALTX, ALTV)
				&& lineeditor->is_history_searched)
			le_zero_hist_research(lineeditor, history);
		return (1);
	}
	return (0);
}

static void					input_command_prep(t_lineeditor *lineeditor,
		char prompt, t_shell *shell)
{
	ft_bzero(lineeditor, sizeof(t_lineeditor));
	ft_bzero(lineeditor->trash, sizeof(lineeditor->trash));
	if (ioctl(0, TIOCGWINSZ, &lineeditor->ws) == -1 ||
			!lineeditor->ws.ws_col ||
			!lineeditor->ws.ws_row)
	{
		ft_fprintf(STDERR_FILENO, "21sh: getWinSize error\n");
		exit(EXIT_FAILURE);
	}
	lineeditor->selected[0] = -1;
	lineeditor->selected[1] = -1;
	get_cursor_position(lineeditor->curpos, lineeditor);
	--lineeditor->curpos[1];
	--lineeditor->curpos[0];
	lineeditor->prompt = prompt;
	lineeditor->buffer = ft_memalloc(1);
	lineeditor->shell = shell;
}

char						*input_command(t_lineeditor *lineeditor,
		t_history *history, char prompt, t_shell *shell)
{
	extern volatile sig_atomic_t	g_sigwinch;
	int								ret;

	input_command_prep(lineeditor, prompt, shell);
	ret = 0;
	while (1)
	{
		if (lineeditor->shell->reading_mode == READEND)
			break ;
		ft_bzero(lineeditor->letter, sizeof(lineeditor->letter));
		ret = input_command_kernel(lineeditor, history, ret);
		if (ret > 0)
			continue;
		if (ret < 0)
			return (NULL);
		le_unselect(lineeditor, history, NULL);
		if (lineeditor->is_history_searched)
			le_zero_hist_research(lineeditor, history);
		if (ft_strequ(lineeditor->letter, "\n"))
			break ;
		input_command_print_buffer(lineeditor, history);
	}
	free(lineeditor->cpbuf);
	return (lineeditor->buffer);
}
