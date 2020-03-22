CC=gcc
CFLAGS=-std=c18 -pedantic-errors -Werror=pedantic -Wall -Wextra -Werror -O3
DEPS=gcrypt.h
SRC=main.c gcrypt.c
OBJ=${SRC:%.c=%.o}

%.o:		%.c $(DEPS)
			$(CC) -c -o $@ $< $(CFLAGS)

gcrypt:		$(OBJ)
			$(CC) -o $@ $^

win32:		$(DEPS) $(SRC)
			i686-w64-mingw32-gcc $(CFLAGS) -static -static-libgcc -o gcrypt32.exe $(SRC)

win64:		$(DEPS) $(SRC)
			x86_64-w64-mingw32-gcc $(CFLAGS) -static -static-libgcc -o gcrypt64.exe $(SRC)

.PHONY:		clean

clean:
			rm $(OBJ) gcrypt
