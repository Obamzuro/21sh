/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 12:31:22 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void				print_buffer(t_lineeditor *lineeditor, t_history *history,
		char *newbuf, int offset);

void				line_editing_left_notmove(t_lineeditor *lineeditor,
		t_history *history);

int			ft_putc(int c)
{
	write(1, &c, 1);
	return (0);
}

int			handle_commands(char **args,
		char ***env)
{
	int		i;

	extern t_comm_corr g_commands[AM_COMMANDS];
	i = -1;
	while (++i < AM_COMMANDS)
	{
		if (!ft_strncmp(g_commands[i].comm, args[0],
					ft_strlen(g_commands[i].comm) + 1))
		{
			g_commands[i].func(args, env);
			break ;
		}
	}
	if (i == AM_COMMANDS)
		return (0);
	return (1);
}

void				history_append(char *command, t_history *history, int appendmode)
{
	char	*temp;
	int		i;

	if (history->last == AM_HISTORY && !appendmode)
	{
		i = -1;
		free(history->commands[0]);
		while (++i < AM_HISTORY - 1)
			history->commands[i] = history->commands[i + 1];
		history->commands[AM_HISTORY - 1] = 0;
		--history->last;
	}
	if (!appendmode)
	{
		history->commands[history->last] = ft_strdup(command);
		++history->last;
	}
	else
	{
		temp = history->commands[history->last - 1];
		history->commands[history->last - 1] =
			ft_strjoin(history->commands[history->last - 1], command);
		free(temp);
	}
	history->current = history->last;
}

int					free_ast(t_ast *ast)
{
	if (!ast)
		return (1);
	if (ast->type == REDIRECTION && ft_strequ((char *)ast->content, "<<"))
		free(((t_binary_token *)(ast->right->content))->right);
	free_ast(ast->left);
	free_ast(ast->right);
	if (ast->type == COMMAND)
		free_double_arr(ast->content);
	else
		free(ast->content);
	free(ast);
	return (1);
}

static void			get_cursor_position(int cursorpos[2])
{
	char	temp[16];

	cursorpos[0] = 0;
	cursorpos[1] = 0;
	/*
	**DANGEROUS
	*/
	write(2, "\x1B[6n", 4);
	while (read(2, temp, 1) && temp[0] != '\x1B')
		;
	read(2, temp + 1, 15);
	cursorpos[0] = ft_atoi(temp + 2);
	cursorpos[1] = ft_atoi(temp + 3 + ft_nbr_size(cursorpos[0]));
}

void				line_editing_left(t_lineeditor *lineeditor, t_history *history)
{
	line_editing_left_notmove(lineeditor, history);
	tputs(tgoto(tgetstr("cm", 0), lineeditor->curpos[1],
			lineeditor->curpos[0]), 1, &ft_putc);
}

void				line_editing_left_notmove_prev_lbreak(t_lineeditor *lineeditor)
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
				line_editing_left_notmove_prev_lbreak(lineeditor);
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

void				line_editing_up(t_lineeditor *lineeditor, t_history *history)
{
	if (history->current)
	{
		left_shift_cursor(lineeditor->seek, lineeditor, history);
		ft_putstr(tgetstr("le", 0));
		ft_putstr(tgetstr("le", 0));
		ft_putstr(tgetstr("le", 0));
		ft_putstr(tgetstr("cd", 0));
		ft_printf("%c> ", lineeditor->prompt);
		if (!lineeditor->is_history_searched && history->current == history->last)
		{
			history_append(lineeditor->buffer, history, 0);
			--history->current;
		}
		--history->current;
		free(lineeditor->buffer);
		lineeditor->buffer = ft_strdup((char *)history->commands[history->current]);
		write_line(lineeditor);
		lineeditor->seek = ft_strlen(lineeditor->buffer);
		lineeditor->is_history_searched = 1;
	}
}

void				line_editing_down(t_lineeditor *lineeditor, t_history *history)
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

