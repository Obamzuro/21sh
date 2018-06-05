/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/02 14:48:42 by obamzuro          #+#    #+#             */
/*   Updated: 2018/05/23 12:16:42 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strjoin_gnl(int s1l, const char *s1, int s2l, const char *s2)
{
	char	*a;
	char	*b;
	size_t	i;

	i = s1l + s2l + 1;
	a = (char *)malloc(sizeof(char) * i);
	b = a;
	if (!a)
		return (NULL);
	if (s1)
		b = ft_strcpy_gnl(b, s1);
	if (s2)
		b = ft_strcpy_gnl(b, s2);
	return (a);
}

static int	read_line(const int fd, t_gnl_list *nd, char **line, int linelen)
{
	int		ret;
	char	*dop[2];

	while ((ret = read(fd, nd->buf, BUFF_SIZE)) > 0)
	{
		if ((dop[0] = ft_strchr(nd->buf, '\n')))
		{
			*dop[0] = 0;
			SMARTCHECK(dop[1] = ft_strjoin_gnl(linelen, *line,
						dop[0] - nd->buf, nd->buf));
			free(*line);
			*line = dop[1];
			nd->last = dop[0] - nd->buf + 1;
			return (1);
		}
		SMARTCHECK(dop[1] = ft_strjoin_gnl(linelen, *line, ret, nd->buf));
		free(*line);
		*line = dop[1];
		ft_memset(nd->buf, 0, BUFF_SIZE);
		linelen += ret;
	}
	if (ret == -1)
		return (-1);
	return (*line ? 1 : 0);
}

static int	handle_not_fd(const int fd, t_gnl_list *back, char **line)
{
	back->fd = fd;
	back->last = 0;
	ft_memset(back->buf, 0, BUFF_SIZE + 1);
	back->next = 0;
	return (read_line(fd, back, line, 0));
}

static int	handle_fd_exist(const int fd, t_gnl_list *fdnode, char **line)
{
	char	*slashn;

	if ((slashn = ft_strchr(fdnode->buf + fdnode->last, '\n')))
	{
		SMARTCHECK(*line = ft_strsub(fdnode->buf + fdnode->last,
				0, slashn - (fdnode->buf + fdnode->last)));
		fdnode->last = slashn - fdnode->buf + 1;
		return (1);
	}
	if (*(fdnode->buf + fdnode->last))
		SMARTCHECK(*line = ft_strdup(fdnode->buf + fdnode->last));
	ft_memset(fdnode->buf, 0, BUFF_SIZE);
	fdnode->last = 0;
	return (read_line(fd, fdnode, line, *line ? ft_strlen(*line) : 0));
}

int			get_next_line(const int fd, char **line)
{
	static t_gnl_list	*head;
	t_gnl_list			*node;

	if (fd < 0 || !line || BUFF_SIZE < 1)
		return (-1);
	*line = 0;
	if (!head)
	{
		SMARTCHECK(head = (t_gnl_list *)malloc(sizeof(t_gnl_list)));
		return (handle_not_fd(fd, head, line));
	}
	node = head;
	while (1)
	{
		if (node->fd == fd)
			return (handle_fd_exist(fd, node, line));
		if (!(node->next))
		{
			SMARTCHECK(node->next = (t_gnl_list *)malloc(sizeof(t_gnl_list)));
			node = node->next;
			return (handle_not_fd(fd, node, line));
		}
		node = node->next;
	}
}
