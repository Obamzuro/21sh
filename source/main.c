/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2018/08/23 22:19:20 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

//static void			handle_commands(char *line,
//		t_comm_corr commands[AM_COMMANDS],
//		char ***env)
//{
//	int		i;
//	char	**args;
//
//	if (!(args = ft_strsplit2(line, " \t"))[0])
//	{
//		free_double_arr(args);
//		return ;
//	}
//	replace_env_variable(args, *env);
//	i = -1;
//	while (++i < AM_COMMANDS)
//	{
//		if (!ft_strncmp(commands[i].comm, args[0],
//					ft_strlen(commands[i].comm) + 1))
//		{
//			commands[i].func(args, env);
//			break ;
//		}
//	}
//	if (i == AM_COMMANDS)
//		ft_exec(args, env);
//	free_double_arr(args);
//}

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

int					identify_operator(char buf)
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

int					try_append_operator(char buf, char **tokenstr)
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

//char				*read_until(t_lexer *lexer, char delim)
//{
//	char buf[8];
//	char *ret;
//	char *temp;
//
//	ret = 0;
//	while (1)
//	{
//		ft_bzero(buf, sizeof(buf));
//		read(0, buf, sizeof(buf));
//		if (!ft_strcmp(buf, delim))
//			break ;
//		temp = ret;
//		ret = ft_strjoin(ret, buf);
//		free(temp);
//		write(1, buf, sizeof(buf));
//	}
//	write(1, buf, sizeof(buf));
//	return (ret);
//}

void				lexer_creating(char *command, t_lexer *lexer)
{
	char		*temp;
	t_token		*token;
	char		buf;
	int			j;

	token = (t_token *)ft_memalloc(sizeof(t_token));
	init_ftvector(&lexer->tokens);
	j = -1;
	while (1)
	{
		++j;
		buf = command[j];
		if (token->type == OPERATOR)
		{
			if (try_append_operator(buf, &token->str))
				continue ;
			if (token->str)
			{
				push_ftvector(&lexer->tokens, token);
				token = (t_token *)ft_memalloc(sizeof(t_token));
			}
		}
		if (identify_operator(buf))
		{
			if (token->str)
			{
				push_ftvector(&lexer->tokens, token);
				token = (t_token *)malloc(sizeof(t_token));
			}
			token->str = (char *)ft_memalloc(2);
			token->str[0] = buf;
			token->type = OPERATOR;
			continue ;
		}
		else if ('\"' == buf || '\'' == buf)
		{
			token->type = WORD;
			char	delim;
			delim = buf;
			while (1)
			{
				++j;
				buf = command[j];
				if (buf == delim)
					break ;
				if (!buf)
				{
					temp = ft_chrjoin(token->str, '\n');
					free(token->str);
					token->str = temp;
					free(command);
					command = input_command();
					j = -1;
					continue ;
				}
				temp = ft_chrjoin(token->str, buf);
				free(token->str);
				token->str = temp;
			}
		}
		else if (!buf)
		{
			if (token->str)
				push_ftvector(&lexer->tokens, token);
			break ;
		}
		else if (ft_strchr(" \t", buf))
		{
			if (token->str)
			{
				push_ftvector(&lexer->tokens, token);
				token = (t_token *)ft_memalloc(sizeof(t_token));
			}
		}
		else if (token->type == WORD)
		{
			temp = ft_chrjoin(token->str, buf);
			free(token->str);
			token->str = temp;
		}
		else
		{
			token->str = (char *)ft_memalloc(2);
			token->str[0] = buf;
			token->type = WORD;
		}
	}
	//lexer_creating_mbmore(lexer);
	//tilde_expansion(&lexer->tokens);
	int i = 0;
	ft_printf("\n");
	while (i < lexer->tokens.len)
	{
		ft_printf("%s | %d\n", ((t_token *)lexer->tokens.elem[i])->str, ((t_token *)lexer->tokens.elem[i])->type);
		++i;
	}
	ft_printf("\n");
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
	{ ";", "&" },
	{ "|", "|" }
};

char				*(io_file_op[AM_IOFILEOP]) =
{
	"<", "<&", ">", ">&", ">>", "<>", ">|"
};

