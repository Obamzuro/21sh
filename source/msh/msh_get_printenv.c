/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_get_printenv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:51 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/21 20:51:34 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

char		*get_env(char *key, char **env)
{
	int		i;
	char	*envvalue;

	i = 0;
	while (env[i])
	{
		if (!(envvalue = ft_strchr(env[i], '=')))
			continue ;
		if (ft_strcmp(env[i], key) == '=')
			return (envvalue + 1);
		++i;
	}
	return (0);
}

void		print_env(char **args, char ***env)
{
	int		i;

	args++;
	i = 0;
	while ((*env)[i])
		ft_printf("%s\n", (*env)[i++]);
	return ;
}
