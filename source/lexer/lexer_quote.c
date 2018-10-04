/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 12:58:07 by obamzuro          #+#    #+#             */
/*   Updated: 2018/10/04 15:13:35 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int					lexing_handling_quotes_ifnull(t_shell *shell,
		t_token *token, char **last, char **command)
{
	char	*temp;

	temp = ft_chrjoin(token->str, '\n');
	line_editing_end(&shell->lineeditor, &shell->history);
	free(token->str);
	token->str = temp;
	free(*command);
	ft_putstr("\nq> ");
	shell->reading_mode = QUOTE;
	if ((!(*command = input_command(&shell->lineeditor,
					&shell->history, 'q', shell)) && ft_printf("\n"))
		|| shell->reading_mode == READEND)
	{
		if (shell->reading_mode == READEND)
			ft_printf("\n21sh: syntax error: unexpected eof\n");
		free(token->str);
		free(token);
		free_lineeditor(&shell->lineeditor);
		return (-1);
	}
	history_append("\n", &shell->history, 1);
	history_append(*command, &shell->history, 1);
	*last = *command - 1;
	return (0);
}

t_token						*lexing_handling_quotes(t_shell *shell,
		t_token *token, char **last, char **command)
{
	char	delim;
	char	*temp;

	delim = **last;
	if (!token)
		token = (t_token *)ft_memalloc(sizeof(t_token));
	lexing_handling_appword(token, delim);
	while (1)
	{
		++(*last);
		if (**last == delim && !lexing_handling_appword(token, delim))
			break ;
		if (!**last)
		{
			if (lexing_handling_quotes_ifnull(shell,
							token, last, command))
				return (NULL);
			else
				continue;
		}
		temp = ft_chrjoin(token->str, **last);
		free(token->str);
		token->str = temp;
	}
	return (token);
}
