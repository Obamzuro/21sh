/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_replace_env_variable.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 12:24:29 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/28 12:49:41 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int			replace_env_variable_repl_end(char **args,
		char **env, int i, int *j)
{
	char	*temp;

	(void)env;
	temp = args[i];
	args[i] = ft_strjoin(temp, get_env(args[i] + *j + 1, env));
	free(temp);
	return (1);
}

static void			replace_env_variable_repl_middle(char **str,
		int *j, char *foundstable, char **env)
{
	char	*temp;
	char	*temp2;

	temp = ft_strsub(*str, *j + 1,
			foundstable - *str - *j - 1);
	temp2 = get_env(temp, env);
	*j += ft_strlen(temp2);
	free(temp);
	temp = *str;
	*str = ft_strjoin(temp, temp2);
	temp2 = ft_strdup(foundstable);
	free(temp);
	temp = *str;
	*str = ft_strjoin(temp, temp2);
	free(temp);
	free(temp2);
}

static int			replace_env_variable_repl(char **args, char **env,
		int i, int *j)
{
	char	*foundstable;

	args[i][*j] = 0;
	foundstable = ft_strchr_str(args[i] + *j + 1, "$\'\" \t\n");
	if (!foundstable && replace_env_variable_repl_end(args, env, i, j))
		return (1);
	else
		replace_env_variable_repl_middle(&args[i], j, foundstable, env);
	return (0);
}

void				env_expansion_kernel(t_shell *shell,
		int i, char **args)
{
	int		squotemode;
	int		dquotemode;
	int		j;

	j = 0;
	squotemode = 0;
	dquotemode = 0;
	while (args[i][j])
		if (args[i][j] == '\"')
			++j && (dquotemode = !dquotemode);
		else if (args[i][j] == '\'' && !dquotemode)
			++j && (squotemode = !squotemode);
		else if (!squotemode && args[i][j] == '$')
		{
			if (ft_is_char_in_str(args[i][j + 1], "$\'\" \t\n")
						|| !args[i][j + 1])
			{
				++j;
				continue ;
			}
			else if (replace_env_variable_repl(args, shell->env, i, &j))
				break ;
		}
		else
			++j;
}

int					env_expansion(t_shell *shell, char **args)
{
	int		i;

	i = -1;
	while (args[++i])
		env_expansion_kernel(shell, i, args);
	return (0);
}
