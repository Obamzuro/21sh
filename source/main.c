/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2018/08/31 22:10:30 by obamzuro         ###   ########.fr       */
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

int					free_ast(t_ast *ast)
{
	if (!ast)
		return (1);
	if (ast->type == REDIRECTION && ft_strequ((char *)ast->content, "<<"))
		free(((t_binary_token *)(ast->right->content))->right);
	free_ast(ast->left);
	free_ast(ast->right);
	free(ast->content);
	free(ast);
	return (1);
}

void				line_editing(char letter[8], char *buffer)
{
	if (!ft_strcmp(letter, LEFT))
	{
		tgoto(tgetstr("cm", 0);
	}
	return (0);
}

char				*input_command(void)
{
	char		letter[8];
	char		*command;
	char		*temp;

	command = 0;
	while (1)
	{
		ft_bzero(letter, sizeof(letter));
		read(0, letter, sizeof(letter));
		if (line_editing(letter, command))
			continue ;
		if (!letter[1] && letter[0] == '\n')
		{
			write(1, letter, sizeof(letter));
			break ;
		}
		temp = command;
		command = ft_strjoin(command, letter);
		free(temp);
		write(1, letter, sizeof(letter));
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
	"<", "<&", "<<", ">", ">&", ">>", NULL
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

void				create_redirection_ast_content_heredoc(t_ast *ast)
{
	char	*line;
	char	*end;
	char	*str;
	char	*temp;

	end = ((t_binary_token *)ast->right->content)->right;
	str = 0;
	ft_printf("> ");
	while ((line = input_command()))
	{
		if (ft_strequ(line, end))
			break ;
		ft_printf("> ");
		temp = str;
		str = ft_strjoin(str, line);
		free(temp);
		free(line);
	}
	free(line);
	temp = str;
	str = ft_chrjoin(str, '\n');
	free(temp);
	((t_binary_token *)(ast->right->content))->right = str;
}

t_ast				*create_redirection_ast_content(t_lexer *lexer, int pos)
{
	t_ast	*ast;

	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = (void *)ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = REDIRECTION;
	((t_token *)lexer->tokens.elem[pos])->type = USED;
	ast->right = (t_ast *)ft_memalloc(sizeof(t_ast));
//	ast->right->type = USED;
	ast->right->content = (t_binary_token *)ft_memalloc(sizeof(t_binary_token));
	((t_binary_token *)ast->right->content)->right = ((t_token *)lexer->tokens.elem[pos + 1])->str;
	if (ft_strequ(ast->content, "<<"))
		create_redirection_ast_content_heredoc(ast);
	((t_token *)lexer->tokens.elem[pos + 1])->type = USED;
	return (ast);
}

t_ast				*create_redirection_ast(t_lexer *lexer, int beg, int end)
{
	t_ast		*ast;
	char		*str;
	int			pos;
	char		*temp;

	if ((pos = first_token_pos(lexer, beg, end, io_file_op)) == -1)
		return (create_command(lexer, beg, end));
	if (lexer->tokens.len <= pos + 1 || pos + 1 > end)
	{
		ft_fprintf(2, "21sh: parse error - redirection word missed\n");
		free_ast(ast);
		return (0);
	}
	ast = create_redirection_ast_content(lexer, pos);
	if (pos >= 1 && ((t_token *)lexer->tokens.elem[pos - 1])->type == IO_NUMBER)
	{
		((t_binary_token *)ast->right->content)->left = ((t_token *)lexer->tokens.elem[pos - 1])->str;
		((t_token *)lexer->tokens.elem[pos - 1])->type = USED;
	}
	if (!(ast->left = create_redirection_ast(lexer, beg, end)) && free_ast(ast))
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
	if (!(ast->left = create_separator_ast(lexer, beg, pos - 1, level)) && free_ast(ast))
		return (0);
	if (!level && !(ast->right = create_separator_ast(lexer, pos + 1, end, level + 1)) &&
			free_ast(ast))
		return (0);
	else if (level == 1 && !(ast->right = create_redirection_ast(lexer, pos + 1, end)) &&
			free_ast(ast))
		return (0);
	return (ast);
}

int						parse_ast(t_ast *ast, t_shell *shell, int needfork);

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

int						parse_ast_pipe_child(t_ast *ast, t_shell *shell,
		int fdpipe[2], int is_out)
{
	pid_t	pid;
	int		a;
	int		b;
	t_ast	*side;

	a = is_out ? 0 : 1;
	b = is_out ? 1 : 0;
	side = is_out ? ast->left : ast->right;
	pid = fork();
	if (pid < 0)
	{
		ft_fprintf(2, "21sh: Error creating subprocess");
		return (-1);
	}
	if (!pid)
	{
		close(fdpipe[a]);
		dup2(fdpipe[b], b);
		close(fdpipe[b]);
		parse_ast(side, shell, 0);
		exit(0);
	}
	return (pid);
}

int						parse_ast_pipe(t_ast *ast, t_shell *shell)
{
	int			fdpipe[2];
	pid_t		pid[2];

	if (pipe(fdpipe) == -1)
	{
		ft_fprintf(2, "21sh: pipe creating error");
		return (0);
	}
	if ((pid[0] = parse_ast_pipe_child(ast, shell, fdpipe, 1)) == -1)
	{
		close(fdpipe[0]);
		close(fdpipe[1]);
		return (0);
	}
	if ((pid[1] = parse_ast_pipe_child(ast, shell, fdpipe, 0)) == -1)
	{
		close(fdpipe[0]);
		close(fdpipe[1]);
		kill(pid[0], SIGINT);
		return (0);
	}
	close(fdpipe[0]);
	close(fdpipe[1]);
	waitpid(pid[0], 0, 0);
	waitpid(pid[1], 0, 0);
	return (1);
}

int						parse_ast_command(t_ast *ast, t_shell *shell,
		int needfork)
{
	char	**args;

	args = ft_strsplit(ast->content, ' ');
	if (!handle_commands(args, &shell->env))
	{
		if (needfork)
		{
			if (ft_exec(args, &shell->env, 1) == -1)
			{
				free_double_arr(args);
				return (0);
			}
			wait(0);
		}
		else if (ft_exec(args, &shell->env, 0) == -1)
		{
			free_double_arr(args);
			return (0);
		}
	}
	free_double_arr(args);
	return (1);
}

int						parse_ast_redirection_right(t_ast *ast)
{
	int				rightfd;
	t_binary_token	*binary_token;

	rightfd = 0;
	binary_token = (t_binary_token *)ast->right->content;
	if (ft_strequ((char *)ast->content, ">"))
		rightfd = open(binary_token->right, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strequ((char *)ast->content, ">>"))
		rightfd = open(binary_token->right, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (ft_strequ((char *)ast->content, "<"))
		rightfd = open(binary_token->right, O_RDONLY, 0644);
	else if (ft_strequ((char *)ast->content, ">&") || ft_strequ((char *)ast->content, "<&"))
		rightfd = ft_atoi(binary_token->right);
	if (rightfd == -1)
	{
		ft_fprintf(2, "21sh: parse_redirection error\n", binary_token->right);
		return (-1);
	}
	return (rightfd);
}

int						parse_ast_redirection_kernel_heredoc(t_ast *ast, t_shell *shell, int fd)
{
	int		pipefd[2];
	int		oldfd;

	if (pipe(pipefd) == -1)
	{
		ft_fprintf(2, "21sh: pipe creating error");
		return (0);
	}
	dup2(pipefd[0], fd);
	close(pipefd[0]);
	write(pipefd[1], ((t_binary_token *)(ast->right->content))->right,
		ft_strlen(((t_binary_token *)(ast->right->content))->right));
	close(pipefd[1]);
	return (1);
}

int						parse_ast_redirection_kernel(t_ast *ast, t_shell *shell,
		int fd, int rightfd)
{
	int		oldfd;

	oldfd = dup(fd);
	if (ft_strequ(((t_binary_token *)(ast->right->content))->right, "-"))
		close(fd);
	else if (ft_strequ(ast->content, "<<"))
	{
		if (!parse_ast_redirection_kernel_heredoc(ast, shell, fd))
			return (0);
	}
	else
	{
		dup2(rightfd, fd);
		close(rightfd);
	}
	if (!parse_ast(ast->left, shell, 0))
		return (0);
	dup2(oldfd, fd);
	close(oldfd);
	return (1);
}

int						parse_ast_redirection_child(t_ast *ast, t_shell *shell)
{
	int			fd;
	int			rightfd;

	if (((t_binary_token *)(ast->right->content))->left)
		fd = ft_atoi(((t_binary_token *)(ast->right->content))->left);
	else if (((char *)ast->content)[0] == '>')
		fd = 1;
	else if (((char *)ast->content)[0] == '<')
		fd = 0;
	if ((rightfd = parse_ast_redirection_right(ast)) == -1)
		return (0);
	if (!parse_ast_redirection_kernel(ast, shell, fd, rightfd))
		return (0);
	return (1);
}

int						parse_ast_redirection(t_ast *ast, t_shell *shell,
		int needfork)
{
	pid_t		pid;

	if (needfork)
	{
		pid = fork();
		if (pid < 0)
		{
			ft_fprintf(2, "21sh: fork error\n");
			return (0);
		}
		if (!pid)
		{
			if (!parse_ast_redirection_child(ast, shell))
				exit(1);
		}
		else if (needfork)
			wait(0);
		return (1);
	}
	if (!parse_ast_redirection_child(ast, shell))
		return (0);
	return (1);
}

int						parse_ast(t_ast *ast, t_shell *shell, int needfork)
{
	if (!ast)
		return (0);
	if (ast->type == OPERATOR && ft_strequ(ast->content, ";"))
	{
		if (!parse_ast(ast->left, shell, 1))
			return (0);
		if (!parse_ast(ast->right, shell, 1))
			return (0);
	}
	else if (ast->type == OPERATOR && ft_strequ(ast->content, "|"))
	{
		if (!parse_ast_pipe(ast, shell))
			return (0);
	}
	else if (ast->type == REDIRECTION &&
			!parse_ast_redirection(ast, shell, needfork))
		return (0);
	else if (ast->type == COMMAND &&
			!parse_ast_command(ast, shell, needfork))
		return (0);
	return (1);
}

void				preparation(t_shell *shell)
{
	term_associate();
	shell->initfd.fdin = dup(0);
	shell->initfd.fdout = dup(1);
	shell->env = fill_env();
	//signal(SIGINT, int_handler);
}

int					main(void)
{
	char		*command;
	t_lexer		lexer;
	t_ast		*ast;
	t_shell		shell;

	char	buf[8];

	preparation(&shell);
	while (1)
	{
		change_termios(&shell.initfd, 0);
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
		change_termios(&shell.initfd, 1);
		parse_ast(ast, &shell, 1);
		free_lexer(&lexer);
		free_ast(ast);
		system("leaks -quiet 21sh");
	}
	free_double_arr(shell.env);
	//TODO: close shell->initfd
	return (0);
}
