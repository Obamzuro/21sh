/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_replace_env_variable.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 12:24:29 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/23 19:12:02 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			replace_env_variable_repl_end(char **args,
		char **env, int i, int *j)
{
	char	*temp;

	temp = args[i];
	args[i] = ft_strjoin(temp, get_env(args[i] + *j + 1, env));
	free(temp);
	return (1);
}

static int			replace_env_variable_repl(char **args, char **env,
		int i, int *j)
{
	char	*temp;
	char	*temp2;
	char	*foundstable;

	temp = 0;
	args[i][*j] = 0;
	foundstable = ft_strchr(args[i] + *j + 1, '$');
	if (!foundstable && replace_env_variable_repl_end(args, env, i, j))
		return (1);
	else
	{
		temp = ft_strsub(args[i], *j + 1, foundstable - args[i] - *j - 1);
		temp2 = get_env(temp, env);
		*j += ft_strlen(temp2);
		free(temp);
		temp = args[i];
		args[i] = ft_strjoin(temp, temp2);
		temp2 = ft_strdup(foundstable);
		free(temp);
		temp = args[i];
		args[i] = ft_strjoin(temp, temp2);
		free(temp);
		free(temp2);
	}
	return (0);
}

void				replace_env_variable(char **args, char **env)
{
	int		i;
	int		j;

	i = 0;
	while (args[++i])
	{
		j = 0;
		while (args[i][j])
		{
			if (args[i][j] == '$')
			{
				if (args[i][j + 1] == '$' || !args[i][j + 1])
				{
					++j;
					continue ;
				}
				else if (replace_env_variable_repl(args, env, i, &j))
					break ;
			}
			else
				++j;
		}
	}
	return ;
}
