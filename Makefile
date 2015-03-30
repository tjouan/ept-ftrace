NAME	=	ftrace
SRCS	=	main.c		\
		env.c		\
		std.c		\
		trace.c		\
		info.c		\
		elf.c		\
		list.c		\
		stabs.c		\
		syscall.c
OBJS	=	$(SRCS:.c=.o)
CFLAGS	=	-Wall -ggdb -I./include
RM	=	rm -f

$(NAME)	: $(OBJS)
	$(CC) -o $(NAME) $(LIBS) $(OBJS)

all	: $(NAME)

clean	:
	$(RM) $(OBJS)

fclean	: clean
	$(RM) $(NAME)
