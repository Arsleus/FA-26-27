CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: lab1.out

lab1.out: main.o lab1fa.o
	$(CC) $(CFLAGS) main.o lab1fa.o -o lab1fa.out

main.o: main.c lab1fa.h
	$(CC) $(CFLAGS) -c main.c

lab1fa.o: lab1fa.c lab1fa.h
	$(CC) $(CFLAGS) -c lab1fa.c

clean:
	rm -f *.o lab1fa.out
