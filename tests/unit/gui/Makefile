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

SRC =	../../../gui/src/Client/Client.cpp \
		../../../gui/src/Client/MsgHandler.cpp \
		../../../gui/src/CLI/CLI.cpp \
		../../../gui/src/Communication/Communication.cpp \
		../../../gui/src/Game/GameInfos.cpp \
		../../../gui/src/Graphic/RayLib/RayLib.cpp \
		../../../gui/src/Graphic/GUI.cpp \
		../../../gui/src/Graphic/Map.cpp \

OBJ = $(SRC:.cpp=.o)

TEST_SRC = main_test.cpp \
		   CLI/CLI_test.cpp \
		   Exceptions/Exceptions_test.cpp \
		   Client/Client_test.cpp \
		   Communication/Communication_test.cpp \

TEST_OBJ = $(TEST_SRC:.cpp=.o)

INCLUDES = -I./include

CFLAGS =	-std=c++20 -Wall -Wextra -Werror
CFLAGS_COVERAGE = $(CFLAGS) --coverage -fprofile-arcs -ftest-coverage

LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

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

coverage: tests_run
	@echo -e "$(BLUE)ᒥ📊ᒧ Generating coverage report...$(NC)"
	@mkdir -p coverage_report
	@gcovr --root . --exclude tests/ --exclude-unreachable-branches \
	--merge-mode-functions=merge-use-line-min \
	--html --html-details -o coverage_report/index.html
	@echo -e "$(GREEN)ᒥ✅ᒧ Report generated in coverage_report/index.html$(NC)"
	@firefox coverage_report/index.html &> /dev/null || xdg-open \
		coverage_report/index.html &> /dev/null || echo "Please open \
		coverage_report/index.html in your browser"

tests_run: $(SRC) $(TEST_SRC)
	@echo -e "$(BLUE)ᒥ🧪ᒧ Compiling Google Test unit tests...$(NC)"
	$(CC) -o $(UNIT_TESTS) $(SRC) $(TEST_SRC) $(CFLAGS_COVERAGE) $(INCLUDES) \
		$(LIBS) -lgtest -lgtest_main
	@echo -e "$(GREEN)ᒥ✅ᒧ Unit tests compiled! Running...$(NC)"
	./$(UNIT_TESTS)
	@echo -e "$(GREEN)ᒥ✅ᒧ All tests completed!$(NC)"

.PHONY: all clean fclean re
