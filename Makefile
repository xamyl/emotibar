CC = gcc
CFLAGS = -Iinclude -Wall -O2
LIB_NAME = libemobar.a
SRC = src/emotibar.c
OBJ = $(SRC:.c=.o)

all: $(LIB_NAME) example

$(LIB_NAME): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

example: examples/main.c $(LIB_NAME)
	$(CC) $(CFLAGS) examples/main.c -L. -lemobar -o $@

clean:
	rm -f src/*.o $(LIB_NAME) example
