/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2018/08/29 00:31:54 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

static int			handle_commands(char **args,
		char ***env)
{
	int		i;

//	if (!(args = ft_strsplit2(line, " \t"))[0])
//	{
//		free_double_arr(args);
//		return ;
//	}
//	replace_env_variable(args, *env);
	extern t_comm_corr g_commands[AM_COMMANDS];
	i = -1;
	while (++i < AM_COMMANDS)
	{
		if (!ft_strncmp(g_commands[i].comm, args[0],
					ft_strlen(g_commands[i].comm) + 1))
		{
			g_commands[i].func(args, env);
			break ;
		}
	}
	if (i == AM_COMMANDS)
		return (0);
	return (1);
}

char				*input_command(void)
{
	char		buf[8];
	char		*command;
	char		*temp;

	command = 0;
	while (1)
	{
		ft_bzero(buf, sizeof(buf));
		read(0, buf, sizeof(buf));
		if (!buf[1] && buf[0] == '\n')
		{
			write(1, buf, sizeof(buf));
			break ;
		}
		temp = command;
		command = ft_strjoin(command, buf);
		free(temp);
		write(1, buf, sizeof(buf));
	}
	return (command);
}

int					lexing_try_append_operator(char buf, char **tokenstr)
{
	int			i;
	char		*joining;
	int			tokenlen;
	extern char	*operators[AM_OPERATORS];

	if (!buf)
		return (0);
	tokenlen = ft_strlen(*tokenstr);
	joining = ft_chrjoin(*tokenstr, buf);
	i = -1;
	while (++i < AM_OPERATORS)
	{
		if (ft_strnstr(operators[i], joining, tokenlen + 1))
		{
			free(*tokenstr);
			*tokenstr = joining;
			return (1);
		}
	}
	free(joining);
	return (0);
}

t_token				*lexing_divide_operator(t_lexer *lexer, t_token *token)
{
	if (lexer->tokens.len > 0 &&
		((t_token *)lexer->tokens.elem[lexer->tokens.len - 1])->type == UKNOWN)
	{
		if (ft_isnumber(((t_token *)lexer->tokens.elem[lexer->tokens.len - 1])->str) &&
				ft_is_str_in_args(token->str, 5, ">", ">&", ">>", "<", "<&"))
			((t_token *)lexer->tokens.elem[lexer->tokens.len - 1])->type = IO_NUMBER;
		else
			((t_token *)lexer->tokens.elem[lexer->tokens.len - 1])->type = WORD;
	}
	push_ftvector(&lexer->tokens, token);
	token = 0;
	return (token);
}

int					lexing_is_operator_begin(char buf)
{
	int			i;
	extern char	*operators[AM_OPERATORS];

	i = -1;
	if (!buf)
		return (0);
	while (++i < AM_OPERATORS)
		if (buf == operators[i][0])
			return (1);
	return (0);
}

t_token				*lexing_init_operator_token(char buf, t_token *token, t_lexer *lexer)
{
	if (token && token->str)
		push_ftvector(&lexer->tokens, token);
	token = (t_token *)malloc(sizeof(t_token));
	token->str = (char *)ft_memalloc(2);
	token->str[0] = buf;
	token->type = OPERATOR;
	return (token);
}

t_token				*lexing_handling_quotes(t_lexer *lexer, t_token *token, char **last,
		char **command)
{
	char	delim;
	char	*temp;

	delim = **last;
	while (1)
	{
		++(*last);
		if (**last == delim)
			break ;
		if (!token)
			token = (t_token *)ft_memalloc(sizeof(t_token));
		if (!**last)
		{
			temp = ft_chrjoin(token->str, '\n');
			free(token->str);
			token->str = temp;
			free(*command);
			*command = input_command();
			*last = *command - 1;
			continue ;
		}
		temp = ft_chrjoin(token->str, **last);
		free(token->str);
		token->str = temp;
	}
	return (token);
}

