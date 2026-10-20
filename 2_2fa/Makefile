CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: 2lab2pac.out

2lab2pac.out: main.o 2lab2pac.o
	$(CC) $(CFLAGS) main.o 2lab2pac.o -o 2lab2pac.out

main.o: main.c 2lab2pac.h
	$(CC) $(CFLAGS) -c main.c

2lab2pac.o: 2lab2pac.c 2lab2pac.h
	$(CC) $(CFLAGS) -c 2lab2pac.c

clean:
	rm -f *.o 2lab2pac.out
