/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_replace_env_variable.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 12:24:29 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/18 18:32:16 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int			replace_env_variable_repl_end(t_token **tokens,
		char **env, int i, int *j)
{
	char	*temp;

	(void)env;
	temp = tokens[i]->str;
	tokens[i]->str = ft_strjoin(temp, getenv(tokens[i]->str + *j + 1));
	free(temp);
	return (1);
}

static int			replace_env_variable_repl(t_token **tokens, char **env,
		int i, int *j)
{
	char	*temp;
	char	*temp2;
	char	*foundstable;

	temp = 0;
	tokens[i]->str[*j] = 0;
	foundstable = ft_strchr_str(tokens[i]->str + *j + 1, "$\'\"");
	if (!foundstable && replace_env_variable_repl_end(tokens, env, i, j))
		return (1);
	else
	{
		temp = ft_strsub(tokens[i]->str, *j + 1,
				foundstable - tokens[i]->str - *j - 1);
		temp2 = getenv(temp);
		*j += ft_strlen(temp2);
		free(temp);
		temp = tokens[i]->str;
		tokens[i]->str = ft_strjoin(temp, temp2);
		temp2 = ft_strdup(foundstable);
		free(temp);
		temp = tokens[i]->str;
		tokens[i]->str = ft_strjoin(temp, temp2);
		free(temp);
		free(temp2);
	}
	return (0);
}

int					env_expansion(t_shell *shell)
{
	int		i;
	int		j;
	t_token	**tokens;
	int		squotemode;

	i = -1;
	squotemode = 0;
	tokens = (t_token **)shell->lexer->tokens.elem;
	while (++i < shell->lexer->tokens.len)
	{
		j = 0;
		while (tokens[i]->str[j])
			if (tokens[i]->str[j] == '\'')
			{
				squotemode = !squotemode;
				++j;
			}
			else if (!squotemode && tokens[i]->str[j] == '$')
			{
				if (ft_is_char_in_str(tokens[i]->str[j + 1], "$\'\"")
							|| !tokens[i]->str[j + 1])
				{
					++j;
					continue ;
				}
				else if (replace_env_variable_repl(tokens, shell->env, i, &j))
					break ;
			}
			else
				++j;
	}
	return (0);
}
