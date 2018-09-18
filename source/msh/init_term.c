/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_term.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/09 14:25:20 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/17 21:32:11 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <21sh.h>

void	term_associate(void)
{
	char	*termtype;
	int		success;

	termtype = getenv("TERM");
	if (!termtype)
	{
		ft_fprintf(2, "21sh: Can't find an env TERM\n");
		exit(EXIT_FAILURE);
	}
	success = tgetent(0, termtype);
	if (success < 0)
	{
		ft_fprintf(2, "21sh: Can't access the termcap data base\n");
		exit(EXIT_FAILURE);
	}
	if (!success)
	{
		ft_fprintf(2, "21sh: Terminal type '%s' is not defined\n", termtype);
		exit(EXIT_FAILURE);
	}
}

struct termios		g_tty;

void	change_termios(t_initfd *initfd, int canon)
{
	struct termios		tty;

	(void)initfd;
	if (!canon)
	{
//		if (!isatty(initfd->fdin))
		if (!isatty(0))
		{
			ft_fprintf(2, "21sh: stdin isn't terminal\n");
			exit(EXIT_FAILURE);
		}
		(initfd->fdin == -1) ? dup2(initfd->fdin, 0) : 0;
		(initfd->fdout == -1) ? dup2(initfd->fdout, 1) : 0;
		(initfd->fderr == -1) ? dup2(initfd->fderr, 2) : 0;
		tcgetattr(0, &tty);
		tty.c_lflag &= ~(ICANON | ECHO);
		tty.c_cc[VMIN] = 1;
		tty.c_cc[VTIME] = 0;
		tcsetattr(0, TCSANOW, &tty);
	}
	else
	{
//		tcgetattr(0, &tty);
//		tty.c_lflag |= (ICANON | ECHO);
		tcsetattr(0, TCSANOW, &g_tty);
	}
}