void				write_line_chcurpos_kernel(t_lineeditor *lineeditor,
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

void				write_line_chcurpos(t_lineeditor *lineeditor)
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

void				line_editing_right_increment_seek(t_lineeditor *lineeditor,
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

void				line_editing_right(t_lineeditor *lineeditor, t_history *history)
{
	if (lineeditor->seek == (int)ft_strlen(lineeditor->buffer))
		return ;
	if (lineeditor->curpos[1] >= lineeditor->ws.ws_col - 1 &&
		(lineeditor->buffer[lineeditor->seek + 1] == '\n' ||
		 !lineeditor->buffer[lineeditor->seek + 1]))
	{
		++lineeditor->curpos[1];
//		ft_printf(tgetstr("nd", 0));
		ft_putstr(tgoto(tgetstr("cm", 0),
			lineeditor->curpos[1], lineeditor->curpos[0]));
	}
	else if (lineeditor->curpos[1] >= lineeditor->ws.ws_col - 1 ||
		lineeditor->buffer[lineeditor->seek] == '\n')
	{
		++lineeditor->curpos[0];
		lineeditor->curpos[1] = 0;
		ft_putstr(tgoto(tgetstr("cm", 0), 0, lineeditor->curpos[0]));
	}
	else
	{
		++lineeditor->curpos[1];
		ft_printf(tgetstr("nd", 0));
		ft_putstr(tgoto(tgetstr("cm", 0),
			lineeditor->curpos[1], lineeditor->curpos[0]));
	}
	line_editing_right_increment_seek(lineeditor, history);
}

void				line_editing_help(t_lineeditor *lineeditor, t_history *history)
{
	ft_printf("\ncursorpos[0]=%d  ws_col=%d\ncursorpos[1]=%d  ws_row=%d\nseek=%d\n",
			lineeditor->curpos[0], lineeditor->ws.ws_col,
			lineeditor->curpos[1], lineeditor->ws.ws_row,
			lineeditor->seek);
	int i = 0;

	while (i < 5)
	{
		ft_printf("%s\n", history->commands[i]);
		++i;
	}
	ft_printf("cur = %d\nlast = %d\nseek = %d\nbuffer=%s\nbuffer[seek]=%c\n", history->current, history->last, lineeditor->seek, lineeditor->buffer,
			lineeditor->buffer[lineeditor->seek]);
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

void				line_editing_backspace(t_lineeditor *lineeditor,
		t_history *history)
{
	char	*temp;
	int		i;

	if (lineeditor->seek)
	{
		i = 1;
		while (lineeditor->seek - i >= 0 &&
				(unsigned char)(lineeditor->buffer[lineeditor->seek - i]) >> 6 == 0b10)
			++i;
		temp = ft_strdup(lineeditor->buffer);
		temp[lineeditor->seek - i] = 0;
		print_buffer(lineeditor, history,
				ft_strjoin(temp, temp + lineeditor->seek), -i);
		free(temp);
	}
}

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

void				check_trash(t_lineeditor *lineeditor, char *esc_command)
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

int					le_line_editing(t_lineeditor *lineeditor, t_history *history)
{
	int					i;
	extern t_esc_corr	g_esc[AM_ESC];

	i = 0;
	while (i < AM_ESC)
	{
		if (ft_strnstr(lineeditor->letter, g_esc[i].str, ft_strlen(g_esc[i].str)))
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

void				print_buffer(t_lineeditor *lineeditor, t_history *history,
		char *newbuf, int offset)
{
	int		prevseek;
	char	*temp;

	prevseek = lineeditor->seek;
	left_shift_cursor(lineeditor->seek, lineeditor, history);
	tputs(tgetstr("le", 0), 1, &ft_putc);
	tputs(tgetstr("le", 0), 1, &ft_putc);
	tputs(tgetstr("le", 0), 1, &ft_putc);
	tputs(tgetstr("cd", 0), 1, &ft_putc);
	ft_printf("%c> ", lineeditor->prompt);
	if (newbuf)
	{
		temp = lineeditor->buffer;
		lineeditor->buffer = newbuf;
		free(temp);
	}
	write_line(lineeditor);
	lineeditor->seek += ft_strlen(lineeditor->buffer);
	left_shift_cursor(ft_strlen(lineeditor->buffer) - prevseek -
			offset, lineeditor, history);
}

void				input_command_prep(t_lineeditor *lineeditor, char prompt,
		t_shell *shell)
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
	get_cursor_position(lineeditor->curpos);
	--lineeditor->curpos[1];
	--lineeditor->curpos[0];
	lineeditor->prompt = prompt;
	lineeditor->buffer = ft_memalloc(1);
	lineeditor->shell = shell;
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

void				input_command_print_buffer(t_lineeditor *lineeditor,
		t_history *history)
{
	if (lineeditor->seek != (int)ft_strlen(lineeditor->buffer))
		print_buffer(lineeditor, history, ft_strjoin_inner(lineeditor->buffer,
				lineeditor->letter, lineeditor->seek),
				ft_strlen(lineeditor->letter));
	else
		print_buffer(lineeditor, history, ft_strjoin(lineeditor->buffer,
				lineeditor->letter), ft_strlen(lineeditor->letter));
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

int					le_reading(t_lineeditor *lineeditor, int offset, int size)
{
	if (read(0, lineeditor->letter + offset, size) == -1)
	{
		/*
		**WINCH
		*/
		if (g_sigwinch)
		{
			if (ioctl(STDIN_FILENO, TIOCGWINSZ, &lineeditor->ws) == -1 ||
				!lineeditor->ws.ws_col || !lineeditor->ws.ws_row)
			{
				ft_fprintf(STDERR_FILENO, "21sh: GetWinsize error\n");
				exit(EXIT_FAILURE);
			}
			lineeditor->selected[0] = -1;
			lineeditor->selected[1] = -1;
			get_cursor_position(lineeditor->curpos);
			--lineeditor->curpos[1];
			--lineeditor->curpos[0];
			g_sigwinch = 0;
			return (0);
		}
		/*
		**Ctrl-c
		*/
		else
			return (-1);
	}
	return (0);
}

int					le_is_trash_exist(t_lineeditor *lineeditor)
{
	if (lineeditor->trash[0])
	{
		ft_strcpy(lineeditor->letter, lineeditor->trash);
		ft_bzero(lineeditor->trash, sizeof(lineeditor->trash));
		return (1);
	}
	return (0);
}

int					after_read_unicode(t_lineeditor *lineeditor)
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

int					input_command_kernel(t_lineeditor *lineeditor,
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
		if (ft_strequ(lineeditor->letter, BACKSPACE)
				&& lineeditor->is_history_searched)
			le_zero_hist_research(lineeditor, history);
		return (1);
	}
	return (0);
}

char				*input_command(t_lineeditor *lineeditor,
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

void				preparation(t_shell *shell)
{
	struct sigaction		act;
	extern struct termios	g_tty;

	ft_bzero(&act, sizeof(act));
	act.sa_handler = int_handler;
	term_associate();
	tcgetattr(STDIN_FILENO, &g_tty);
	if ((shell->initfd.fdin = dup(0)) == -1 ||
		(shell->initfd.fdout = dup(1)) == -1 ||
		(shell->initfd.fderr = dup(2)) == -1)
	{
		ft_fprintf(2, "21sh: dup() error\n");
		exit(EXIT_FAILURE);
	}
	shell->env = fill_env();
	shell->lexer = (t_lexer *)malloc(sizeof(t_lexer));
	ft_bzero(&shell->history, sizeof(shell->history));
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGSTOP, SIG_IGN);
	if (sigaction(SIGINT, &act, 0) == -1 ||
		sigaction(SIGWINCH, &act, 0) == -1)
	{
		ft_fprintf(2, "21sh: sigaction error\n");
		exit(EXIT_FAILURE);
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
	offset = right - left;
	if (lineeditor->cpbuf)
		free(lineeditor->cpbuf);
	lineeditor->cpbuf = ft_strsub(lineeditor->buffer, left, offset);
	temp = ft_strsub(lineeditor->buffer, 0, left);
	print_buffer(lineeditor, history,
			ft_strjoin(temp, lineeditor->buffer + right), 0);
	free(temp);
	lineeditor->selected[0] = -1;
	lineeditor->selected[1] = -1;
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

int					tilde_expansion(t_shell *shell, char **args)
{
	int			i;
	char		*temp;

	//if (!getenv("HOME"))
	//	return (0);
	i = -1;
	while (args[++i])
	{
		if (args[i][0] == '~')
		{
//			if (elem[i]->str[1] == '/')
//			{
				temp = ft_strjoin(get_env("HOME", shell->env), args[i] + 1);
				free(args[i]);
				args[i] = temp;
//			}
		}
	}
	return (0);
}

void				quote_removing_str(char **str)
{
	char		*temp;
	char		*temp2;
	char		*quote;
	char		*quote2;
	int			nextsearch;

	nextsearch = 0;
	while (*str && **str)
	{
		if (!(quote = ft_strchr_str(*str + nextsearch, "\'\"")) ||
			!(quote2 = ft_strchr(quote + 1, *quote)))
			return ;
		nextsearch = quote2 - *str - 1;
		*quote = 0;
		*quote2 = 0;
		temp = *str;
		temp2 = ft_strjoin(*str, quote + 1);
		*str = ft_strjoin(temp2, quote2 + 1);
		free(temp);
		free(temp2);
		if (!*(*str + nextsearch))
			break ;
	}
}

int					quote_removing(t_shell *shell, char **args)
{
	int			i;

	(void)shell;
	i = -1;
	while (args[++i])
		quote_removing_str(&args[i]);
	return (0);
}

void				free_lineeditor(t_lineeditor *lineeditor)
{
	free(lineeditor->buffer);
	free(lineeditor->cpbuf);
}

char				*lineediting(t_shell *shell)
{
	char		*command;

	if (!(command = input_command(&shell->lineeditor,
					&shell->history, '$', shell)))
	{
		le_unselect(&shell->lineeditor, &shell->history, NULL);
		print_buffer(&shell->lineeditor, &shell->history, 0, 0);
		line_editing_end(&shell->lineeditor, &shell->history);
		ft_printf("\n");
		free_lineeditor(&shell->lineeditor);
		return (NULL);
	}
	le_unselect(&shell->lineeditor, &shell->history, NULL);
	print_buffer(&shell->lineeditor, &shell->history, 0, 0);
	line_editing_end(&shell->lineeditor, &shell->history);
	return (command);
}

int					lexing(t_shell *shell, char *command)
{
	if (lexer_creating(command, shell))
	{
		free_lexer(shell->lexer);
		return (-1);
	}
	write(1, "\n", 1);
	if (!shell->lexer->tokens.len)
	{
		free_lexer(shell->lexer);
		--shell->history.last;
		free(shell->history.commands[shell->history.last]);
		shell->history.commands[shell->history.last] = 0;
		return (-1);
	}
	return (0);
}

int					creating_ast(t_shell *shell)
{
	if (!(shell->ast = create_separator_ast(0,
					shell->lexer->tokens.len - 1, 0, shell)))
	{
		free_lexer(shell->lexer);
		return (-1);
	}
	return (0);
}

int					main(void)
{
	char		*command;
	t_shell		shell; 
	preparation(&shell);
	while (1)
	{
		change_termios(&shell.initfd, 0);
		ft_bzero(shell.lexer, sizeof(t_lexer));
		ft_printf("$> ");
		shell.reading_mode = BASIC;
		if (!(command = lineediting(&shell)) ||
			lexing(&shell, command) ||
			creating_ast(&shell))
			continue;
		change_termios(&shell.initfd, 1);
		parse_ast(shell.ast, &shell, 1);
		free_lexer(shell.lexer);
		free_ast(shell.ast);
	}
	free_double_arr(shell.env);
	free(shell.lexer);
	return (0);
}
