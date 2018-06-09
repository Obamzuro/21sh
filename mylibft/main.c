/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/09 17:40:02 by obamzuro          #+#    #+#             */
/*   Updated: 2018/06/09 17:54:53 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int main()
{
	t_ftvector lal;
	t_ftvector kek;
	init_ftvector(&lal);
	init_ftvector(&kek);
	push_ftvector(&lal, "gkgk");
	push_ftvector(&lal, "asdfasdf");
	push_ftvector(&lal, "123123");
	push_ftvector(&lal, "[p[[p[");
	int		*a = malloc(2 * sizeof(int));
	int		*b = malloc(2 * sizeof(int));

	push_ftvector(&kek, a);
	push_ftvector(&kek, a + 1);
	push_ftvector(&kek, b);
	push_ftvector(&kek, b + 1);
	system("leaks a.out");
	return (0);
}
