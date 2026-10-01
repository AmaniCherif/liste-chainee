CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
OBJ    = main.o liste.o test.o

ifeq ($(OS),Windows_NT)
EXE = .exe
endif

demo$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o demo $^

%.o: %.c liste.h test.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) demo demo.exe

test: demo$(EXE)
	valgrind --leak-check=full --error-exitcode=1 ./demo

.PHONY: clean test
