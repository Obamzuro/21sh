/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_filler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:42 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/09 15:59:42 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		fill_commands(t_comm_corr *commands)
{
	commands[0].comm = "pwd";
	commands[0].func = print_pwd;
	commands[1].comm = "cd";
	commands[1].func = change_dir;
	commands[2].comm = "env";
	commands[2].func = print_env;
	commands[3].comm = "setenv";
	commands[3].func = set_env_comm;
	commands[4].comm = "unsetenv";
	commands[4].func = unset_env;
	commands[5].comm = "echo";
	commands[5].func = ft_echo;
	commands[6].comm = "exit";
	commands[6].func = ft_exit;
}

char		**init_operators(void)
{
	char **operators;

	operators = (char **)malloc(sizeof(char *) * AM_OPERATORS);
	operators[0] = ">";
	operators[1] = ">&";
	operators[2] = ">>";
	operators[3] = "<";
	operators[4] = "<&";
	operators[5] = "<<";
	operators[6] = "|";
	return (operators);
}

char		**fill_env(void)
{
	extern char		**environ;
	char			**env;
	int				count;
	int				i;

	count = 0;
	while (environ[count])
		++count;
	env = (char **)malloc(sizeof(char *) * (count + 1));
	i = -1;
	while (++i < count)
		env[i] = ft_strdup(environ[i]);
	env[i] = 0;
	return (env);
}
