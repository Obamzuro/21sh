/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_filler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:42 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/23 18:30:11 by obamzuro         ###   ########.fr       */
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
	{LEFT, line_editing_left, 0, 0},
	{RIGHT, line_editing_right, 0, 0},
	{UP, line_editing_up, 1, 0},
	{DOWN, line_editing_down, 1, 0},
	{ALTLEFT, line_editing_begword, 0, 0},
	{ALTRIGHT, line_editing_endword, 0, 0},
	{ALTUP, line_editing_altup, 0, 0},
	{ALTDOWN, line_editing_altdown, 0, 0},
	{BACKSPACE, line_editing_backspace, 1, 0},
	{HOME, line_editing_home, 0, 0},
	{END, line_editing_end, 0, 0},
	{SHIFTLEFT, line_editing_shiftleft, 1, 1},
	{SHIFTRIGHT, line_editing_shiftright, 1, 1},
	{ALTC, line_editing_altc, 0, 1},
	{ALTV, line_editing_altv, 1, 0},
	{ALTX, line_editing_altx, 1, 1},
	{ALTQ, line_editing_help, 0, 0},
	{DEL, line_editing_del, 1, 0},
	{EOT, line_editing_eot, 1, 0}
};

char		g_ascii_ignore_chars[AM_ASCII_IGNORE + 1] =
{
	EOT[0], '\n', 0
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
