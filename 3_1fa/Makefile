CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Wconversion -Wsign-conversion
LDLIBS = -lm

all: 1lab3pac.out

1lab3pac.out: main.o 1lab3pac.o
	$(CC) $(CFLAGS) main.o 1lab3pac.o -o 1lab3pac.out $(LDLIBS)

main.o: main.c 1lab3pac.h
	$(CC) $(CFLAGS) -c main.c

1lab3pac.o: 1lab3pac.c 1lab3pac.h
	$(CC) $(CFLAGS) -c 1lab3pac.c

clean:
	rm -f *.o 1lab3pac.out

# valgrind --leak-check=full --show-leak-kinds=all и потом ./......out и тд