int					lexing_handling_end(t_lexer *lexer, t_token *token)
{
	if (token && token->str)
	{
		token->type = WORD;
		push_ftvector(&lexer->tokens, token);
	}
	return (1);
}

t_token				*lexing_handling_separator(t_lexer *lexer, t_token *token)
{
	if (token && token->str)
	{
		if (token->type == UKNOWN)
			token->type = WORD;
		push_ftvector(&lexer->tokens, token);
	}
	token = NULL;
	return (token);
}

void				lexing_handling_appword(t_token *token, char buf)
{
	char	*temp;

	temp = ft_chrjoin(token->str, buf);
	free(token->str);
	token->str = temp;
}

t_token				*lexing_handling_initword(t_token *token, char buf)
{
	token = (t_token *)malloc(sizeof(t_token));
	token->str = (char *)ft_memalloc(2);
	token->str[0] = buf;
	token->type = UKNOWN;
	return (token);
}

void				lexing_print(t_lexer *lexer)
{
	int i;

	i = 0;
	ft_printf("\n");
	while (i < lexer->tokens.len)
	{
		ft_printf("%s | %d\n", ((t_token *)lexer->tokens.elem[i])->str, ((t_token *)lexer->tokens.elem[i])->type);
		++i;
	}
	ft_printf("\n");
}

void				tilde_expansion(t_ftvector *line)
{
	int				i;
	char			*slash;
	char			*temp;
	char			*temp2;
	struct stat		mystat;

	i = -1;
	while (line->elem[++i])
	{
		if (((char *)(line->elem[i]))[0] == '~')
		{
			if (((char *)(line->elem[i]))[1] == '/')
			{
				temp = ft_strjoin(getenv("HOME"), line->elem[i] + 1);
				free(line->elem[i]);
				line->elem[i] = temp;
			}
			else
			{
				slash = ft_strchr(line->elem[i] + 1, '/');
				if (slash)
				{
					*slash = 0;
					temp = ft_strjoin("/Users/", line->elem[i] + 1);
					if (lstat(temp, &mystat) != -1)
					{
						temp2 = ft_strjoin_path(temp, slash + 1);
						free(temp);
						free(line->elem[i]);
						line->elem[i] = temp2;
					}
					else
					{
						ft_fprintf(2, "21sh: no such user or named directory: %s", line->elem[i] + 1);
						*slash = '/';
					}
				}
				else
				{
					temp = ft_strjoin("/Users/", line->elem[i] + 1);
					if (lstat(temp, &mystat) != -1)
					{
						line->elem[i] = temp;
						free(line->elem[i]);
					}
					else
						ft_fprintf(2, "21sh: no such user or named directory: %s", line->elem[i] + 1);
				}
			}
		}
	}
}

void				lexer_creating_cycle(char **command, t_lexer *lexer,
		t_token *token, char *last)
{
	while (1)
	{
		++last;
		if (token && token->type == OPERATOR)
		{
			if (lexing_try_append_operator(*last, &token->str))
				continue ;
			token = lexing_divide_operator(lexer, token);
		}
		if (lexing_is_operator_begin(*last))
		{
			token = lexing_init_operator_token(*last, token, lexer);
			continue ;
		}
		else if ('\"' == *last || '\'' == *last)
			token = lexing_handling_quotes(lexer, token, &last, command);
		else if (!*last && lexing_handling_end(lexer, token))
			break ;
		else if (ft_strchr(" \t", *last))
			token = lexing_handling_separator(lexer, token);
		else if (token && token->type == UKNOWN)
			lexing_handling_appword(token, *last);
		else
			token = lexing_handling_initword(token, *last);
	}
}

void				lexer_creating(char *command, t_lexer *lexer)
{
	t_token		*token;
	char		*last;

	token = NULL;
	last = command - 1;
	init_ftvector(&lexer->tokens);
	lexer_creating_cycle(&command, lexer, token, last);
	lexing_print(lexer);
	free(command);
}

