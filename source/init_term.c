/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_term.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/09 14:25:20 by obamzuro          #+#    #+#             */
/*   Updated: 2018/07/05 15:17:10 by obamzuro         ###   ########.fr       */
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

void	set_noncanon(void)
{
	struct termios		tty;

	if (!isatty(0))
	{
		ft_fprintf(2, "21sh: stdin isn't terminal\n");
		exit(EXIT_FAILURE);
	}
	tcgetattr(1, &tty);
	tty.c_lflag &= ~(ICANON | ECHO);
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	tcsetattr(1, TCSANOW, &tty);
}
