CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: 3lab2pac.out

3lab2pac.out: main.o 3lab2pac.o
	$(CC) $(CFLAGS) main.o 3lab2pac.o -o 3lab2pac.out

main.o: main.c 3lab2pac.h
	$(CC) $(CFLAGS) -c main.c

3lab2pac.o: 3lab2pac.c 3lab2pac.h
	$(CC) $(CFLAGS) -c 3lab2pac.c

clean:
	rm -f *.o 3lab2pac.out
