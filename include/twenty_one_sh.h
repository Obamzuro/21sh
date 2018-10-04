/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   twenty_one_sh.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:50:56 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/04 14:36:45 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __TWENTY_ONE_SH_H
# define __TWENTY_ONE_SH_H

# include <curses.h>
# include <term.h>
# include <termcap.h>
# include <unistd.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <errno.h>
# include "libft.h"
# include "ft_printf.h"

# define AM_COMMANDS 7
# define AM_OPERATORS 8
# define AM_LEVELS 2
# define AM_SEPARATOROP 2
# define AM_IOFILEOP 7
# define AM_HISTORY 1000
# define AM_ESC 19
# define AM_ASCII_IGNORE 2
# define READING_SIZE 8

# define ESC "\x1B"
# define DOWN "\x1B[B"
# define UP "\x1B[A"
# define LEFT "\x1B[D"
# define RIGHT "\x1B[C"
# define SPACE " "
# define SUSPEND "\x1A"
# define DEL "\x1B[3~"
# define DEFAULT "\x1B[0m"
# define BACKSPACE "\x7F"
# define ENTER "\x0A"
# define ALTLEFT "\x1b\x1b\x5b\x44"
# define ALTRIGHT "\x1b\x1b\x5b\x43"
# define ALTUP "\x1b\x1b\x5b\x41"
# define ALTDOWN "\x1b\x1b\x5b\x42"
# define HOME "\x1b\x5b\x48"
# define END "\x1b\x5b\x46"
# define SHIFTLEFT "\x1b\x5b\x31\x3b\x32\x44"
# define SHIFTRIGHT "\x1b\x5b\x31\x3b\x32\x43"
# define ALTX "\xe2\x89\x88"
# define ALTC "\xc3\xa7"
# define ALTV "\xe2\x88\x9a"
# define ALTQ "\xc5\x93"
# define EOT "\x4"

typedef enum		e_tokentype
{
	UKNOWN = 0,
	WORD,
	NAME,
	OPERATOR,
	IO_NUMBER,
	COMMAND,
	REDIRECTION,
	USED,
	ERROR
}					t_tokentype;

typedef enum		e_reading_mode
{
	BASIC = 0,
	QUOTE,
	HEREDOC,
	PIPE,
	READEND
}					t_reading_mode;

typedef struct		s_history
{
	char	*commands[AM_HISTORY];
	int		last;
	int		current;
}					t_history;

struct s_shell;

typedef struct		s_lineeditor
{
	char			letter[READING_SIZE];
	int				curpos[2];
	int				seek;
	char			*buffer;
	int				is_history_searched;
	int				selected[2];
	int				selectedmode;
	char			*cpbuf;
	struct winsize	ws;
	char			prompt;
	char			trash[READING_SIZE + 1];
	struct s_shell	*shell;
}					t_lineeditor;

typedef struct		s_initfd
{
	int		fdin;
	int		fdout;
	int		fderr;
}					t_initfd;

typedef struct		s_border
{
	int		beg;
	int		end;
}					t_border;

typedef struct		s_token
{
	t_tokentype		type;
	char			*str;
}					t_token;

typedef struct		s_binary_token
{
	char	*left;
	char	*right;
}					t_binary_token;

typedef struct		s_ast
{
	void				*content;
	t_tokentype			type;
	struct s_ast		*left;
	struct s_ast		*right;
}					t_ast;

typedef struct		s_lexer
{
	t_ftvector	tokens;
}					t_lexer;

typedef struct		s_comm_corr
{
	char	*comm;
	void	(*func)(char **, char ***);
}					t_comm_corr;

typedef struct		s_esc_corr
{
	char	*str;
	void	(*func)(t_lineeditor *, t_history *);
	int		is_printing : 1;
	int		is_selecting : 1;
}					t_esc_corr;

typedef struct		s_shell
{
	char			**env;
	t_initfd		initfd;
	t_history		history;
	t_lexer			*lexer;
	t_lineeditor	lineeditor;
	t_reading_mode	reading_mode;
	t_ast			*ast;
}					t_shell;

