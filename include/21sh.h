/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   21sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/04 11:35:30 by obamzuro          #+#    #+#             */
/*   Updated: 2018/08/14 20:03:39 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __21SH_H
# define __21SH_H
# define AM_COMMANDS 7
# define AM_OPERATORS 8
# define AM_LEVELS 2
# define AM_SEPARATOROP 2
# define AM_IOFILEOP 7
# include <unistd.h>
# include <signal.h>
# include <term.h>
# include <sys/stat.h>
# include <sys/types.h>
# include "libft.h"
# include "ft_printf.h"

typedef enum			e_tokentype
{
	UKNOWN,
	WORD,
	ASSIGNMENT_WORD,
	NAME,
	OPERATOR,
	IO_NUMBER,
	NEWLINE,
	COMMAND,
	REDIRECTION,
	USED
}						t_tokentype;

typedef struct			s_token
{
	t_tokentype		type;
	char			*str;
}						t_token;

typedef struct			s_binary_token
{
	char	*left;
	char	*right;
}						t_binary_token;

typedef struct			s_ast
{
	void				*content;
	t_tokentype			type;
	struct s_ast		*left;
	struct s_ast		*right;
}						t_ast;

typedef struct			s_lexer
{
	t_ftvector	tokens;
}						t_lexer;

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

void					term_associate(void);
void					set_noncanon(void);
char					**init_operators(void);
#endif
