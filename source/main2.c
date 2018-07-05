/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/04 11:34:53 by obamzuro          #+#    #+#             */
/*   Updated: 2018/07/05 14:57:46 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

const char	*g_operators[] = 
	{
		">",
		">&",
		">>",
		"<",
		"<&",
		"<<",
		"|"
	};

int		operator_part(char buf, char **token)
{
	int		i;
	char	*joining;
	int		tokenlen;

	tokenlen = ft_strlen(*token);
	joining = (char *)malloc(tokenlen + 2);
	ft_memcpy(joining, token, tokenlen);
	joining[tokenlen] = buf;
	joining[tokenlen + 1] = 0;
	i = -1;
	while (++i < sizeof(g_operators) / sizeof(g_operators[0]))
	{
		if (ft_strnstr(g_operators[i], joining, tokenlen + 1))
		{
			free(*token);
			*token = joining;
			return (1);
		}
	}
	return (0);
}

int		operator_begin(char buf)
{
	int		i;

	i = -1;
	while (++i < AM_OPERATORS)
	{
		if (buf == g_operators[i][0])
			return (1);
	}
	return (0);
}

char	*handle_quotes(t_lexer *lexer, char quote,
		char *line, int i)
{
	char	buf;
	char	*ret;
	char	*temp;

	ret = 0;
	while (1)
	{
		++i;
		buf = line[i];
		if (buf == quote)
			break ;
		if (buf == '\n')
			break ;
		temp = ret;
		ret = (char *)malloc(ft_strlen(ret) + 2);
		memcpy(ret, temp, ft_strlen(temp));
		ret[ft_strlen(temp)] = buf;
		ret[ft_strlen(temp) + 1] = 0;
		free(temp);
	}
	return (ret);
}

void	lexing(t_lexer *lexer, char **line)
{
	char		*temp;
	t_token		*token;
	char		buf;
	int			i;

	token = (t_token *)malloc(sizeof(t_token));
	token->str = 0;
	init_ftvector(&lexer.tokens);
	i = -1;
	while (1)
	{
		++i;
		buf = line[i];
		if (token->type == OPERATOR)
		{
			if (operator_part(buf, &token->str))
				continue ;
			push_ftvector(&lexer.tokens, token);
			token = (t_token *)malloc(sizeof(t_token));
			token->str = 0;
		}
		if (operator_begin(buf))
		{
			if (token->str)
			{
				push_ftvector(&lexer.tokens, token);
				token = (t_token *)malloc(sizeof(t_token));
			}
			token->str = (char *)malloc(2);
			token->str[0] = buf;
			token->str[1] = 0;
			token->type = OPERATOR:
			continue ;
		}
		else if (buf == ''' || buf == '"')
		{
			temp = ft_strjoin(token->str, handle_quotes(&lexer, buf, line, i));
			free(token->str);
			token->str = temp
		}
		else if (buf == '\n')
		{
			push_ftvector(&lexer.tokens, token);
			break ;
		}
		else if (buf == ' ')
		{
			if (token->str)
			{
				push_ftvector(&lexer.tokens, token);
				token = (t_token *)malloc(sizeof(t_token));
				token->str = 0;
			}
		}
		else if (token->type != OPERATOR)
		{
			temp = ft_strjoin(token->str, buf);
			free(token->str);
			token->str = temp;
		}
		else
		{
			token->str = (char *)malloc(2);
			token->str[0] = buf;
			token->str[1] = 0;
			token->type = WORD;
		}
	}
}

void	line_reading(char **line)
{
	char	buf[8];
	char	*temp;

	*line = 0;
	while (1)
	{
		if (read(0, buf, sizeof(buf)) <= 0)
			break ;
		if (!ft_strcmp(buf, "\n"))
			break ;
		write(1, buf, sizeof(buf));
		temp = ft_strjoin(line, buf);
		free(line);
		line = temp;
	}
}

void	print_lexer_tokens(const t_lexer *lexer)
{
	int		i;

	i = 0;
	while (i < lexer->tokens.len)
	{
		ft_printf("tokenType = %d, token = %s\n", ((t_token *)lexer->tokens[i])->type,
				((t_token *)lexer->tokens[i])->str,);
		++i;
	}
}

int		main(void)
{
	char	**env;
	char	*line;
	t_lexer	lexer;

	env = fill_env();
	term_associate();
	set_noncanon();
	while (1)
	{
		ft_printf("$> ");
		line_reading(&line);
		lexing(&lexer, &line);
		print_lexer_token(&lexer);
	}
	free_double_arr(env);
	return (0);
}
