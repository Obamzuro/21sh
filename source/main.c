/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/28 13:36:21 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void			handle_commands(char *line,
		t_comm_corr commands[AM_COMMANDS],
		char ***env)
{
	int		i;
	char	**args;

	if (!(args = ft_strsplit2(line, " \t"))[0])
	{
		free_double_arr(args);
		return ;
	}
	replace_env_variable(args, *env);
	i = -1;
	while (++i < AM_COMMANDS)
	{
		if (!ft_strncmp(commands[i].comm, args[0],
					ft_strlen(commands[i].comm) + 1))
		{
			commands[i].func(args, env);
			break ;
		}
	}
	if (i == AM_COMMANDS)
		ft_exec(args, env);
	free_double_arr(args);
}

int					main(void)
{
	char		*line;
	int			i;
	char		**env;
	t_comm_corr	commands[AM_COMMANDS];
	char		**args;

	g_sigint = 0;
	signal(SIGINT, int_handler);
	fill_commands(commands);
	env = fill_env();
	while (1)
	{
		ft_printf("$> ");
		g_sigint = 0;
		get_next_line(0, &line);
		g_sigint = 1;
		args = ft_strsplit(line, ';');
		i = -1;
		while (args[++i])
			handle_commands(args[i], commands, &env);
		free_double_arr(args);
		free(line);
	}
	free_double_arr(env);
	return (0);
}
