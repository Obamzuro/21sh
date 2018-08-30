/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_filler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:42 by obamzuro          #+#    #+#             */
/*   Updated: 2018/08/30 15:45:33 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

t_comm_corr g_commands[] =
{
	{"pwd", print_pwd},
	{"cd", change_dir},
	{"env", print_env},
	{"setenv", set_env_comm},
	{"unsetenv", unset_env},
	{"echo", ft_echo},
	{"exit", ft_exit}
};

char *(operators[]) =
{
	">", ">&", ">>", "<", "<&", "<<", "|", ";"
};

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
