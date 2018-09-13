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
		if (ft_isnumber(((t_token *)lexer->tokens.elem[lexer->tokens.len - 1])->str) &&
				ft_is_str_in_args(token->str, 5, ">", ">&", ">>", "<", "<&"))
			((t_token *)lexer->tokens.elem[lexer->tokens.len - 1])->type = IO_NUMBER;
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

t_token				*lexing_init_operator_token(char buf, t_token *token, t_lexer *lexer)
{
	if (token && token->str)
		push_ftvector(&lexer->tokens, token);
	token = (t_token *)malloc(sizeof(t_token));
	token->str = (char *)ft_memalloc(2);
	token->str[0] = buf;
	token->type = OPERATOR;
	return (token);
}

t_token				*lexing_handling_quotes(t_shell *shell, t_token *token, char **last,
		char **command)
{
	char	delim;
	char	*temp;

	delim = **last;
	while (1)
	{
		++(*last);
		if (**last == delim)
			break ;
		if (!token)
			token = (t_token *)ft_memalloc(sizeof(t_token));
		if (!**last)
		{
			temp = ft_chrjoin(token->str, '\n');
			free(token->str);
			token->str = temp;
			history_append(*command, &shell->history);
			history_append("\n", &shell->history);
			free(*command);
			line_editing_end(&shell->lineeditor, &shell->history);
			ft_putstr("\n$> ");
			*command = input_command(shell);
			*last = *command - 1;
			continue ;
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

void				lexing_handling_appword(t_token *token, char buf)
{
	char	*temp;

	temp = ft_chrjoin(token->str, buf);
	free(token->str);
	token->str = temp;
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
		ft_printf("%s | %d\n", ((t_token *)lexer->tokens.elem[i])->str, ((t_token *)lexer->tokens.elem[i])->type);
		++i;
	}
	ft_printf("\n");
}

void				tilde_expansion(t_ftvector *line)
{
	int				i;
	char			*slash;
	char			*temp;
	char			*temp2;
	struct stat		mystat;

	i = -1;
	while (line->elem[++i])
	{
		if (((char *)(line->elem[i]))[0] == '~')
		{
			if (((char *)(line->elem[i]))[1] == '/')
			{
				temp = ft_strjoin(getenv("HOME"), line->elem[i] + 1);
				free(line->elem[i]);
				line->elem[i] = temp;
			}
			else
			{
				slash = ft_strchr(line->elem[i] + 1, '/');
				if (slash)
				{
					*slash = 0;
					temp = ft_strjoin("/Users/", line->elem[i] + 1);
					if (lstat(temp, &mystat) != -1)
					{
						temp2 = ft_strjoin_path(temp, slash + 1);
						free(temp);
						free(line->elem[i]);
						line->elem[i] = temp2;
					}
					else
					{
						ft_fprintf(2, "21sh: no such user or named directory: %s", line->elem[i] + 1);
						*slash = '/';
					}
				}
				else
				{
					temp = ft_strjoin("/Users/", line->elem[i] + 1);
					if (lstat(temp, &mystat) != -1)
					{
						line->elem[i] = temp;
						free(line->elem[i]);
					}
					else
						ft_fprintf(2, "21sh: no such user or named directory: %s", line->elem[i] + 1);
				}
			}
		}
	}
}

void				lexer_creating_cycle(char **command, t_shell *shell,
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
			token = lexing_handling_quotes(shell, token, &last, command);
		else if (!*last && lexing_handling_end(shell->lexer, token))
			break ;
		else if (ft_strchr(" \t", *last))
			token = lexing_handling_separator(shell->lexer, token);
		else if (token && token->type == UKNOWN)
			lexing_handling_appword(token, *last);
		else
			token = lexing_handling_initword(token, *last);
	}
}

void				lexer_creating(char *command, t_shell *shell)
{
	t_token		*token;
	char		*last;

	token = NULL;
	last = command - 1;
	init_ftvector(&shell->lexer->tokens);
	lexer_creating_cycle(&command, shell, token, last);
//	lexing_print(lexer);
	history_append(command, &shell->history);
	free(command);
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
