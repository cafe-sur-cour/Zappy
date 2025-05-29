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

fclean: clean
	@echo "Full clean server..."
	@make -C $(SERVER_DIR) fclean
	@rm -f $(SERVER_NAME)

re: fclean all

.PHONY: all clean fclean re
