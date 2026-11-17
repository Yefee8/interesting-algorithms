CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude

SRC = src/main.c src/components/list.c src/components/menu.c src/components/test_cases.c
OUT = genetic-algorithm

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
