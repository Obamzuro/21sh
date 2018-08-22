/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/21 12:54:23 by obamzuro          #+#    #+#             */
/*   Updated: 2018/08/21 14:42:55 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/21sh.h"

int main(int argc, char **argv)
{
	int		ret;
	char	line[11];

	printf("in\n");
	while (1)
	{
		bzero(line, 11);
		ret = read(0, line, 10);
		printf("%11s | %d\n", line, ret);
		if (ret < 1)
			break ;
	}
}
