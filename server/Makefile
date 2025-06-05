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
				src/game/client.c				\
				src/game/game.c					\
				src/game/init_egg.c				\
				src/free.c						\
				src/algo.c						\
				src/accept.c					\
				src/guiMessage/gui_send_map_data.c	\
				src/guiMessage/gui_send_team_name.c	\
				src/guiMessage/gui_send_eggs.c		\
				src/guiMessage/gui_send_time.c

OBJ 		= 	$(SRC:.c=.o)

INCLUDES 	= 	-I./include

CFLAGS 		=	-Wall -Wextra -Werror -Wshadow

LIBS 		= -L./ -lmy -lnetwork

NAME 		= 	zappy_server

CC 			= 	gcc

CRIT 		= 	-lcriterion --coverage

all: libs $(NAME)


libs:
	$(MAKE) -C lib/my/
	$(MAKE) -C src/network/

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
	-rm libmy.a
	-rm libnetwork.a

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(UNIT)
	@rm -f tests/*.c
	-rm libmy.a
	-rm libnetwork.a

re: fclean all

.PHONY: all clean fclean re
