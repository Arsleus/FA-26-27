CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -Wconversion -Wsign-conversion

all: 8lab2pac.out

8lab2pac.out: main.o 8lab2pac.o
	$(CC) $(CFLAGS) main.o 8lab2pac.o -o 8lab2pac.out

main.o: main.c 8lab2pac.h
	$(CC) $(CFLAGS) -c main.c

8lab2pac.o: 8lab2pac.c 8lab2pac.h
	$(CC) $(CFLAGS) -c 8lab2pac.c

clean:
	rm -f *.o 8lab2pac.out

# valgrind --leak-check=full --show-leak-kinds=all и потом ./......out и тд