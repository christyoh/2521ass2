CC=gcc
CFLAGS= -gdwarf-2 -Wall -Werror -std=gnu99 -O -lm

all: pagerank

pagerank: pagerank.o readData.o Graph.o list.o
	$(CC) -o pagerank pagerank.o readData.o Graph.o list.o

pagerank.o: pagerank.c pagerank.h readData.h Graph.h list.h
	$(CC) -c $(CFLAGS) pagerank.c

readData.o: readData.c readData.h Graph.h list.h
	$(CC) -c $(CFLAGS) readData.c

Graph.o: Graph.c Graph.h list.h
	$(CC) -c $(CFLAGS) Graph.c

list.o: list.c list.h
	$(CC) -c $(CFLAGS) list.c

clean:
	rm -f testReadData testReadData.o readData.o Graph.o list.o
