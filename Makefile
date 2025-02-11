SRC_DIR = ./src
SRC= $(SRC_DIR)/main.c \


OBJ = $(SRC:.c=.o)
# CFLAGS += -Wall -Wextra -Werror
NAME = pipex

LIB_DIR = ./lib
MYLIB42_DIR = $(LIB_DIR)/MYLIB42
MYLIB42 = $(MYLIB42_DIR)/mylib42.a

all : $(NAME)

$(NAME) :  $(MYLIB42) $(OBJ)
	gcc -g $(CFLAGS) $(OBJ) $(MYLIB42) -o $(NAME)

$(MYLIB42_DIR) :
	git clone https://github.com/Nvnrs/MYLIB42.git $(MYLIB42_DIR)

$(MYLIB42) : $(MYLIB42_DIR)
	$(MAKE) -C $(MYLIB42_DIR)

clean :
	rm -rf $(OBJ)
	$(MAKE) clean -C $(MYLIB42_DIR)

fclean : clean
	rm -rf $(NAME)
	rm -rf $(LIB_DIR)

re : fclean all

PHONY : clean fclean re all