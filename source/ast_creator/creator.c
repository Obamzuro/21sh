#include "21sh.h"
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

void				create_redirection_ast_content_heredoc(t_ast *ast, t_shell *shell)
{
	char	*line;
	char	*end;
	char	*str;
	char	*temp;

	end = ((t_binary_token *)ast->right->content)->right;
	str = 0;
	ft_printf("> ");
	while ((line = input_command(&shell->history)))
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

t_ast				*create_redirection_ast_content(t_lexer *lexer, int pos, t_shell *shell)
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
		create_redirection_ast_content_heredoc(ast, shell);
	((t_token *)lexer->tokens.elem[pos + 1])->type = USED;
	return (ast);
}

t_ast				*create_redirection_ast(t_lexer *lexer, int beg, int end, t_shell *shell)
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
	ast = create_redirection_ast_content(lexer, pos, shell);
	if (pos >= 1 && ((t_token *)lexer->tokens.elem[pos - 1])->type == IO_NUMBER)
	{
		((t_binary_token *)ast->right->content)->left = ((t_token *)lexer->tokens.elem[pos - 1])->str;
		((t_token *)lexer->tokens.elem[pos - 1])->type = USED;
	}
	if (!(ast->left = create_redirection_ast(lexer, beg, end, shell)) && free_ast(ast))
		return (0);
	return (ast);
}

t_ast				*create_separator_ast(t_lexer *lexer, int beg, int end, int level, t_shell *shell);

t_ast				*create_separator_ast_notfound(t_lexer *lexer,
		int beg, int end, int level, t_shell *shell)
{
	if (level == 1)
		return (create_redirection_ast(lexer, beg, end, shell));
	else
		return (create_separator_ast(lexer, beg, end, level + 1, shell));
}

t_ast				*create_separator_ast(t_lexer *lexer, int beg, int end, int level, t_shell *shell)
{
	int		pos;
	t_ast	*ast;

	if ((pos = last_token_pos(lexer, beg, end, separator_op[level])) == -1)
		return (create_separator_ast_notfound(lexer, beg, end, level, shell));
	if (lexer->tokens.len <= pos + 1 || !pos)
	{
		ft_fprintf(2, "21sh: parse operator error - incorrect position\n");
		return (0);
	}
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = (void *)ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = OPERATOR;
	if (!(ast->left = create_separator_ast(lexer, beg, pos - 1, level, shell)) && free_ast(ast))
		return (0);
	if (!level && !(ast->right = create_separator_ast(lexer, pos + 1, end, level + 1, shell)) &&
			free_ast(ast))
		return (0);
	else if (level == 1 && !(ast->right = create_redirection_ast(lexer, pos + 1, end, shell)) &&
			free_ast(ast))
		return (0);
	return (ast);
}
