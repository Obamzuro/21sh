/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creator_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 13:07:28 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 22:02:24 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int			create_command_getcount_args(t_lexer *lexer,
		int beg, int end)
{
	int		i;

	i = 0;
	while (beg <= end)
	{
		if (((t_token *)lexer->tokens.elem[beg])->type == WORD)
			++i;
		++beg;
	}
	return (i);
}

t_ast				*create_command(t_lexer *lexer, int beg, int end)
{
	t_ast	*ast;
	char	**args;
	int		i;

	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	args = (char **)ft_memalloc(sizeof(char *) *
			(create_command_getcount_args(lexer, beg, end) + 1));
	i = 0;
	while (beg <= end)
	{
		if (((t_token *)lexer->tokens.elem[beg])->type == WORD)
		{
			args[i] = ft_strdup(((t_token *)lexer->tokens.elem[beg])->str);
			++i;
		}
		++beg;
	}
	ast->content = (void *)args;
	ast->type = COMMAND;
	return (ast);
}
