SRC_DIR = ./src
SRC= $(SRC_DIR)/main.c \
	$(SRC_DIR)/path.c \
	$(SRC_DIR)/processes.c \
	$(SRC_DIR)/utils.c \


OBJ = $(SRC:.c=.o)
CC = gcc
CFLAGS += -Wall -Wextra -Werror
NAME = pipex

LIB_DIR = ./lib
MYLIB42_DIR = $(LIB_DIR)/MYLIB42
MYLIB42 = $(MYLIB42_DIR)/mylib42.a

%.o: %.c 
	$(CC) $(CFLAGS) -c $(<) -o $(@)

all : $(NAME)

$(NAME) :  $(MYLIB42) $(OBJ)
	$(CC) -g $(CFLAGS) $(OBJ) $(MYLIB42) -o $(NAME)

$(MYLIB42_DIR) :
	git clone https://github.com/Nvnrs/MYLIB42.git $(MYLIB42_DIR)

$(MYLIB42) : $(MYLIB42_DIR)
	$(MAKE) -C $(MYLIB42_DIR)

clean :
	rm -rf $(OBJ)
	$(MAKE) clean -C $(MYLIB42_DIR)

fclean : clean
	rm -rf $(NAME)
	$(MAKE) fclean -C $(MYLIB42_DIR)

re : fclean all

PHONY : clean fclean re all