void				free_lexer(t_lexer *lexer)
{
	int		i;

	i = 0;
	while (i < lexer->tokens.len)
	{
		free(((t_token *)lexer->tokens.elem[i])->str);
		free(lexer->tokens.elem[i]);
		++i;
	}
	free(lexer->tokens.elem);
}

char				*(separator_op[AM_LEVELS][AM_SEPARATOROP]) =
{
	{ ";", NULL },
	{ "|", NULL }
};

char				*(io_file_op[AM_IOFILEOP]) =
{
	"<", "<&", ">", ">&", ">>", NULL
};

int					first_token_pos(t_lexer *lexer, int beg, int end,
		char **findname)
{
	int		i;
	int		j;

	i = beg - 1;
	while (++i <= end)
	{
		j = -1;
		while (findname[++j])
			if (ft_strequ(((t_token *)lexer->tokens.elem[i])->str, findname[j]) &&
					((t_token *)lexer->tokens.elem[i])->type == OPERATOR)
				return (i);
	}
	return (-1);
}

int					last_token_pos(t_lexer *lexer, int beg, int end,
		char **findname)
{
	int		i;
	int		j;

	i = end + 1;
	while (--i >= beg)
	{
		j = -1;
		while (findname[++j])
			if (ft_strequ(((t_token *)lexer->tokens.elem[i])->str, findname[j]) &&
					((t_token *)lexer->tokens.elem[i])->type == OPERATOR)
				return (i);
	}
	return (-1);
}

t_ast				*create_command(t_lexer *lexer, int beg, int end)
{
	t_ast	*ast;
	char	*str;
	char	*temp;

	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	str = 0;
	while (beg <= end)
	{
		if (((t_token *)lexer->tokens.elem[beg])->type == WORD)
		{
			temp = ft_strjoin_char(str, ((t_token *)lexer->tokens.elem[beg])->str, ' ');
			free(str);
			str = temp;
		}
		++beg;
	}
	ast->content = (void *)str;
	ast->type = COMMAND;
	return (ast);
}

t_ast				*create_redirection_ast(t_lexer *lexer, int beg, int end)
{
	t_ast		*ast;
	int			i;
	char		*str;
	int			pos;
	char		*temp;

	if ((pos = first_token_pos(lexer, beg, end, io_file_op)) == -1)
		return (create_command(lexer, beg, end));
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = (void *)ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = REDIRECTION;
	((t_token *)lexer->tokens.elem[pos])->type = USED;
	if (lexer->tokens.len <= pos + 1 || pos + 1 > end)
	{
		ft_fprintf(2, "21sh: parse error - redirection word missed\n");
		return (0);
	}

	ast->right = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->right->content = (t_binary_token *)ft_memalloc(sizeof(t_binary_token));

	// try to catch previous token
	if (pos >= 1 && ((t_token *)lexer->tokens.elem[pos - 1])->type == IO_NUMBER)
	{
		((t_binary_token *)ast->right->content)->left = ((t_token *)lexer->tokens.elem[pos - 1])->str;
		((t_token *)lexer->tokens.elem[pos - 1])->type = USED;
	}
	((t_binary_token *)ast->right->content)->right = ((t_token *)lexer->tokens.elem[pos + 1])->str;
	ast->right->type = USED;
	((t_token *)lexer->tokens.elem[pos + 1])->type = USED;
	if (!ast->left && !(ast->left = create_redirection_ast(lexer, beg, end)))
		return (0);
	return (ast);
}

t_ast				*create_separator_ast(t_lexer *lexer, int beg, int end, int level);

t_ast				*create_separator_ast_notfound(t_lexer *lexer,
		int beg, int end, int level)
{
	if (level == 1)
		return (create_redirection_ast(lexer, beg, end));
	else
		return (create_separator_ast(lexer, beg, end, level + 1));
}

