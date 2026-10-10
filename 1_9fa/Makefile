CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Wconversion -Wsign-conversion
LDLIBS = -lm

all: 9lab1pac.out

9lab1pac.out: main.o 9lab1pac.o
	$(CC) $(CFLAGS) main.o 9lab1pac.o -o 9lab1pac.out $(LDLIBS)

main.o: main.c 9lab1pac.h
	$(CC) $(CFLAGS) -c main.c

9lab1pac.o: 9lab1pac.c 9lab1pac.h
	$(CC) $(CFLAGS) -c 9lab1pac.c

clean:
	rm -f *.o 9lab1pac.out

# valgrind --leak-check=full --show-leak-kinds=all и потом ./......out и тд