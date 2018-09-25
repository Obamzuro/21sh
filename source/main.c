/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/25 15:12:07 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static char				*lineediting(t_shell *shell)
{
	char		*command;

	if (!(command = input_command(&shell->lineeditor,
					&shell->history, '$', shell)) || !*command)
	{
		le_unselect(&shell->lineeditor, &shell->history, NULL);
		print_buffer(&shell->lineeditor, &shell->history, 0, 0);
		line_editing_end(&shell->lineeditor, &shell->history);
		ft_printf("\n");
		free_lineeditor(&shell->lineeditor);
		return (NULL);
	}
	le_unselect(&shell->lineeditor, &shell->history, NULL);
	print_buffer(&shell->lineeditor, &shell->history, 0, 0);
	line_editing_end(&shell->lineeditor, &shell->history);
	return (command);
}

static int				lexing(t_shell *shell, char *command)
{
	if (lexer_creating(command, shell))
	{
		free_lexer(shell->lexer);
		return (-1);
	}
	write(1, "\n", 1);
	if (!shell->lexer->tokens.len)
	{
		free_lexer(shell->lexer);
		--shell->history.last;
		free(shell->history.commands[shell->history.last]);
		shell->history.commands[shell->history.last] = 0;
		return (-1);
	}
	return (0);
}

static int				creating_ast(t_shell *shell)
{
	if (!(shell->ast = create_separator_ast(0,
					shell->lexer->tokens.len - 1, shell)))
	{
		free_lexer(shell->lexer);
		return (-1);
	}
	return (0);
}

static void				preparation(t_shell *shell)
{
	struct sigaction		act;
	extern struct termios	g_tty;

	ft_bzero(&act, sizeof(act));
	act.sa_handler = int_handler;
	term_associate();
	tcgetattr(STDIN_FILENO, &g_tty);
	if ((shell->initfd.fdin = dup(0)) == -1 ||
		(shell->initfd.fdout = dup(1)) == -1 ||
		(shell->initfd.fderr = dup(2)) == -1)
	{
		ft_fprintf(2, "21sh: dup() error\n");
		exit(EXIT_FAILURE);
	}
	shell->lexer = (t_lexer *)malloc(sizeof(t_lexer));
	ft_bzero(&shell->history, sizeof(shell->history));
	signal(SIGTSTP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGSTOP, SIG_IGN);
	if (sigaction(SIGINT, &act, 0) == -1 ||
		sigaction(SIGWINCH, &act, 0) == -1)
	{
		ft_fprintf(2, "21sh: sigaction error\n");
		exit(EXIT_FAILURE);
	}
}

int						main(void)
{
	char		*command;
	t_shell		shell;

	preparation(&shell);
	shell.env = fill_env();
	while (1)
	{
		change_termios(&shell.initfd, 0);
		ft_bzero(shell.lexer, sizeof(t_lexer));
		ft_printf("\e[31m[%s]\x1b[0m\n", get_env("PWD", shell.env));
		ft_printf("$> ");
		shell.reading_mode = BASIC;
		if (!(command = lineediting(&shell)) ||
			lexing(&shell, command) ||
			creating_ast(&shell))
			continue;
		change_termios(&shell.initfd, 1);
		parse_ast(shell.ast, &shell, 1);
		free_lexer(shell.lexer);
		free_ast(shell.ast);
	}
	free_double_arr(shell.env);
	free(shell.lexer);
	return (0);
}
