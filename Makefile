TARGET = myhtop.exe
CC = gcc
MAIN = ./main.c
SRCS = $(MAIN)
OBJS = $(SRCS:%.c=%.o)
CFLAGS = -Wall -Wextra -Werror -lncurses

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) 

run: all
	./$(TARGET)

clean:
	rm $(OBJS)