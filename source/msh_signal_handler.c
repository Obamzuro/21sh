/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_signal_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:53:06 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/19 13:53:07 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		int_handler(int sig)
{
	if (sig == SIGINT)
	{
		signal(SIGINT, int_handler);
		if (g_sigint == 0)
			write(1, "\n$> ", 4);
		else
			write(1, "\n", 1);
	}
}
