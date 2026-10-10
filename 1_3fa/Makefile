CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Wconversion -Wsign-conversion
LDLIBS = -lm

all: 3lab1pac.out

3lab1pac.out: main.o 3lab1pac.o
	$(CC) $(CFLAGS) main.o 3lab1pac.o -o 3lab1pac.out $(LDLIBS)

main.o: main.c 3lab1pac.h
	$(CC) $(CFLAGS) -c main.c

3lab1pac.o: 3lab1pac.c 3lab1pac.h
	$(CC) $(CFLAGS) -c 3lab1pac.c

clean:
	rm -f *.o 3lab1pac.out

# valgrind --leak-check=full --show-leak-kinds=all и потом ./......out и тд