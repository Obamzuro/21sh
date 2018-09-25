/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/09 16:36:46 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 14:34:51 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_swap_pointers(void **ptr1, void **ptr2)
{
	void	*temp;

	temp = *ptr2;
	*ptr2 = *ptr1;
	*ptr1 = temp;
}

void	ft_iswap(int *a, int *b)
{
	int	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}