t_ast				*create_separator_ast(t_lexer *lexer, int beg, int end, int level)
{
	int		pos;
	t_ast	*ast;

	if ((pos = last_token_pos(lexer, beg, end, separator_op[level])) == -1)
		return (create_separator_ast_notfound(lexer, beg, end, level));
	if (lexer->tokens.len <= pos + 1 || !pos)
	{
		ft_fprintf(2, "21sh: parse operator error - incorrect position\n");
		return (0);
	}
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = (void *)ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = OPERATOR;
	if (!(ast->left = create_separator_ast(lexer, beg, pos - 1, level)))
		return (0);
	if (level != 1)
	{
		if (!(ast->right = create_separator_ast(lexer, pos + 1, end, level + 1)))
			return (0);
	}
	else if (!(ast->right = create_redirection_ast(lexer, pos + 1, end)))
		return (0);
	return (ast);
}

void				print_ast(t_ast *ast)
{
	int		i;

	i = 0;
	while (1)
	{
		if (!ast)
			break ;
		ft_printf("AST #%d - %s %d\n", i, ast->content, ast->type);
		if (ast->right)
		{
			if (ast->type == REDIRECTION)
				ft_printf("AST #%d - |%s  %s| %d\n", i, ((t_binary_token *)ast->right->content)->left, ((t_binary_token *)ast->right->content)->right, ast->right->type);
			else
				ft_printf("AST #%d - %s %d\n", i, ast->right->content, ast->right->type);
		}
		else
			ft_printf("AST #%d right - NULL\n", i);
		ast = ast->left;
		++i;
	}
}

