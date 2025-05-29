##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile Tests
##

CC 		= 	gcc

UNIT 	= 	unit_tests

TESTS 	= 	unit/tests_params_server.c

SRC 	= 	../server/src/params.c			\
			../server/src/errors.c

CFLAGS 	= 	-Wall -Wextra -Werror -I ../server/include

CRIT 	= 	--coverage -lcriterion

GREEN		= 	\033[1;32m
RESET		= 	\033[0m

unit_test:
	@$(CC) $(CFLAGS) -o $(UNIT) $(SRC) $(TESTS) $(CRIT)
	@echo "$(GREEN)Unit tests compiled$(RESET)"

tests_run: unit_test
	@./$(UNIT)
	@echo "$(GREEN)Unit tests executed$(RESET)"

coverage: tests_run
	@gcovr --exclude unit/
	@gcovr --exclude unit/ --branch
	@echo "$(GREEN)Coverage report generated$(RESET)"

clean:
	@rm -f $(UNIT)
	@rm -f *.gcda *.gcno
