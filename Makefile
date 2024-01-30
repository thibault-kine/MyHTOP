TARGET = my-htop.exe
CC = gcc
MAIN = ./main.c
SRCS = $(MAIN) src/get_proc.c
OBJS = $(SRCS:%.c=%.o)
CFLAGS = -Wall -Wextra -Werror -lncurses -g3

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) 

run: all
	./$(TARGET)

clean:
	rm $(OBJS)