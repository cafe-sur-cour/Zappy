##
## EPITECH PROJECT, 2025
## zappy
## File description:
## GUI Makefile
##

# ANSI Colors
GREEN=\033[1;32m
BLUE=\033[1;34m
RED=\033[1;31m
YELLOW=\033[1;33m
NC=\033[0m

MAIN = src/main.cpp

MAIN_OBJ = $(MAIN:.cpp=.o)

SRC =	src/Client/Client.cpp \
		src/Client/MsgHandler.cpp \
		src/CLI/CLI.cpp \
		src/Communication/Communication.cpp \
		src/Game/GameInfos.cpp \

OBJ = $(SRC:.cpp=.o)

TEST_SRC = ../tests/unit/gui/main_test.cpp \
		   ../tests/unit/gui/CLI/CLI_test.cpp \
		   ../tests/unit/gui/Exceptions/Exceptions_test.cpp \
		   ../tests/unit/gui/Client/Client_test.cpp \
		   ../tests/unit/gui/Communication/Communication_test.cpp \
		   ../tests/unit/gui/Game/GameInfos_test.cpp \

TEST_OBJ = $(TEST_SRC:.cpp=.o)

INCLUDES = -I./include

CFLAGS =	-std=c++20 -Wall -Wextra -Werror
CFLAGS_COVERAGE = $(CFLAGS) --coverage -fprofile-arcs -ftest-coverage

LIBS =

NAME = zappy_gui

UNIT_TESTS = unit_tests

CC = g++

all: $(NAME)

$(NAME): $(OBJ) $(MAIN_OBJ)
	@echo -e "$(BLUE)ᒥ🔧ᒧ Compiling $(NAME)...$(NC)"
	$(CC) -o $(NAME) $(MAIN_OBJ) $(OBJ) $(CFLAGS) $(INCLUDES) $(LIBS)
	@echo -e "$(GREEN)ᒥ✅ᒧ Compilation completed successfully!$(NC)"

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ)
	@rm -rf $(MAIN_OBJ)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(UNIT_TESTS)
	@rm -rf coverage_report/
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.gcov

re: fclean all

.PHONY: all clean fclean re
