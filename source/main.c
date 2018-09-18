/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/18 18:30:20 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void				print_buffer(t_lineeditor *lineeditor, t_history *history,
		char *newbuf, int offset);

void				line_editing_left_notmove(t_lineeditor *lineeditor,
		t_history *history);

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

static void	get_cursor_position(int cursorpos[2])
{
	char	temp[16];

	cursorpos[0] = 0;
	cursorpos[1] = 0;
	write(2, "\x1B[6n", 4);
	read(2, &temp, 16);
	cursorpos[0] = ft_atoi(temp + 2);
	cursorpos[1] = ft_atoi(temp + 3 + ft_nbr_size(cursorpos[0]));
}

void				line_editing_left(t_lineeditor *lineeditor, t_history *history)
{
	line_editing_left_notmove(lineeditor, history);
	ft_putstr(tgoto(tgetstr("cm", 0), lineeditor->curpos[1],
			lineeditor->curpos[0]));
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

void				write_line_chcurpos(t_lineeditor *lineeditor)
{
	char	*str;
	int		linewidth;

	str = lineeditor->buffer;
	while (str && *str)
	{
		if (str != lineeditor->buffer)
		{
			lineeditor->curpos[1] = 0;
			++lineeditor->curpos[0];
			++str;
		}
		linewidth = lineeditor->curpos[1] + ft_uccount(str, '\n');
		if (!(linewidth % lineeditor->ws.ws_col) &&
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
	ft_putstr(tgoto(tgetstr("cm", 0), lineeditor->curpos[1],
		lineeditor->curpos[0]));
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
	if ((unsigned char)lineeditor->buffer[lineeditor->seek] >> 5 == 0b110 ||
		(unsigned char)lineeditor->buffer[lineeditor->seek] >> 4 == 0b1110 ||
		(unsigned char)lineeditor->buffer[lineeditor->seek] >> 3 == 0b11110)
	{
		++lineeditor->seek;
		while (lineeditor->buffer[lineeditor->seek] &&
				(unsigned char)lineeditor->buffer[lineeditor->seek] >> 6 == 0b10)
			++lineeditor->seek;
	}
	else
		++(lineeditor->seek);
}

void				line_editing_right(t_lineeditor *lineeditor, t_history *history)
{
	if (lineeditor->seek == (int)ft_strlen(lineeditor->buffer))
		return ;
	if (lineeditor->ws.ws_col - 1 <= lineeditor->curpos[1] &&
		lineeditor->buffer[lineeditor->seek + 1] == '\n')
	{
		++lineeditor->curpos[1];
		ft_printf(tgetstr("nd", 0));
		ft_putstr(tgoto(tgetstr("cm", 0),
			lineeditor->curpos[1], lineeditor->curpos[0]));
	}
	else if (lineeditor->ws.ws_col - 1 <= lineeditor->curpos[1] ||
		lineeditor->buffer[lineeditor->seek] == '\n')
	{
		++lineeditor->curpos[0];
		ft_putstr(tgoto(tgetstr("cm", 0), 0, lineeditor->curpos[0]));
		lineeditor->curpos[1] = 0;
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
				lineeditor->buffer[lineeditor->seek - i] >> 6 == 0b10)
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

void				line_editing_unselect(t_lineeditor *lineeditor, t_history *history)
{
	if (!ft_strequ(lineeditor->letter, SHIFTLEFT) && 
		!ft_strequ(lineeditor->letter, SHIFTRIGHT) &&
		!ft_strequ(lineeditor->letter, ALTX) &&
		!ft_strequ(lineeditor->letter, ALTC) &&
		lineeditor->selectedmode)
	{
		lineeditor->selectedmode = 0;
		lineeditor->selected[0] = -1;
		lineeditor->selected[1] = -1;
		print_buffer(lineeditor, history, 0, 0);
	}
}

int					line_editing(t_lineeditor *lineeditor, t_history *history)
{
	int					i;
	extern t_esc_corr	g_esc[AM_ESC];

	i = 0;
	line_editing_unselect(lineeditor, history);
	while (i < AM_ESC)
	{
		if (ft_strequ(lineeditor->letter, g_esc[i].str))
		{
			g_esc[i].func(lineeditor, history);
			return (1);
		}
		++i;
	}
	return (0);
}

void				print_buffer(t_lineeditor *lineeditor, t_history *history,
		char *newbuf, int offset)
{
	int		prevseek;
	char	*temp;

	prevseek = lineeditor->seek;
	left_shift_cursor(lineeditor->seek, lineeditor, history);
	ft_printf("%s%s%s%s", tgetstr("le", 0), tgetstr("le", 0),
			tgetstr("le", 0), tgetstr("cd", 0));
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
			return (-1);
		if (read(0, lineeditor->letter + 1, extra_bytes) != extra_bytes)
			return (-1);
	}
	else if (firstletter == 0x1b)
		if (read(0, lineeditor->letter + 1, 7) < 1)
			return (-1);
	return (0);
}

int					input_command_prep(t_lineeditor *lineeditor, char prompt)
{
	ft_bzero(lineeditor, sizeof(t_lineeditor));
	if (ioctl(0, TIOCGWINSZ, &lineeditor->ws) == -1 ||
			!lineeditor->ws.ws_col ||
			!lineeditor->ws.ws_row)
		return (-1);
	lineeditor->selected[0] = -1;
	lineeditor->selected[1] = -1;
	get_cursor_position(lineeditor->curpos);
	--lineeditor->curpos[1];
	--lineeditor->curpos[0];
	lineeditor->prompt = prompt;
	return (0);
}

void				input_command_after_hist_research(t_lineeditor *lineeditor,
		t_history *history)
{
	--history->last;
	free(history->commands[history->last]);
	history->commands[history->last] = 0;
	lineeditor->is_history_searched = 0;
	history->current = history->last;
}

int					lineeditor_free(t_lineeditor *lineeditor)
{
	free(lineeditor->cpbuf);
	return (1);
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

int					ignore_special_chars(t_lineeditor *lineeditor)
{
	extern char		g_ascii_ignore_chars[AM_ASCII_IGNORE + 1];

	if (!lineeditor->letter[1] &&
		lineeditor->letter[0] < 040 && !ft_strchr(g_ascii_ignore_chars,
			lineeditor->letter[0]))
		return (1);
	return (0);
}

char				*input_command(t_lineeditor *lineeditor,
		t_history *history, char prompt)
{
	if (input_command_prep(lineeditor, prompt))
		return (NULL);
	while (1)
	{
		ft_bzero(lineeditor->letter, sizeof(lineeditor->letter));
		if ((read(0, lineeditor->letter, 1) != 1
				|| after_read_unicode(lineeditor))
				&& lineeditor_free(lineeditor))
			return (NULL);
		if (ignore_special_chars(lineeditor) || line_editing(lineeditor, history))
		{
			if (ft_strequ(lineeditor->letter, BACKSPACE)
					&& lineeditor->is_history_searched)
				input_command_after_hist_research(lineeditor, history);
			continue ;
		}
		if (lineeditor->is_history_searched)
			input_command_after_hist_research(lineeditor, history);
		if (ft_strequ(lineeditor->letter, "\n"))
			break ;
		input_command_print_buffer(lineeditor, history);
	}
	lineeditor_free(lineeditor);
	return (lineeditor->buffer);
}

void				winch_handler(int sig)
{
	int	curpos[2];

	if (sig == SIGWINCH)
	{
		get_cursor_position(curpos);
	}
}

void				preparation(t_shell *shell)
{
	struct sigaction	act;

	ft_bzero(&act, sizeof(act));
	act.sa_handler = int_handler;
//	sigemptyset(&act.sa_mask);
//	act.sa_mask |= SA_INTERRUPT;
	term_associate();
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGSTOP, SIG_IGN);
//	signal(SIGWINCH, winch_handler);
//	signal(SIGINT, int_handler);
	if ((shell->initfd.fdin = dup(0)) == -1)
	{
		write(1, "0", 1);
		ft_fprintf(2, "21sh: stdin isn't terminal\n");
		exit(EXIT_FAILURE);
	}
	if ((shell->initfd.fdout = dup(1)) == -1)
	{
		write(2, "1", 1);
		shell->initfd.fdout = -1;
	}
	if ((shell->initfd.fderr = dup(2)) == -1)
	{
		write(1, "2", 1);
		shell->initfd.fderr = -1;
	}
//	dup(0);
	shell->env = fill_env();
	shell->lexer = (t_lexer *)malloc(sizeof(t_lexer));
	ft_bzero(&shell->history, sizeof(shell->history));
	sigaction(SIGINT, &act, 0);
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
		exit(0);
}

int					tilde_expansion(t_shell *shell)
{
	t_token		**elem;
	int			i;
	char		*temp;

	if (!getenv("HOME"))
		return (0);
	elem = (t_token **)shell->lexer->tokens.elem;
	i = -1;
	while (++i < shell->lexer->tokens.len)
	{
		if (elem[i]->str[0] == '~')
		{
			if (elem[i]->str[1] == '/')
			{
				temp = ft_strjoin(getenv("HOME"), elem[i]->str + 1);
				free(elem[i]->str);
				elem[i]->str = temp;
			}
		}
	}
	return (0);
}

int					quote_removing(t_shell *shell)
{
	t_token		**tokens;
	int			i;
	int			j;
	char		*temp;

	tokens = (t_token **)shell->lexer->tokens.elem;
	i = -1;
	while (++i < shell->lexer->tokens.len)
	{
		j = 0;
		while (tokens[i]->str[j])
		{
			if (ft_is_char_in_str(tokens[i]->str[j], "\'\""))
			{
				tokens[i]->str[j] = 0;
				temp = ft_strjoin(tokens[i]->str, tokens[i]->str + j + 1);
				free(tokens[i]->str);
				tokens[i]->str = temp;
			}
			else
				++j;
		}
	}
	return (0);
}

int					main(void)
{
	char		*command;
	extern struct termios g_tty;
	t_shell		shell; 
	preparation(&shell);
	tcgetattr(STDIN_FILENO, &g_tty);
	while (1)
	{
		change_termios(&shell.initfd, 0);
		ft_bzero(shell.lexer, sizeof(t_lexer));
		ft_printf("$> ");
		if (!(command = input_command(&shell.lineeditor,
						&shell.history, '$')) && write(1, "\n", 1))
			continue ;
		if (lexer_creating(command, &shell))
			continue ;
		if (tilde_expansion(&shell))
			continue ;
		if (env_expansion(&shell))
			continue ;
		if (quote_removing(&shell))
			continue ;
		write(1, "\n", 1);
		if (!shell.lexer->tokens.len)
		{
			free_lexer(shell.lexer);
			free(shell.history.commands[shell.history.last]);
			shell.history.commands[shell.history.last] = 0;
			continue ;
		}
		if (!(shell.ast = create_separator_ast(shell.lexer, 0, shell.lexer->tokens.len - 1, 0, &shell)))
		{
			--shell.history.last;
			free(shell.history.commands[shell.history.last]);
			shell.history.commands[shell.history.last] = 0;
		}
//		print_ast(shell.ast);
		change_termios(&shell.initfd, 1);
		parse_ast(shell.ast, &shell, 1);
		free_lexer(shell.lexer);
		free_ast(shell.ast);
//		system("leaks -quiet 21sh");
	}
	free_double_arr(shell.env);
	free(shell.lexer);
	//TODO: close shell->initfd
	return (0);
}
