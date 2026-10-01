CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
OBJ    = main.o liste.o

ifeq ($(OS),Windows_NT)
EXE = .exe
endif

demo$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o demo $^

%.o: %.c liste.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) demo demo.exe

.PHONY: clean
