/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_signal_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:53:06 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/19 12:44:47 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

volatile sig_atomic_t	g_sigwinch = 0;

void		int_handler(int sig)
{
	(void)sig;

	if (sig == SIGWINCH)
	{
		g_sigwinch = 1;
	}
}
