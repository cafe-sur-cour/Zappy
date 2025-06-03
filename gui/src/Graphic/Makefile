##
## EPITECH PROJECT, 2025
## B-YEP-400-NAN-4-1-zappy-albane.merian
## File description:
## Makefile
##

SRC	=	GUI.cpp \
		RayLib/RayLib.cpp

OBJ	=	$(SRC:.cpp=.o)

NAME	=	libzappy_graphic.a

CXXFLAGS	=	-I./ -I../Utils -I../../include -W -Wall -Wextra

all:	$(NAME)

$(NAME):	$(OBJ)
	ar rc $(NAME) $(OBJ)
	cp $(NAME) ../../build/graphic/

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)
	rm -f ../../build/graphic/$(NAME)

re:	fclean all

.PHONY:	all clean fclean re
