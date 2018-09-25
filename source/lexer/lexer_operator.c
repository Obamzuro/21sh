/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 12:57:16 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 21:27:07 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int					lexing_try_append_operator(char buf, char **tokenstr)
{
	int			i;
	char		*joining;
	int			tokenlen;
	extern char	*g_operators[AM_OPERATORS];

	if (!buf)
		return (0);
	tokenlen = ft_strlen(*tokenstr);
	joining = ft_chrjoin(*tokenstr, buf);
	i = -1;
	while (++i < AM_OPERATORS)
	{
		if (ft_strnstr(g_operators[i], joining, tokenlen + 1))
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
		if (ft_isnumber(((t_token *)lexer->tokens.elem[lexer->tokens.len
						- 1])->str) &&
				ft_is_str_in_args(token->str, 5, ">", ">&", ">>", "<", "<&"))
			((t_token *)lexer->tokens.elem[lexer->tokens.len
				- 1])->type = IO_NUMBER;
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
	extern char	*g_operators[AM_OPERATORS];

	i = -1;
	if (!buf)
		return (0);
	while (++i < AM_OPERATORS)
		if (buf == g_operators[i][0])
			return (1);
	return (0);
}

t_token				*lexing_init_operator_token(char buf,
		t_token *token, t_lexer *lexer)
{
	if (token && token->str)
		push_ftvector(&lexer->tokens, token);
	token = (t_token *)malloc(sizeof(t_token));
	token->str = (char *)ft_memalloc(2);
	token->str[0] = buf;
	token->type = OPERATOR;
	return (token);
}
