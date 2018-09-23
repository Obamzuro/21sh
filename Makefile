# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/11 15:56:03 by obamzuro          #+#    #+#              #
#    Updated: 2018/09/21 17:50:44 by obamzuro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh

SRCNAME = main.c		\
		  msh/init_term.c\
		  msh/msh_filler.c\
		  msh/msh_exec.c\
		  msh/msh_free.c\
		  msh/msh_get_printenv.c\
		  msh/msh_cd.c\
		  msh/msh_echo.c\
		  msh/msh_exit.c\
		  msh/msh_print_pwd.c\
		  msh/msh_setenv.c\
		  msh/msh_signal_handler.c\
		  msh/msh_unsetenv.c\
		  msh/msh_replace_env_variable.c\
		  ast_creator/creator.c\
		  lexer/lexer.c\
		  parser/parser.c\
		  error.c

FLAGS = -g -Wall -Wextra -Werror

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
