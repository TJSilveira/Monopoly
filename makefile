CC = gcc
CFLAGS = -Wall

objects = Engine.o Functions.o


Engine: $(objects)
	$(CC) -o Engine $(objects) $(CFLAGS)

Engine.o: Engine.c Functions.h
	$(CC) $(CFLAGS) -c Engine.c -o Engine.o

Functions.o: Functions.c Functions.h
	$(CC) $(CFLAGS) -c Functions.c -o Functions.o
