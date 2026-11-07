CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = main
OBJS = main.o cobertura.o

ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    EXEC = main.exe
else
    RM = rm -f
    EXEC = ./main
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c cobertura.h
	$(CC) $(CFLAGS) -c main.c

cobertura.o: cobertura.c cobertura.h
	$(CC) $(CFLAGS) -c cobertura.c

clean:
	$(RM) *.o $(TARGET) $(TARGET).exe

run: $(TARGET)
	$(EXEC)

.PHONY: all clean run