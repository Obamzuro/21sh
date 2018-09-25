/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creator_tokenfinders.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 13:07:37 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 13:07:38 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int					first_token_pos(t_lexer *lexer, int beg, int end,
		char **findname)
{
	int		i;
	int		j;

	i = beg - 1;
	while (++i <= end)
	{
		j = -1;
		while (findname[++j])
			if (ft_strequ(((t_token *)lexer->tokens.elem[i])->str,
					findname[j]) &&
					((t_token *)lexer->tokens.elem[i])->type == OPERATOR)
				return (i);
	}
	return (-1);
}

int					last_token_pos(t_lexer *lexer, int beg, int end,
		char **findname)
{
	int		i;
	int		j;

	i = end + 1;
	while (--i >= beg)
	{
		j = -1;
		while (findname[++j])
			if (ft_strequ(((t_token *)lexer->tokens.elem[i])->str,
						findname[j]) &&
					((t_token *)lexer->tokens.elem[i])->type == OPERATOR)
				return (i);
	}
	return (-1);
}
