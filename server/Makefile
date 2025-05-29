##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Makefile
##

NAME 	= 	zappy_server

SRC 	= 	src/main.c						\
	  		src/params.c					\
			src/errors.c

OBJ 	= 	$(SRC:.c=.o)

CFLAGS 	= 	-Wall -Wextra -Werror -I include

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
