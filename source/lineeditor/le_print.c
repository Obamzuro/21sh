/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:06:43 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 14:08:41 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int			ft_putc(int c)
{
	write(1, &c, 1);
	return (0);
}

void		print_buffer(t_lineeditor *lineeditor, t_history *history,
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

void		input_command_print_buffer(t_lineeditor *lineeditor,
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
