/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 16:28:21 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/23 19:15:35 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

int					lexing_try_append_operator(char buf, char **tokenstr)
{
	int			i;
	char		*joining;
	int			tokenlen;
	extern char	*operators[AM_OPERATORS];

	if (!buf)
		return (0);
	tokenlen = ft_strlen(*tokenstr);
	joining = ft_chrjoin(*tokenstr, buf);
	i = -1;
	while (++i < AM_OPERATORS)
	{
		if (ft_strnstr(operators[i], joining, tokenlen + 1))
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
	extern char	*operators[AM_OPERATORS];

	i = -1;
	if (!buf)
		return (0);
	while (++i < AM_OPERATORS)
		if (buf == operators[i][0])
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

int					lexing_handling_appword(t_token *token, char buf)
{
	char	*temp;

	temp = ft_chrjoin(token->str, buf);
	free(token->str);
	token->str = temp;
	return (0);
}

int					lexing_handling_quotes_ifnull(t_shell *shell,
		t_token *token, char **last, char **command)
{
	char	*temp;

	temp = ft_chrjoin(token->str, '\n');
	free(token->str);
	token->str = temp;
	free(*command);
	line_editing_end(&shell->lineeditor, &shell->history);
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

t_token				*lexing_handling_quotes(t_shell *shell,
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

int					lexing_handling_end(t_lexer *lexer, t_token *token)
{
	if (token && token->str)
	{
		token->type = WORD;
		push_ftvector(&lexer->tokens, token);
	}
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

void				lexing_print(t_lexer *lexer)
{
	int i;

	i = 0;
	ft_printf("\n");
	while (i < lexer->tokens.len)
	{
		ft_printf("%s | %d\n", ((t_token *)lexer->tokens.elem[i])->str,
				((t_token *)lexer->tokens.elem[i])->type);
		++i;
	}
	ft_printf("\n");
}

void				lexer_creating_cycle_cont(t_shell *shell, t_token **token,
		char *last)
{
	if (ft_strchr(" \t", *last))
		*token = lexing_handling_separator(shell->lexer, *token);
	else if (*token && (*token)->type == UKNOWN)
		lexing_handling_appword(*token, *last);
	else
		*token = lexing_handling_initword(*token, *last);
}

int					lexer_creating_cycle(char **command, t_shell *shell,
		t_token *token, char *last)
{
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
		else if ('\"' == *last || '\'' == *last)
		{
			if (!(token = lexing_handling_quotes(shell, token, &last, command)))
				return (-1);
		}
		else if (!*last && lexing_handling_end(shell->lexer, token))
			break ;
		else
			lexer_creating_cycle_cont(shell, &token, last);
	}
	return (0);
}

int					lexer_creating(char *command, t_shell *shell)
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

void				free_lexer(t_lexer *lexer)
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