int						parse_ast(t_ast *ast, char ***env, int ispipe, t_initfd *initfd, int isfork)
{
	int			fd;
	int			newfd;
	int			oldfd[2];
	int			fdin;
	int			fdout;
	int			fdpipe[2];
	char		**args;
	int			process;
	pid_t		pid[2];

	if (!ast)
		return (0);
	if (ast->type == OPERATOR && ((char *)ast->content)[0] == ';' && !((char *)ast->content)[1])
	{
		parse_ast(ast->left, env, 0, initfd, 0);
		parse_ast(ast->right, env, 0, initfd, 0);
		return (process);
	}
	else if (ast->type == OPERATOR && ((char *)ast->content)[0] == '|' && !((char *)ast->content)[1])
	{
		pipe(fdpipe);
		pid[0] = fork();
		change_termios(initfd, 1);
		if (pid[0] < 0)
		{
			ft_fprintf(2, "21sh: Error creating subprocess");
			exit(1);
		}
		if (!pid[0])
		{
			close(fdpipe[0]);
			dup2(fdpipe[1], 1);
			close(fdpipe[1]);
			parse_ast(ast->left, env, 1, initfd, 0);
			exit(0);
		}
		pid[1] = fork();
		if (pid[1] < 0)
		{
			ft_fprintf(2, "21sh: Error creating subprocess");
			exit(1);
		}
		if (!pid[1])
		{
			close(fdpipe[1]);
			dup2(fdpipe[0], 0);
			close(fdpipe[0]);
			parse_ast(ast->right, env, 1, initfd, 0);
			exit(0);
		}
		close(fdpipe[0]);
		close(fdpipe[1]);
		waitpid(pid[0], 0, 0);
		waitpid(pid[1], 0, 0);
		change_termios(initfd, 0);
//		oldfd[0] = dup(0);
//		oldfd[1] = dup(1);
//		pipe(fdpipe);
//		dup2(fdpipe[1], 1);
//		close(fdpipe[1]);
//		parse_ast(ast->left, env, 1);
//		dup2(fdpipe[0], 0);
//		close(fdpipe[0]);
//		dup2(oldfd[1], 1);
//		close(oldfd[1]);
//		process = parse_ast(ast->right, env, 1);
//		if (!ispipe)
//			waitpid(process, 0, 0);
//		dup2(oldfd[0], 0);
//		close(oldfd[0]);
//		return (process);
	}
	else if (ast->type == REDIRECTION)
	{
		isfork = 1;
		if (!fork())
		{
			if (((t_binary_token *)(ast->right->content))->left)
				fd = ft_atoi(((t_binary_token *)(ast->right->content))->left);
			else if (((char *)ast->content)[0] == '>')
			{
				if (!((char *)ast->content)[1])
					fd = 1;
				else if (((char *)ast->content)[1] == '>' && !((char *)ast->content)[2])
					fd = 1;
			}
			else if (((char *)ast->content)[0] == '<' && !((char *)ast->content)[1])
				fd = 0;
			if (((char *)ast->content)[0] == '>')
			{
				if (!((char *)ast->content)[1])
					newfd = open(((t_binary_token *)(ast->right->content))->right, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				else if (((char *)ast->content)[1] == '&' && !((char *)ast->content)[2])
					newfd = ft_atoi(((t_binary_token *)(ast->right->content))->right);
				else if (((char *)ast->content)[1] == '>' && !((char *)ast->content)[2])
					newfd = open(((t_binary_token *)(ast->right->content))->right, O_WRONLY | O_CREAT | O_APPEND, 0644);
			}
			if (((char *)ast->content)[0] == '<')
			{
				if (!((char *)ast->content)[1])
					newfd = open(((t_binary_token *)(ast->right->content))->right, O_RDONLY, 0644);
				else if (((char *)ast->content)[1] == '&' && !((char *)ast->content)[2])
					newfd = ft_atoi(((t_binary_token *)(ast->right->content))->right);
			}
			if (newfd == -1)
			{
				ft_fprintf(2, "sh: %s: No such file or directory\n", ((t_binary_token *)(ast->right->content))->right);
				return (0);
			}
			if (((char *)ast->content)[1] != '&')
			{
				oldfd[0] = dup(fd);
				dup2(newfd, fd);
				close(newfd);
				process = parse_ast(ast->left, env, ispipe, initfd, 1);
				dup2(oldfd[0], fd);
				close(oldfd[0]);
			}
			else
			{
				oldfd[0] = dup(fd);
				if (ft_strequ(((t_binary_token *)(ast->right->content))->right, "-"))
					close(fd);
				else
					dup2(newfd, fd);
				process = parse_ast(ast->left, env, ispipe, initfd, 1);
				dup2(oldfd[0], fd);
				close(oldfd[0]);
			}
		}
		wait(0);
		return (process);
	}
	else if (ast->type == COMMAND)
	{
		args = ft_strsplit2(ast->content, " \t");
		if (!handle_commands(args, env))
		{
			if (!ispipe)
			{
//				change_termios(initfd, 1);
				if (!isfork)
				{
					process = ft_exec(args, env, 1);
					wait(0);
				}
				else
					process = ft_exec(args, env, 0);
//				change_termios(initfd, 0);
			}
			else
				process = ft_exec(args, env, 0);
		}
		free_double_arr(args);
		return (process);
	}
	return (0);
}

void				free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	free(ast->content);
	free(ast);
}

void				preparation(t_initfd *initfd, char ***env)
{
	term_associate();
	initfd->fdin = dup(0);
	initfd->fdout = dup(1);
	*env = fill_env();
	//signal(SIGINT, int_handler);
}

int					main(void)
{
	char		**env;
	char		*command;
	t_lexer		lexer;
	t_ast		*ast;
	t_initfd	initfd;

	preparation(&initfd, &env);
	while (1)
	{
		change_termios(&initfd, 0);
		ft_bzero(&lexer, sizeof(t_lexer));
		ft_printf("$> ");
		if (!(command = input_command()))
			continue ;
		lexer_creating(command, &lexer);
		if (!lexer.tokens.len)
		{
			free_lexer(&lexer);
			continue ;
		}
		ast = create_separator_ast(&lexer, 0, lexer.tokens.len - 1, 0);
		print_ast(ast);
		change_termios(&initfd, 1);
		parse_ast(ast, &env, 0, &initfd, 0);
		free_lexer(&lexer);
		free_ast(ast);
		system("leaks -quiet 21sh");
	}
	free_double_arr(env);
	return (0);
}
