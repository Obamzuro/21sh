/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 13:19:59 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/02 13:20:07 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/21sh.h"
#include "include/libft.h"
#include "include/ft_printf.h"

int main(int argc, char **argv)
{
	int		ret;
	int		i;
	
	i = 0;
	while (i < 500000)
	{
		write(1, "woop", 4);
		++i;
		ft_fprintf(2, "%d\n", i);
	}
//	while (1)
//		;
}
