CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Wconversion -Wsign-conversion
LDLIBS = -lm

all: 9lab2pac.out

9lab2pac.out: main.o 9lab2pac.o
	$(CC) $(CFLAGS) main.o 9lab2pac.o -o 9lab2pac.out $(LDLIBS)

main.o: main.c 9lab2pac.h
	$(CC) $(CFLAGS) -c main.c

9lab2pac.o: 9lab2pac.c 9lab2pac.h
	$(CC) $(CFLAGS) -c 9lab2pac.c

clean:
	rm -f *.o 9lab2pac.out

# valgrind --leak-check=full --show-leak-kinds=all и потом ./......out и тд