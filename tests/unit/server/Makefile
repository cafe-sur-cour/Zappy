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
				tests_map_init.c		\
				tests_free.c			\
				tests_guiMessage.c		\
				tests_client.c			\
				tests_network.c			\
				tests_aiMessage.c		\
				tests_messaggeReception.c

SRC 		=	../../../server/src/params.c					\
				../../../server/src/messages.c					\
				../../../server/src/checkers.c					\
				../../../server/src/server.c					\
				../../../server/src/game/player.c					\
				../../../server/src/protocol.c					\
				../../../server/src/game/client.c					\
				../../../server/src/accept.c					\
				../../../server/src/game/game.c						\
				../../../server/src/game/init_egg.c					\
				../../../server/src/algo.c						\
				../../../server/src/free.c						\
				../../../server/src/signal.c					\
				../../../server/src/network/accept_connection.c			\
				../../../server/src/network/buffer.c			\
				../../../server/src/network/close.c		\
				../../../server/src/network/create_server.c	\
				../../../server/src/network/hanlde_input.c		\
				../../../server/src/network/print_error.c		\
				../../../server/src/network/send_message.c		\
				../../../server/src/guiMessage/gui_send_map_data.c	\
				../../../server/src/guiMessage/gui_send_team_name.c	\
				../../../server/src/guiMessage/gui_send_eggs.c		\
				../../../server/src/guiMessage/gui_send_time.c		\
				../../../server/src/guiMessage/gui_send_egg_death.c \
				../../../server/src/guiMessage/gui_send_egg_connect.c	\
				../../../server/src/guiMessage/gui_send_player_info.c	\
				../../../server/src/guiMessage/gui_send_rest_info_player.c	\
				../../../server/src/guiMessage/gui_miscellaneous_send.c		\
				../../../server/src/guiMessage/gui_send_boradcast.c			\
				../../../server/src/guiMessage/gui_send_death_player.c		\
				../../../server/src/guiMessage/gui_send_ressource.c			\
				../../../server/src/receiveMessage/action_handler.c			\
				../../../server/src/receiveMessage/pollin_handler.c			\
				../../../server/src/receiveMessage/loop_thru_command.c		\
				../../../server/src/receiveMessage/queu_hanlder.c			\
				../../../server/src/aiMessage/forward.c


INCLUDES 	= 	-I./../../../server/include

CFLAGS 		=	-Wall -Wextra -Werror

LIBS 		=	-L. -lmy -lnetwork

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
