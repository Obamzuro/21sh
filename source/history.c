/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:47:11 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/25 20:19:45 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void				history_append(char *command,
		t_history *history, int appendmode)
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
}
