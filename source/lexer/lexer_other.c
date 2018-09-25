/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_other.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 12:57:53 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/25 13:53:12 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int					lexing_handling_appword(t_token *token, char buf)
{
	char	*temp;

	temp = ft_chrjoin(token->str, buf);
	free(token->str);
	token->str = temp;
	return (0);
}

static int			lexing_handling_end_pipemore(t_shell *shell,
		char **last, char **command)
{
	free(*command);
	line_editing_end(&shell->lineeditor, &shell->history);
	ft_putstr("\n|> ");
	shell->reading_mode = PIPE;
	if ((!(*command = input_command(&shell->lineeditor,
			&shell->history, '|', shell)) && ft_printf("\n"))
			|| shell->reading_mode == READEND)
	{
		if (shell->reading_mode == READEND)
			ft_printf("\n21sh: syntax error: unexpected eof\n");
		free_lineeditor(&shell->lineeditor);
		return (-1);
	}
	*last = *command - 1;
	return (0);
}

int					lexing_handling_end(t_shell *shell, t_token *token,
		char **last, char **command)
{
	t_ftvector		*tokens;

	tokens = &shell->lexer->tokens;
	if (token && token->str)
	{
		token->type = WORD;
		push_ftvector(tokens, token);
	}
	else if (!token && tokens->len > 0 &&
			ft_strequ(((t_token *)tokens->elem[tokens->len - 1])->str, "|"))
		return (lexing_handling_end_pipemore(shell, last, command));
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

t_token				*lexing_handling_initword(t_token *token, char buf)
{
	token = (t_token *)malloc(sizeof(t_token));
	token->str = (char *)ft_memalloc(2);
	token->str[0] = buf;
	token->type = UKNOWN;
	return (token);
}
