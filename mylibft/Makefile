# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/20 11:09:03 by obamzuro          #+#    #+#              #
#    Updated: 2018/05/23 19:41:17 by obamzuro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCNAME =   ft_memset.c				\
			ft_bzero.c				\
			ft_memcpy.c				\
			ft_memccpy.c				\
			ft_memmove.c				\
			ft_memchr.c				\
			ft_memcmp.c				\
			ft_strlen.c				\
			ft_strdup.c				\
			ft_strcpy.c				\
			ft_strncpy.c				\
			ft_strcat.c				\
			ft_strncat.c				\
			ft_strlcat.c				\
			ft_strchr.c				\
			ft_strrchr.c				\
			ft_strstr.c				\
			ft_strnstr.c				\
			ft_strcmp.c				\
			ft_strncmp.c				\
			ft_atoi.c					\
			ft_isalpha.c				\
			ft_isdigit.c				\
			ft_isalnum.c				\
			ft_isascii.c				\
			ft_isprint.c				\
			ft_toupper.c				\
			ft_tolower.c				\
			ft_memalloc.c				\
			ft_memdel.c				\
			ft_strnew.c				\
			ft_strdel.c				\
			ft_strclr.c				\
			ft_striter.c				\
			ft_striteri.c				\
			ft_strmap.c				\
			ft_strmapi.c				\
			ft_strequ.c				\
			ft_strnequ.c				\
			ft_strsub.c				\
			ft_strjoin.c				\
			ft_strtrim.c				\
			ft_strsplit.c				\
			ft_itoa.c					\
			ft_putchar.c				\
			ft_putstr.c				\
			ft_putendl.c				\
			ft_putnbr.c				\
			ft_putchar_fd.c			\
			ft_putstr_fd.c			\
			ft_putendl_fd.c			\
			ft_putnbr_fd.c			\
			ft_lstnew.c				\
			ft_lstdelone.c			\
			ft_lstdel.c				\
			ft_lstadd.c				\
			ft_lstiter.c				\
			ft_lstmap.c				\
			ft_ccount.c				\
			ft_wcount.c				\
			ft_is_prime.c				\
			ft_list_size.c			\
			ft_list_last.c			\
			ft_strcpy_gnl.c			\
			get_next_line.c			\
			ft_nbr_size.c				\
			ft_swap.c				\
			ft_eol.c				

SRC    = $(addprefix src/, $(SRCNAME))
OBJECT = $(SRC:.c=.o)

HDIR   = include

all: $(NAME)

$(NAME): $(OBJECT)
	ar rc $(NAME) $(OBJECT)
	ranlib $(NAME)

%.o: %.c
	gcc -Wall -Wextra -Wall -o $@ -c $< -I $(HDIR)

clean:
	rm -f $(OBJECT)
	rm -f "*.swp"

fclean: clean
	rm -f $(NAME)

re: fclean all
