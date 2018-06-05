/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:06:10 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/23 19:07:43 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MINISHELL_H
# define __MINISHELL_H
# define AM_COMMANDS 7
# include <unistd.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include "libft.h"
# include "ft_printf.h"

typedef struct			s_comm_corr
{
	char	*comm;
	void	(*func)(char **, char ***);
}						t_comm_corr;

volatile sig_atomic_t	g_sigint;

char					*msh_strjoin_char(const char *s1,
		const char *s2, char c);
char					*msh_strjoin_path(const char *s1, const char *s2);

char					**fill_env(void);
void					fill_commands(t_comm_corr *commands);
void					replace_env_variable(char **args, char **env);

void					change_dir(char **args, char ***env);
void					ft_echo(char **args, char ***env);
void					ft_exec(char **args, char ***env);
void					ft_exit(char **args, char ***env);
void					print_env(char **args, char ***env);
void					print_pwd(char **args, char ***env);
void					set_env_comm(char **args, char ***env);
void					unset_env(char **args, char ***env);
void					set_env(char *key, char *value, char ***env);

char					*get_env(char *key, char **env);
void					int_handler(int sig);

void					free_double_arr(char **args);
#endif
