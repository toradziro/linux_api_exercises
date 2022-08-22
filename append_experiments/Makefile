NAME = atm_append

SRCS =	atomic_append.c \
		../error_handler/error_handler.c \

INC_DIRS =	-I../error_handler \

OBJS = $(SRCS:.c=.o)

FLAGS = -Wall -Wextra -Werror

CC = gcc

.c.o:
	$(CC) $(FLAGS) $(INC_DIRS) -c $< -o ${<:.c=.o}

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "Built. Usage: atm_append filename num-bytes [x]"

all: $(NAME)

debug: FLAGS += -D_V_DEBUG_ 
debug: fclean $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "BUILT IN DEBUG MODE"

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

re_debug: fclean debug

.SILENT: