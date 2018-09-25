/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creator.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 16:35:41 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 21:23:11 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

char				*(g_separator_op[AM_LEVELS][AM_SEPARATOROP]) =
{
	{ ";", NULL },
	{ "|", NULL }
};

static t_ast		*create_separator_ast_pipe(int beg,
		int end, t_shell *shell)
{
	int		pos;
	t_ast	*ast;
	t_lexer	*lexer;

	lexer = shell->lexer;
	ast = 0;
	if ((pos = last_token_pos(lexer, beg, end, g_separator_op[1])) == -1)
		return (create_redirection_ast(lexer, beg, end, shell));
	if (pos == beg || pos + 1 > end)
		return (print_error_zero("21sh: parse operator error\n"));
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = OPERATOR;
	if (!(ast->left = create_separator_ast_pipe(beg,
					pos - 1, shell)) && free_ast(ast))
		return (0);
	if (!(ast->right = create_redirection_ast(lexer, pos + 1,
					end, shell)) && free_ast(ast))
		return (0);
	return (ast);
}

t_ast				*create_separator_ast(int beg, int end,
		t_shell *shell)
{
	int		pos;
	t_ast	*ast;
	t_lexer	*lexer;

	lexer = shell->lexer;
	ast = 0;
	if ((pos = last_token_pos(lexer, beg, end, g_separator_op[0])) == -1)
		return (create_separator_ast_pipe(beg, end, shell));
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = OPERATOR;
	if (pos != beg && !(ast->left = create_separator_ast(beg,
					pos - 1, shell)) && free_ast(ast))
		return (0);
	if (pos + 1 <= end && !(ast->right = create_separator_ast_pipe(pos + 1,
					end, shell)) && free_ast(ast))
		return (0);
	return (ast);
}
