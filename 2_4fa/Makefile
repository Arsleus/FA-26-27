CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: 4lab2pac.out

4lab2pac.out: main.o 4lab2pac.o
	$(CC) $(CFLAGS) main.o 4lab2pac.o -o 4lab2pac.out

main.o: main.c 4lab2pac.h
	$(CC) $(CFLAGS) -c main.c

4lab2pac.o: 4lab2pac.c 4lab2pac.h
	$(CC) $(CFLAGS) -c 4lab2pac.c

clean:
	rm -f *.o 4lab2pac.out
