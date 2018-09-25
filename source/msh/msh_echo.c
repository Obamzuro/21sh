/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:29 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/18 18:17:12 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void			ft_echo(char **args, char ***env)
{
	int		i;
	int		first;

	i = 1;
	first = 0;
	++env;
	if (ft_strequ(args[1], "-n"))
		++i;
	while (args[i])
	{
		if (!first)
		{
			ft_printf("%s", args[i]);
			first = 1;
		}
		else
			ft_printf(" %s", args[i]);
		++i;
	}
	if (!ft_strequ(args[1], "-n"))
		ft_printf("\n");
	return ;
}
