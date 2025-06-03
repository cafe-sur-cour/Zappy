##
## EPITECH PROJECT, 2025
## zappy
## File description:
## Makefile
##

SERVER_NAME = zappy_server
GUI_NAME = zappy_gui
AI_NAME = zappy_ai

SERVER_DIR = server
GUI_DIR = gui
AI_DIR = ai

AI_TEST_SRC = $(shell cat tests/unit/ai/src.txt)

all: $(SERVER_NAME) $(GUI_NAME) $(AI_NAME)

$(SERVER_NAME):
	@echo "Compiling $(SERVER_NAME)..."
	@make -C $(SERVER_DIR)
	@ln -sf ./$(SERVER_DIR)/$(SERVER_NAME) $(SERVER_NAME)

$(GUI_NAME):
	@echo "Compiling $(GUI_NAME)..."
	@make -C $(GUI_DIR)
	@ln -sf ./$(GUI_DIR)/$(GUI_NAME) $(GUI_NAME)

$(AI_NAME):
	@echo "Compiling $(AI_NAME)..."
	@ln -sf ./$(AI_DIR)/$(AI_NAME) $(AI_NAME)

clean:
	@echo "Cleaning server..."
	@make -C $(SERVER_DIR) clean
	@make -C $(GUI_DIR) clean

fclean: clean
	@echo "Full clean server..."
	@make -C $(SERVER_DIR) fclean
	@make -C $(GUI_DIR) fclean
	@rm -f $(SERVER_NAME)
	@rm -f $(GUI_NAME)
	@rm -f $(AI_NAME)
	@rm -rf $(AI_DIR)/coverage_report
	-rm -rf *.log
	@make -C tests/unit/gui/ fclean
	@make -C tests/unit/server/ fclean


re: fclean all

functional_tests: $(GUI_NAME) $(SERVER_NAME) $(AI_NAME)
	@echo -e "$(BLUE)ᒥ🧪ᒧ Running functional tests...$(NC)"
	@python3 ./tests/functional/Tester.py

tests_run: $(GUI_NAME)
	@make tests_run_server
	@make tests_run_gui
	@make tests_run_ai

tests_run_ai: $(AI_NAME)
	@python3 -m pytest $(AI_TEST_SRC) -v

tests_run_gui:
	@make -C tests/unit/gui/ tests_run

tests_run_server:
	@make -C tests/unit/server/ tests_run

coverage: $(GUI_NAME)
	@make coverage_server
	@make coverage_gui
	@make coverage_ai

coverage_ai: $(AI_NAME)
	@echo "Compiling $(AI_NAME)..."
	@python3 -m pytest $(AI_TEST_SRC) \
	-v --cov=src --cov-report=term --cov-report=html:coverage_report
	@firefox ai/coverage_report/index.html &> /dev/null

coverage_gui:
	@make -C tests/unit/gui/ coverage

coverage_server:
	@make -C tests/unit/server/ coverage

.PHONY: all clean fclean re functional_tests tests_run coverage \
		tests_run_ai tests_run_gui tests_run_server \
		coverage_ai coverage_gui coverage_server \
		 $(SERVER_NAME) $(GUI_NAME) $(AI_NAME)
