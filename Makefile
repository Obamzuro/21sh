# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/11 15:56:03 by obamzuro          #+#    #+#              #
#    Updated: 2018/06/09 19:37:10 by obamzuro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh

SRCNAME = main.c		\
		  msh_cd.c\
		  msh_echo.c\
		  msh_exec.c\
		  msh_exit.c\
		  msh_filler.c\
		  msh_free.c\
		  msh_get_printenv.c\
		  msh_print_pwd.c\
		  msh_setenv.c\
		  msh_signal_handler.c\
		  msh_unsetenv.c\
		  msh_replace_env_variable.c\
		  init_term.c\

FLAGS = -g -Wall -Wextra -Werror

SRC = $(addprefix source/, $(SRCNAME))

OBJ = $(SRC:.c=.o)

HDRDIR = include

HDR = include/minishell.h

all: $(NAME)

$(NAME): $(OBJ)
	make -C mylibft
	make -C ftprintf
	gcc $(FLAGS) -I $(HDRDIR) $(OBJ) -L mylibft -lft -L ftprintf -lftprintf -ltermcap -o $(NAME)

%.o: %.c $(HDR)
	gcc $(FLAGS) -I $(HDRDIR) -c $< -o $@

clean:
	make -C mylibft clean
	make -C ftprintf clean
	find . -name "*.o" -o -name ".*.sw[pon]" -exec rm -rf {} \;
	rm -rf $(OBJ)

fclean: clean
	make -C mylibft fclean
	make -C ftprintf fclean
	rm -rf $(NAME)

re: fclean all
