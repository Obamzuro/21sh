/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2018/08/10 16:36:53 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "21sh.h"

//static void			handle_commands(char *line,
//		t_comm_corr commands[AM_COMMANDS],
//		char ***env)
//{
//	int		i;
//	char	**args;
//
//	if (!(args = ft_strsplit2(line, " \t"))[0])
//	{
//		free_double_arr(args);
//		return ;
//	}
//	replace_env_variable(args, *env);
//	i = -1;
//	while (++i < AM_COMMANDS)
//	{
//		if (!ft_strncmp(commands[i].comm, args[0],
//					ft_strlen(commands[i].comm) + 1))
//		{
//			commands[i].func(args, env);
//			break ;
//		}
//	}
//	if (i == AM_COMMANDS)
//		ft_exec(args, env);
//	free_double_arr(args);
//}

char				*input_command(void)
{
	char		buf[8];
	char		*command;
	char		*temp;

	command = 0;
	while (1)
	{
		ft_bzero(buf, sizeof(buf));
		read(0, buf, sizeof(buf));
		if (!buf[1] && buf[0] == '\n')
		{
			write(1, buf, sizeof(buf));
			break ;
		}
		temp = command;
		command = ft_strjoin(command, buf);
		free(temp);
		write(1, buf, sizeof(buf));
	}
	return (command);
}

int					identify_operator(char buf)
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

int					try_append_operator(char buf, char **tokenstr)
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

//char				*read_until(t_lexer *lexer, char delim)
//{
//	char buf[8];
//	char *ret;
//	char *temp;
//
//	ret = 0;
//	while (1)
//	{
//		ft_bzero(buf, sizeof(buf));
//		read(0, buf, sizeof(buf));
//		if (!ft_strcmp(buf, delim))
//			break ;
//		temp = ret;
//		ret = ft_strjoin(ret, buf);
//		free(temp);
//		write(1, buf, sizeof(buf));
//	}
//	write(1, buf, sizeof(buf));
//	return (ret);
//}

void				lexer_creating(char *command, t_lexer *lexer)
{
	char		*temp;
	t_token		*token;
	char		buf;
	int			j;

	token = (t_token *)ft_memalloc(sizeof(t_token));
	init_ftvector(&lexer->tokens);
	j = -1;
	while (1)
	{
		++j;
		buf = command[j];
		if (token->type == OPERATOR)
		{
			if (try_append_operator(buf, &token->str))
				continue ;
			if (token->str)
			{
				push_ftvector(&lexer->tokens, token);
				token = (t_token *)ft_memalloc(sizeof(t_token));
			}
		}
		if (identify_operator(buf))
		{
			if (token->str)
			{
				push_ftvector(&lexer->tokens, token);
				token = (t_token *)malloc(sizeof(t_token));
			}
			token->str = (char *)ft_memalloc(2);
			token->str[0] = buf;
			token->type = OPERATOR;
			continue ;
		}
		else if ('\"' == buf || '\'' == buf)
		{
			char	delim;
			delim = buf;
			while (1)
			{
				++j;
				buf = command[j];
				if (buf == delim)
					break ;
				if (!buf)
				{
					free(command);
					command = input_command();
					j = -1;
					continue ;
				}
				temp = ft_chrjoin(token->str, buf);
				free(token->str);
				token->str = temp;
			}
		}
		else if (!buf)
		{
			if (token->str)
				push_ftvector(&lexer->tokens, token);
			break ;
		}
		else if (ft_strchr(" \t", buf))
		{
			if (token->str)
			{
				push_ftvector(&lexer->tokens, token);
				token = (t_token *)ft_memalloc(sizeof(t_token));
			}
		}
		else if (token->type != OPERATOR)
		{
			temp = ft_chrjoin(token->str, buf);
			free(token->str);
			token->str = temp;
		}
		else
		{
			token->str = (char *)ft_memalloc(2);
			token->str[0] = buf;
			token->type = WORD;
		}
	}
	//lexer_creating_mbmore(lexer);
	//tilde_expansion(&lexer->tokens);
	int i = 0;
	ft_printf("\n");
	while (i < lexer->tokens.len)
	{
		ft_printf("%s\n", ((t_token *)lexer->tokens.elem[i])->str);
		++i;
	}
	ft_printf("\n");
	system("leaks -quiet 21sh");
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

int					main(void)
{
//	char		*line;
//	int			i;
	char		**env;
	t_comm_corr	commands[AM_COMMANDS];
	char		*command;
	t_lexer		lexer;
//	char		**args;

	g_sigint = 0;
	//signal(SIGINT, int_handler);
	//fill_commands(commands);
	env = fill_env();
	term_associate();
	set_noncanon();
	while (1)
	{
		ft_bzero(&lexer, sizeof(t_lexer));
		ft_printf("$> ");
		g_sigint = 0;
		command = input_command();
		lexer_creating(command, &lexer);
		free_lexer(&lexer);
//		get_next_line(0, &line);
//		g_sigint = 1;
//		args = ft_strsplit(line, ';');
//		i = -1;
//		while (args[++i])
//			handle_commands(args[i], commands, &env);
//		free_double_arr(args);
//		free(line);
	}
	free_double_arr(env);
	return (0);
}
