CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o bellman_ford.o
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

main.o: main.c bellman_ford.h
	$(CC) $(CFLAGS) -c main.c

bellman_ford.o: bellman_ford.c bellman_ford.h
	$(CC) $(CFLAGS) -c bellman_ford.c

clean:
	rm -f $(OBJ) $(EXEC) main.exe