/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creator_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 13:07:31 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 13:07:32 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

char				*(g_io_file_op[AM_IOFILEOP]) =
{
	"<", "<&", "<<", ">", ">&", ">>", NULL
};

static int			create_ast_heredoc_kernel(t_ast *ast,
		t_shell *shell, char *line, char *str)
{
	char	*temp;

	if (!line)
	{
		free_lineeditor(&shell->lineeditor);
		free(str);
		ft_putstr("\n");
		return (-1);
	}
	ft_printf("\n");
	free(line);
	temp = str;
	str = ft_chrjoin(str, '\n');
	free(temp);
	((t_binary_token *)(ast->right->content))->right = str;
	return (0);
}

static int			create_redirection_ast_content_heredoc(t_ast *ast,
		t_shell *shell)
{
	char	*line;
	char	*end;
	char	*str;
	char	*temp;

	end = ((t_binary_token *)ast->right->content)->right;
	str = 0;
	ft_printf("h> ");
	shell->reading_mode = HEREDOC;
	while ((line = input_command(&shell->lineeditor,
					&shell->history, 'h', shell)))
	{
		if (ft_strequ(line, end) || shell->reading_mode == READEND)
			break ;
		if (str &&
			(temp = str) &&
			(str = ft_chrjoin(str, '\n')))
			free(temp);
		ft_printf("\nh> ");
		temp = str;
		str = ft_strjoin(str, line);
		free(temp);
		free(line);
	}
	return (create_ast_heredoc_kernel(ast, shell, line, str));
}

static t_ast		*create_redirection_ast_content(t_lexer *lexer,
		int pos, t_shell *shell)
{
	t_ast	*ast;

	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = (void *)ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = REDIRECTION;
	((t_token *)lexer->tokens.elem[pos])->type = USED;
	ast->right = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->right->content = (t_binary_token *)ft_memalloc(sizeof(t_binary_token));
	((t_binary_token *)ast->right->content)->right =
		((t_token *)lexer->tokens.elem[pos + 1])->str;
	if (ft_strequ(ast->content, "<<"))
		if (create_redirection_ast_content_heredoc(ast, shell))
		{
			free(ast->right->content);
			free(ast->right);
			free(ast->content);
			free(ast);
			return (0);
		}
	((t_token *)lexer->tokens.elem[pos + 1])->type = USED;
	return (ast);
}

t_ast				*create_redirection_ast(t_lexer *lexer, int beg,
		int end, t_shell *shell)
{
	t_ast		*ast;
	int			pos;

	ast = 0;
	if ((pos = first_token_pos(lexer, beg, end, g_io_file_op)) == -1)
		return (create_command(lexer, beg, end));
	if (lexer->tokens.len <= pos + 1 || pos + 1 > end
			|| ((t_token *)lexer->tokens.elem[pos + 1])->type == OPERATOR)
	{
		ft_fprintf(2, "21sh: parse error - redirection word\n");
		free_ast(ast);
		return (0);
	}
	if (!(ast = create_redirection_ast_content(lexer, pos, shell)))
		return (0);
	if (pos >= 1 && ((t_token *)lexer->tokens.elem[pos - 1])->type == IO_NUMBER)
	{
		((t_binary_token *)ast->right->content)->left =
			((t_token *)lexer->tokens.elem[pos - 1])->str;
		((t_token *)lexer->tokens.elem[pos - 1])->type = USED;
	}
	if (!(ast->left = create_redirection_ast(lexer,
					beg, end, shell)) && free_ast(ast))
		return (0);
	return (ast);
}
