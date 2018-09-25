/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 16:28:21 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/25 13:52:34 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

/*
** void				lexing_print(t_lexer *lexer)
** {
** 	int i;
**
** 	i = 0;
** 	ft_printf("\n");
** 	while (i < lexer->tokens.len)
** 	{
** 		ft_printf("%s | %d\n", ((t_token *)lexer->tokens.elem[i])->str,
** 				((t_token *)lexer->tokens.elem[i])->type);
** 		++i;
** 	}
** 	ft_printf("\n");
** }
*/

static int					lexer_creating_cycle_cont(t_shell *shell,
		t_token **token, char **last, char **command)
{
	if ('\"' == **last || '\'' == **last)
	{
		if (!(*token = lexing_handling_quotes(shell, *token, last, command)))
			return (-1);
	}
	else if (!**last)
		return (lexing_handling_end(shell, *token, last, command));
	else if (ft_strchr(" \t", **last))
		*token = lexing_handling_separator(shell->lexer, *token);
	else if (*token && (*token)->type == UKNOWN)
		lexing_handling_appword(*token, **last);
	else
		*token = lexing_handling_initword(*token, **last);
	return (0);
}

static int					lexer_creating_cycle(char **command, t_shell *shell,
		t_token *token, char *last)
{
	int		ret;

	while (1)
	{
		++last;
		if (token && token->type == OPERATOR)
		{
			if (lexing_try_append_operator(*last, &token->str))
				continue ;
			token = lexing_divide_operator(shell->lexer, token);
		}
		if (lexing_is_operator_begin(*last))
		{
			token = lexing_init_operator_token(*last, token, shell->lexer);
			continue ;
		}
		else
			ret = lexer_creating_cycle_cont(shell, &token, &last, command);
		if (ret == -1)
			return (-1);
		else if (ret == 1)
			break ;
	}
	return (0);
}

int							lexer_creating(char *command, t_shell *shell)
{
	t_token		*token;
	char		*last;

	token = NULL;
	last = command - 1;
	init_ftvector(&shell->lexer->tokens);
	history_append(command, &shell->history, 0);
	if (lexer_creating_cycle(&command, shell, token, last))
		return (-1);
	free(command);
	return (0);
}

void						free_lexer(t_lexer *lexer)
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
