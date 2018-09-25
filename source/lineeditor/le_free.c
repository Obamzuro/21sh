/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:06:28 by obamzuro          #+#    #+#             */
/*   Updated: 2018/09/24 14:06:29 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void				free_lineeditor(t_lineeditor *lineeditor)
{
	free(lineeditor->buffer);
	free(lineeditor->cpbuf);
}
