# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/11 15:56:03 by obamzuro          #+#    #+#              #
#    Updated: 2018/09/01 20:27:52 by obamzuro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh

SRCNAME = main.c		\
		  init_term.c\
		  msh_filler.c\
		  msh_exec.c\
		  msh_free.c\
		  msh_get_printenv.c\
		  msh_cd.c\
		  msh_echo.c\
		  msh_exit.c\
		  msh_print_pwd.c\
		  msh_setenv.c\
		  msh_signal_handler.c\
		  msh_unsetenv.c\
		  msh_replace_env_variable.c

FLAGS = -g 

SRC = $(addprefix source/, $(SRCNAME))

OBJ = $(SRC:.c=.o)

HDRDIR = include	\
		 libft/include		\
		 ftprintf/include

HDR = include/21sh.h

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	make -C ftprintf
	gcc $(FLAGS) $(addprefix -I, $(HDRDIR)) $(OBJ) -L libft -lft -L ftprintf -lftprintf -ltermcap -o $(NAME)

%.o: %.c $(HDR)
	gcc $(FLAGS) $(addprefix -I, $(HDRDIR)) -c $< -o $@

clean:
	make -C libft clean
	make -C ftprintf clean
	find . -name "*.o" -o -name ".*.sw[pon]" -exec rm -rf {} \;
	rm -rf $(OBJ)

fclean: clean
	make -C libft fclean
	make -C ftprintf fclean
	rm -rf $(NAME)

re: fclean all
