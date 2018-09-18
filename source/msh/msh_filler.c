/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_filler.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:42 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/17 15:44:51 by obamzuro         ###   ########.fr       */
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
	{LEFT, line_editing_left},
	{RIGHT, line_editing_right},
	{UP, line_editing_up},
	{DOWN, line_editing_down},
	{ALTLEFT, line_editing_begword},
	{ALTRIGHT, line_editing_endword},
	{ALTUP, line_editing_altup},
	{ALTDOWN, line_editing_altdown},
	{BACKSPACE, line_editing_backspace},
	{HOME, line_editing_home},
	{END, line_editing_end},
	{SHIFTLEFT, line_editing_shiftleft},
	{SHIFTRIGHT, line_editing_shiftright},
	{ALTC, line_editing_altc},
	{ALTV, line_editing_altv},
	{ALTX, line_editing_altx},
	{ALTQ, line_editing_help},
	{DEL, line_editing_del},
	{EOT, line_editing_eot}
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
