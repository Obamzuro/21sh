/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 17:17:25 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/23 19:16:46 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

void				print_ast(t_ast *ast)
{
	int		i;

	i = 0;
	while (1)
	{
		if (!ast)
			break ;
		if (ast->type == COMMAND)
			ft_printf("AST #%d - %s %d\n", i, ((char **)ast->content)[0], ast->type);
		else
			ft_printf("AST #%d - %s %d\n", i, ast->content, ast->type);
		if (ast->right)
		{
			if (ast->type == REDIRECTION)
				ft_printf("AST #%d - |%s  %s| %d\n", i,
					((t_binary_token *)ast->right->content)->left,
					((t_binary_token *)ast->right->content)->right,
					ast->right->type);
			else if (ast->type == COMMAND)
				ft_printf("AST #%d - %s %d\n", i, ((char **)ast->content)[0], ast->type);
			else
				ft_printf("AST #%d - %s %d\n", i, ast->right->content,
						ast->right->type);
		}
		else
			ft_printf("AST #%d right - NULL\n", i);
		ast = ast->left;
		++i;
	}
}

int					parse_ast_pipe_child(t_ast *ast, t_shell *shell,
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

int					parse_ast_pipe(t_ast *ast, t_shell *shell)
{
	int			fdpipe[2];
	pid_t		pid[2];

	if (pipe(fdpipe) == -1)
		return ((int)print_error_zero("21sh: pipe creating error"));
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
	while (waitpid(pid[0], 0, 0) == -1)
		;
	while (waitpid(pid[1], 0, 0) == -1)
		;
	return (1);
}

int					parse_ast_command(t_ast *ast, t_shell *shell,
		int needfork)
{
	tilde_expansion(shell, ast->content);
	env_expansion(shell, ast->content);
	quote_removing(shell,ast->content);
	if (!handle_commands(ast->content, &shell->env))
	{
		if (needfork)
		{
			if (ft_exec(ast->content, &shell->env, 1) == -1)
				return (0);
			while (wait(0) == -1)
				;
		}
		else if (ft_exec(ast->content, &shell->env, 0) == -1)
			return (0);
	}
	return (1);
}

int					parse_ast_redirection_right(t_ast *ast)
{
	int				rightfd;
	t_binary_token	*binary_token;

	rightfd = 0;
	binary_token = (t_binary_token *)ast->right->content;
	if (ft_strequ((char *)ast->content, ">"))
		rightfd = open(binary_token->right, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strequ((char *)ast->content, ">>"))
		rightfd = open(binary_token->right, O_WRONLY |
				O_CREAT | O_APPEND, 0644);
	else if (ft_strequ((char *)ast->content, "<"))
		rightfd = open(binary_token->right, O_RDONLY, 0644);
	else if (ft_strequ((char *)ast->content, ">&")
			|| ft_strequ((char *)ast->content, "<&"))
		rightfd = ft_atoi(binary_token->right);
	if (rightfd == -1)
	{
		ft_fprintf(2, "21sh: parse_redirection error\n", binary_token->right);
		return (-1);
	}
	return (rightfd);
}

int					parse_ast_redirection_kernel_heredoc(t_ast *ast, int fd)
{
	int		pipefd[2];

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

int					parse_ast_redirection_kernel(t_ast *ast, t_shell *shell,
		int fd, int rightfd)
{
	int		oldfd;
	int		devnull;

	oldfd = dup(fd);
	if (ft_strequ(((t_binary_token *)(ast->right->content))->right, "-"))
	{
		devnull = open("/dev/null", O_RDWR);
		dup2(devnull, fd);
		close(devnull);
	}
	else if (ft_strequ(ast->content, "<<"))
	{
		if (!parse_ast_redirection_kernel_heredoc(ast, fd))
			return (0);
	}
	else
		dup2(rightfd, fd);
	if (!parse_ast(ast->left, shell, 0))
		return (0);
	dup2(oldfd, fd);
	close(oldfd);
	return (1);
}

int					parse_ast_redirection_child(t_ast *ast, t_shell *shell)
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

int					parse_ast_redirection(t_ast *ast, t_shell *shell,
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
			while (wait(0) == -1)
				;
		return (1);
	}
	if (!parse_ast_redirection_child(ast, shell))
		return (0);
	return (1);
}

int					parse_ast(t_ast *ast, t_shell *shell, int needfork)
{
	if (!ast)
		return (-1);
	if (ast->type == OPERATOR && ft_strequ(ast->content, ";"))
	{
		if (parse_ast(ast->left, shell, 1))
			return (-1);
		if (parse_ast(ast->right, shell, 1))
			return (-1);
	}
	else if (ast->type == OPERATOR && ft_strequ(ast->content, "|"))
	{
		if (!parse_ast_pipe(ast, shell))
			return (-1);
	}
	else if (ast->type == REDIRECTION &&
			!parse_ast_redirection(ast, shell, needfork))
		return (-1);
	else if (ast->type == COMMAND &&
			!parse_ast_command(ast, shell, needfork))
		return (-1);
	return (0);
}