char				**fill_env(void);
void				fill_commands(t_comm_corr *commands);

void				change_dir(char **args, char ***env);
void				ft_echo(char **args, char ***env);
int					ft_exec(char **args, char ***env, int forkneed);
void				ft_exit(char **args, char ***env);
void				print_env(char **args, char ***env);
void				print_pwd(char **args, char ***env);
void				set_env_comm(char **args, char ***env);
void				unset_env(char **args, char ***env);
void				set_env(char *key, char *value, char ***env);
char				*get_env(char *key, char **env);
void				int_handler(int sig);

void				free_double_arr(char **args);

void				term_associate(void);
void				change_termios(t_initfd *initfd, int canon);
char				**init_operators(void);

void				history_append(char *command, t_history *history,
		int appendmode);

void				line_editing_left(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_right(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_up(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_down(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_altup(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_altdown(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_begword(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_endword(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_backspace(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_home(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_end(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_shiftleft(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_shiftright(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_altc(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_altv(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_altx(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_help(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_del(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_eot(t_lineeditor *lineeditor,
		t_history *history);

void				*print_error_zero(char *str);

int					tilde_expansion(t_shell *shell, char **args);
int					env_expansion(t_shell *shell, char **args);
int					quote_removing(t_shell *shell, char **args);

int					free_ast(t_ast *ast);
int					parse_ast(t_ast *ast, t_shell *shell, int needfork);
int					parse_ast_redirection(t_ast *ast, t_shell *shell,
		int needfork);
int					parse_ast_pipe(t_ast *ast, t_shell *shell);

void				free_lexer(t_lexer *lexer);
int					lexer_creating(char *command, t_shell *shell);
t_token				*lexing_handling_initword(t_token *token, char buf);
int					lexing_handling_appword(t_token *token, char buf);
t_token				*lexing_handling_separator(t_lexer *lexer, t_token *token);
int					lexing_handling_end(t_shell *shell, t_token *token,
		char **last, char **command);
t_token				*lexing_handling_quotes(t_shell *shell,
		t_token *token, char **last, char **command);
int					lexing_is_operator_begin(char buf);

t_token				*lexing_init_operator_token(char buf,
		t_token *token, t_lexer *lexer);
int					lexing_try_append_operator(char buf, char **tokenstr);
t_token				*lexing_divide_operator(t_lexer *lexer, t_token *token);

int					first_token_pos(t_lexer *lexer, int beg, int end,
		char **findname);
int					last_token_pos(t_lexer *lexer, int beg, int end,
		char **findname);
t_ast				*create_separator_ast(int beg, int end,
		t_shell *shell);
t_ast				*create_redirection_ast(t_lexer *lexer, int beg,
		int end, t_shell *shell);
t_ast				*create_command(t_lexer *lexer, int beg, int end);

char				*input_command(t_lineeditor *lineeditor, t_history *history,
		char prompt, t_shell *shell);
void				free_lineeditor(t_lineeditor *lineeditor);
char				*input_command(t_lineeditor *lineeditor,
		t_history *history, char prompt, t_shell *shell);
void				get_cursor_position(int cursorpos[2],
		t_lineeditor *lineeditor);
int					le_reading(t_lineeditor *lineeditor, int offset, int size);
int					le_ignore_special_chars(t_lineeditor *lineeditor);
int					le_line_editing(t_lineeditor *lineeditor,
		t_history *history);
void				le_zero_hist_research(t_lineeditor *lineeditor,
		t_history *history);
void				le_unselect(t_lineeditor *lineeditor,
		t_history *history, t_esc_corr *esc);
void				left_shift_cursor(int amount, t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_left_notmove(t_lineeditor *lineeditor,
		t_history *history);
void				input_command_print_buffer(t_lineeditor *lineeditor,
		t_history *history);
void				print_buffer(t_lineeditor *lineeditor, t_history *history,
		char *newbuf, int offset);
int					ft_putc(int c);
void				write_line(t_lineeditor *lineeditor);
#endif
