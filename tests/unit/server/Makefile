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

UNIT 	    = 	unit_tests

TESTS 	    =   tests_params.c			\
				tests_errors.c			\
				tests_checkers.c		\
				tests_server.c			\
				test_map_init.c

SRC 		=	../../../server/src/params.c					\
				../../../server/src/messages.c					\
				../../../server/src/checkers.c					\
				../../../server/src/server.c					\
				../../../server/src/protocol.c					\
				../../../server/src/client.c					\
				../../../server/src/accept.c					\
				../../../server/src/map_init.c					\
				../../../server/src/algo.c						\
				../../../server/src/buffer.c					\
				../../../server/src/free.c						\
				../../../server/src/guiMessage/gui_send_map_data.c	\
				../../../server/src/guiMessage/gui_send_team_name.c



INCLUDES 	= 	-I./../../../server/include

CFLAGS 		=	-Wall -Wextra -Werror

LIBS 		=	-L./ -lmy

CC 			= 	gcc

CRIT 		= 	-lcriterion --coverage


clean:
	@rm -rf $(OBJ)
	@rm -rf $(MAIN_OBJ)
	@rm -rf *.gc*

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(UNIT)

unit_test: fclean
	@echo -e "$(YELLOW)ᒥ🧪ᒧ Compiling unit tests...$(NC)"
	@$(CC) -o $(UNIT) $(SRC)  $(TESTS)  $(CFLAGS) $(INCLUDES) $(CRIT) $(LIBS)

tests_run: unit_test
	@echo -e "$(YELLOW)ᒥ🧪ᒧ Running unit tests...$(NC)"
	@./$(UNIT)

coverage: tests_run
	@echo -e "$(BLUE)ᒥ📊ᒧ Generating coverage report...$(NC)"
	@gcovr --root ../../../ --exclude tests/* --exclude-unreachable-branches

re: fclean all

.PHONY: all clean fclean re
