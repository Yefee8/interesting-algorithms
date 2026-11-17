CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude

SRC = src/main.c src/list.c src/menu.c src/test_cases.c
OUT = genetic-algorithm

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
