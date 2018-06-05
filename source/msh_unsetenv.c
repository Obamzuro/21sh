/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_unsetenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:53:18 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/23 15:06:29 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void			unset_env_cpenv(char **args, char ***env, char **newenv)
{
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	while ((*env)[i])
	{
		if (!flag && ft_strcmp((*env)[i], args[1]) == '=')
		{
			free((*env)[i]);
			++i;
			flag = 1;
			continue ;
		}
		newenv[i - flag] = ft_strdup((*env)[i]);
		free((*env)[i]);
		++i;
	}
	newenv[i - flag] = 0;
	free(*env);
	*env = newenv;
}

void				unset_env(char **args, char ***env)
{
	char	**newenv;
	int		i;

	if (!get_env(args[1], *env))
		return ;
	i = 0;
	while ((*env)[i])
		++i;
	newenv = (char **)malloc(sizeof(char *) * (i));
	unset_env_cpenv(args, env, newenv);
}
