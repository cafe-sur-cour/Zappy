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

MAIN 		= 	../../../server/src/main.c

MAIN_OBJ 	= 	$(MAIN:.c=.o)

SRC 		=	../../../server/src/params.c					\
				../../../server/src/messages.c					\
				../../../server/src/checkers.c					\
				../../../server/src/server.c					\
				../../../server/src/protocol.c					\
				../../../server/src/client.c


OBJ 		= 	$(SRC:.c=.o)

INCLUDES 	= 	-I./../../../server/include

CFLAGS 		=	-Wall -Wextra -Werror

LIBS 		=

NAME 		= 	zappy_server

CC 			= 	gcc

CRIT 		= 	-lcriterion --coverage

all: $(NAME)

$(NAME): $(OBJ) $(MAIN_OBJ)
	@echo -e "$(BLUE)ᒥ🔧ᒧ Compiling $(NAME)...$(NC)"
	$(CC) -o $(NAME) $(MAIN_OBJ) $(OBJ) $(CFLAGS) $(INCLUDES) $(LIBS)
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

unit_test: fclean
	@echo -e "$(YELLOW)ᒥ🧪ᒧ Compiling unit tests...$(NC)"
	@$(CC) -o $(UNIT) $(TESTS) $(SRC) $(CFLAGS) $(INCLUDES) $(CRIT)

tests_run: unit_test
	@echo -e "$(YELLOW)ᒥ🧪ᒧ Running unit tests...$(NC)"
	@./$(UNIT)

coverage: tests_run
	@echo -e "$(BLUE)ᒥ📊ᒧ Generating coverage report...$(NC)"
	@gcovr --exclude /tests

re: fclean all

.PHONY: all clean fclean re
