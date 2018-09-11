/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_filler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:42 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/11 12:34:07 by obamzuro         ###   ########.fr       */
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

t_esc_corr g_esc[] =
{
	{LEFT, line_editing_left, 1},
	{RIGHT, line_editing_right, 1},
	{UP, line_editing_up, 1},
	{DOWN, line_editing_down, 1},
	{ALTLEFT, line_editing_begword, 1},
	{ALTRIGHT, line_editing_endword, 1},
	{ALTUP, line_editing_altup, 1},
	{ALTDOWN, line_editing_altdown, 1},
	{BACKSPACE, line_editing_backspace, 1},
	{ALTQ, line_editing_help, 1}
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