int					last_token_pos(t_lexer *lexer, int beg, int end, char **findname, int names, t_tokentype tokentype)
{
	int		i;
	int		j;
	int		lasttokenpos;

	lasttokenpos = -1;
	i = beg - 1;
	while (++i <= end)
	{
		j = -1;
		while (++j < names)
			if (ft_strequ(((t_token *)lexer->tokens.elem[i])->str, findname[j]) &&
					((t_token *)lexer->tokens.elem[i])->type == tokentype)
				lasttokenpos = i;
	}
	return (lasttokenpos);
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

	if ((pos = last_token_pos(lexer, beg, end, io_file_op, AM_IOFILEOP, OPERATOR)) == -1)
		return (create_command(lexer, beg, end));
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = (void *)ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = REDIRECTION;
	((t_token *)lexer->tokens.elem[pos])->type = USED;

	// TODO: to free if parse error
	ast->right = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->right->content = (t_binary_token *)ft_memalloc(sizeof(t_binary_token));

	// try to catch previous token
	if (pos >= 1 && ft_isnumber(((t_token *)lexer->tokens.elem[pos - 1])->str))
	{
		((t_binary_token *)ast->right->content)->left = ((t_token *)lexer->tokens.elem[pos - 1])->str;
		((t_token *)lexer->tokens.elem[pos - 1])->type = USED;
	}

	// check ast->right correct input
	if (lexer->tokens.len <= pos + 1 || pos + 1 > end)
	{
		ft_fprintf(2, "21sh: parse error - redirection word missed\n");
		return (0);
	}
	((t_binary_token *)ast->right->content)->right = ((t_token *)lexer->tokens.elem[pos + 1])->str;
	ast->right->type = USED;
	((t_token *)lexer->tokens.elem[pos + 1])->type = USED;
	if (!ast->left && !(ast->left = create_redirection_ast(lexer, beg, end)))
		return (0);
	return (ast);
}

t_ast				*create_separator_ast(t_lexer *lexer, int beg, int end, int level)
{
	int		pos;
	t_ast	*ast;

	if ((pos = last_token_pos(lexer, beg, end, separator_op[level], AM_SEPARATOROP, OPERATOR)) == -1)
	{
		if (level == AM_LEVELS - 1)
			return (create_redirection_ast(lexer, beg, end));
		else
			return (create_separator_ast(lexer, beg, end, level + 1));
	}
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	if (lexer->tokens.len <= pos + 1 || !pos)
	{
		ft_fprintf(2, "21sh: parse error - pipe incorrect position\n");
		return (0);
	}
	ast->content = (void *)ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = OPERATOR;
	if (!(ast->left = create_separator_ast(lexer, beg, pos - 1, level)))
		return (0);
	if (!(ast->right = create_redirection_ast(lexer, pos + 1, end)))
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
			oldfd[0] = dup(fd);
			dup2(newfd, fd);
			close(newfd);
			process = parse_ast(ast->left, env, ispipe, initfd, 1);
			dup2(oldfd[0], fd);
			close(oldfd[0]);
		}
		wait(0);
		return (process);
	}
	else if (ast->type == COMMAND)
	{
		args = ft_strsplit2(ast->content, " \t");
		if (!ispipe)
		{
//			change_termios(initfd, 1);
			if (!isfork)
			{
				process = ft_exec(args, env, 1);
				wait(0);
			}
			else
				process = ft_exec(args, env, 0);
//			change_termios(initfd, 0);
		}
		else
			process = ft_exec(args, env, 0);
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

void				zombie_handler(int sig)
{
//	printf("SIGCHLD!\n");
}

int					main(void)
{
//	char		*line;
//	int			i;
	char		**env;
	t_comm_corr	commands[AM_COMMANDS];
	char		*command;
	t_lexer		lexer;
	t_ast		*ast;
	t_initfd	initfd;
//	char		**args;

	initfd.fdin = dup(0);
	initfd.fdout = dup(1);
	g_sigint = 0;
	//signal(SIGINT, int_handler);
	signal(SIGCHLD, zombie_handler);
	//fill_commands(commands);
	env = fill_env();
	term_associate();
	change_termios(&initfd, 0);
	while (1)
	{
		ft_bzero(&lexer, sizeof(t_lexer));
		ft_bzero(&ast, sizeof(t_ast));
		ft_printf("$> ");
		g_sigint = 0;
		command = input_command();
		lexer_creating(command, &lexer);
		if (!lexer.tokens.len)
		{
			ft_fprintf(2, "21sh: lexing error\n");
			free_lexer(&lexer);
			continue ;
		}
		ast = create_separator_ast(&lexer, 0, lexer.tokens.len - 1, 0);
		print_ast(ast);
		change_termios(&initfd, 1);
		parse_ast(ast, &env, 0, &initfd, 0);
		change_termios(&initfd, 0);
		free_lexer(&lexer);
		free_ast(ast);
//		system("leaks -quiet 21sh");
//		get_next_line(0, &line);
//		g_sigint = 1;
//		args = ft_strsplit(line, ';');
//		i = -1;
//		while (args[++i])
//			handle_commands(args[i], commands, &env);
//		free_double_arr(args);
//		free(line);
	}
	free_double_arr(env);
	return (0);
}
