CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: 1lab2pac.out

1lab2pac.out: main.o 1lab2pac.o
	$(CC) $(CFLAGS) main.o 1lab2pac.o -o 1lab2pac.out

main.o: main.c 1lab2pac.h
	$(CC) $(CFLAGS) -c main.c

1lab2pac.o: 1lab2pac.c 1lab2pac.h
	$(CC) $(CFLAGS) -c 1lab2pac.c

clean:
	rm -f *.o 1lab2pac.out
