##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Makefile
##

GREEN=\033[1;32m
BLUE=\033[1;34m
RED=\033[1;31m
YELLOW=\033[1;33m
NC=\033[0m


MAIN 		= 	src/main.c

MAIN_OBJ 	= 	$(MAIN:.c=.o)

SRC 		=	src/params.c					\
				src/messages.c					\
				src/checkers.c					\
				src/server.c					\
				src/protocol.c					\
				src/client.c					\
				src/map_init.c					\
				src/buffer.c					\
				src/free.c						\
				src/algo.c						\
				src/accept.c

OBJ 		= 	$(SRC:.c=.o)

INCLUDES 	= 	-I./include

CFLAGS 		=	-Wall -Wextra -Werror

LIBS 		=

NAME 		= 	zappy_server

CC 			= 	gcc

CRIT 		= 	-lcriterion --coverage

all: $(NAME)

$(NAME): $(OBJ) $(MAIN_OBJ)
	@echo -e "$(BLUE)ᒥ🔧ᒧ Compiling $(NAME)...$(NC)"
	@$(CC) -o $(NAME) $(MAIN_OBJ) $(OBJ) $(CFLAGS) $(INCLUDES) $(LIBS)
	@echo -e "$(GREEN)ᒥ✅ᒧ Compilation completed successfully!$(NC)"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ)
	@rm -rf $(MAIN_OBJ)
	@rm -rf *.gc*

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(UNIT)
	@rm -f tests/*.c


re: fclean all

.PHONY: all clean fclean re
