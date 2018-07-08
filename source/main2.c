/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/04 11:34:53 by obamzuro          #+#    #+#             */
/*   Updated: 2018/07/05 18:38:00 by obamzuro         ###   ########.fr       */
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
	joining = ft_chrjoin(*token, buf);
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
	free(joining);
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

void	line_reading(char **line);

char	*handle_quotes(t_lexer *lexer, char quote,
		char **line, int *i)
{
	char	buf;
	char	*ret;
	char	*temp;

	ret = 0;
	while (1)
	{
		++(*i);
		buf = (*line)[*i];
		if (buf == quote)
			break ;
		if (buf == '\0')
		{
			temp = ret;
			ret = ft_chrjoin(ret, '\n');
			free(temp);
			free(*line);
			line_reading(line);
			*i = -1;
			continue ;
		}
		temp = ret;
		ret = ft_chrjoin(ret, buf);
		free(temp);
	}
	return (ret);
}

void	lexing(t_lexer *lexer, char **line)
{
	char		*temp;
	char		*temp2;
	t_token		*token;
	char		buf;
	int			i;

	if (!lexer || !*line)
		return ;
	token = 0;
	init_ftvector(&lexer->tokens);
	i = -1;
	while (1)
	{
		++i;
		buf = (*line)[i];
		if (buf == '\0')
		{
			if (token)
				push_ftvector(&lexer->tokens, token);
			break ;
		}
		if (token && token->type == OPERATOR)
		{
			if (operator_part(buf, &token->str))
				continue ;
			push_ftvector(&lexer->tokens, token);
			token = 0;
		}
		if (operator_begin(buf))
		{
			if (token)
				push_ftvector(&lexer->tokens, token);
			token = (t_token *)malloc(sizeof(t_token));
			token->str = (char *)malloc(2);
			token->str[0] = buf;
			token->str[1] = 0;
			token->type = OPERATOR;
			continue ;
		}
		else if (buf == '\'' || buf == '\"')
		{
			if (!token)
			{
				token = (t_token *)malloc(sizeof(t_token));
				token->str = 0;
				token->type = WORD;
			}
			temp2 = handle_quotes(lexer, buf, line, &i);
			temp = ft_strjoin(token->str, temp2);
			free(token->str);
			free(temp2);
			token->str = temp;
		}
		else if (buf == ' ')
		{
			if (token)
			{
				push_ftvector(&lexer->tokens, token);
				token = (t_token *)malloc(sizeof(t_token));
				token->str = 0;
			}
		}
		else if (token && token->type == WORD)
		{
			temp = ft_chrjoin(token->str, buf);
			free(token->str);
			token->str = temp;
		}
		else
		{
			token = (t_token *)malloc(sizeof(t_token));
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
		ft_bzero(buf, sizeof(buf));
		if (read(0, buf, sizeof(buf)) <= 0)
			break ;
		write(1, buf, sizeof(buf));
		if (!ft_strcmp(buf, "\n"))
			break ;
		temp = ft_strjoin(*line, buf);
		free(*line);
		*line = temp;
	}
}

void	print_lexer_tokens(const t_lexer *lexer)
{
	int		i;

	if (!lexer)
		return ;
	i = 0;
	while (i < lexer->tokens.len)
	{
		ft_printf("tokenType = %d, token = %s\n", ((t_token *)lexer->tokens.elem[i])->type,
				((t_token *)lexer->tokens.elem[i])->str);
		++i;
	}
}

void	free_lexer(t_lexer *lexer)
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
	system("leaks 21sh");
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
		print_lexer_tokens(&lexer);
		free(line);
		free_lexer(&lexer);
	}
	free_double_arr(env);
	return (0);
}
