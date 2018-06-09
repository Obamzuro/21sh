/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vector.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/09 17:09:11 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/09 19:46:46 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		init_ftvector(t_ftvector *vec)
{
	if (!vec)
		return ;
	vec->capacity = 2;
	vec->len = 0;
	vec->elem = (void **)malloc(vec->capacity *
			sizeof(void *));
}

void		free_ftvector(t_ftvector *vec)
{
	int		i;

	i = -1;
	while (++i < vec->len)
		free(vec->elem[i]);
	free(vec->elem);
}

void		push_ftvector(t_ftvector *vec,
		void *line)
{
	void	**temp;
	int		i;

	if (!vec)
		return ;
	if (vec->capacity < vec->len + 1)
	{
		vec->capacity *= 2;
		temp = (void **)malloc(vec->capacity *
			sizeof(void *));
		i = -1;
		while (++i < vec->len)
			temp[i] = vec->elem[i];
		free(vec->elem);
		vec->elem = temp;
	}
	vec->elem[vec->len] = line;
	++vec->len;
}
