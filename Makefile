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

all: $(SERVER_NAME)

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
	@make -C tests/ clean

fclean: clean
	@echo "Full clean server..."
	@make -C $(SERVER_DIR) fclean
	@rm -f $(SERVER_NAME)

unit_test:
	@echo "Compiling unit tests..."
	@make -C tests unit_test

tests_run:
	@echo "Running unit tests..."
	@make -C tests tests_run

coverage:
	@echo "Generating coverage report..."
	@make -C tests coverage

re: fclean all

functional_tests: $(GUI_NAME) $(SERVER_NAME) $(AI_NAME)
	@echo -e "$(BLUE)ᒥ🧪ᒧ Running functional tests...$(NC)"
	@python3 ./tests/functional/Tester.py

tests_run: $(GUI_NAME)
	@make -C $(GUI_DIR) tests_run

coverage: $(GUI_NAME)
	@make -C $(GUI_DIR) coverage

.PHONY: all clean fclean re functional_tests tests_run coverage\
		 $(SERVER_NAME) $(GUI_NAME) $(AI_NAME)
