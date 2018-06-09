/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/09 20:43:13 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int					identify_operator(char **operators,
		char buf[8])
{
	int		i;

	i = -1;
	if (buf[1])
		return (0);
	while (++i < AM_OPERATORS)
	{
		if (buf[0] == operators[i][0])
		{
			return (1);
		}
	}
	return (0);
}

int					append_operator(char **operators,
		char buf[8], char **token)
{
	int		i;
	char	*joining;
	int		tokenlen;

	if (buf[1])
		return (0);
	tokenlen = ft_strlen(*token);
	joining = ft_strjoin(*token, buf);
	i = -1;
	while (++i < AM_OPERATORS)
	{
		if (ft_strnstr(operators[i], joining, tokenlen + 1))
		{
			free(*token);
			*token = joining;
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

void				line_handling(char **operators)
{
	int			wasoperator;
	char		buf[8];
	char		*token;
	char		*temp;
	t_ftvector	line;

	token = 0;
	wasoperator = -1;
	init_ftvector(&line);
	while (1)
	{
		ft_bzero(buf, sizeof(buf));
		read(0, buf, sizeof(buf));
		if (wasoperator == 1)
		{
			if (append_operator(operators, buf, &token))
			{
				write(1, buf, sizeof(buf));
				continue ;
			}
			else if (token)
			{
				push_ftvector(&line, token);
				token = 0;
			}
		}
		if (identify_operator(operators, buf))
		{
			if (token)
				push_ftvector(&line, token);
			token = ft_strdup(buf);
			wasoperator = 1;
			write(1, buf, sizeof(buf));
			continue ;
		}
		else if (!ft_strcmp("\n", buf))
		{
			if (token)
			{
				push_ftvector(&line, token);
				token = 0;
			}
			write(1, buf, sizeof(buf));
			break ;
		}
		else if (!buf[1] && ft_strchr(" \t", buf[0]))
		{
			if (token)
			{
				push_ftvector(&line, token);
				token = 0;
			}
		}
		else if (!wasoperator)
		{
			temp = ft_strjoin(token, buf);
			free(token);
			token = temp;
		}
		else
			token = ft_strdup(buf);
		wasoperator = 0;
		write(1, buf, sizeof(buf));
	}
	push_ftvector(&line, 0);
	tilde_expansion(&line);
	int i = 0;
	ft_printf("\n");
	while (line.elem[i])
	{
		ft_printf("%s ", line.elem[i]);
		++i;
	}
	ft_printf("\n");
}

int					main(void)
{
//	char		*line;
//	int			i;
	char		**env;
	t_comm_corr	commands[AM_COMMANDS];
	char		**operators;
//	char		**args;

	g_sigint = 0;
	signal(SIGINT, int_handler);
	fill_commands(commands);
	operators = init_operators();
	env = fill_env();
	term_associate();
	set_noncanon();
	while (1)
	{
		ft_printf("$> ");
		g_sigint = 0;
		line_handling(operators);
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
