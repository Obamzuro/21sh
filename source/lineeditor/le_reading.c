/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_reading.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:06:45 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/25 14:29:28 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

/*
**DANGEROUS
*/

void			get_cursor_position(int cursorpos[2], t_lineeditor *lineeditor)
{
	char	temp[16];

	cursorpos[0] = 0;
	cursorpos[1] = 0;
	write(2, "\x1B[6n", 4);
	while (read(2, temp, 1) && temp[0] != '\x1B')
		;
	read(2, temp + 1, 15);
	cursorpos[0] = ft_atoi(temp + 2);
	cursorpos[1] = ft_atoi(temp + 3 + ft_nbr_size(cursorpos[0]));
	if (cursorpos[1] > lineeditor->ws.ws_row)
		cursorpos[1] = lineeditor->ws.ws_row;
}

/*
**HANDLING WINCH AND CTRL-C
*/

int				le_reading(t_lineeditor *lineeditor, int offset, int size)
{
	extern volatile sig_atomic_t	g_sigwinch;

	if (read(0, lineeditor->letter + offset, size) == -1)
	{
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
			get_cursor_position(lineeditor->curpos, lineeditor);
			--lineeditor->curpos[1];
			--lineeditor->curpos[0];
			g_sigwinch = 0;
			return (0);
		}
		else
			return (-1);
	}
	return (0);
}
