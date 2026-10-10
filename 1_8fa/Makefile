CC = gcc
CFLAGS = -std=c99 -Wall -Wextra

all: lab1_8.out

lab1_8.out: main.o lab1_8.o
	$(CC) $(CFLAGS) main.o lab1_8.o -o lab1_8.out

main.o: main.c lab1_8.h
	$(CC) $(CFLAGS) -c main.c

lab1_8.o: lab1_8.c lab1_8.h
	$(CC) $(CFLAGS) -c lab1_8.c

clean:
	rm -f *.o lab1_8.out
