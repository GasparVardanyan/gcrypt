CC=gcc
CFLAGS=-std=c18 -pedantic-errors -Werror=pedantic -Wall -Wextra -Werror -O3
DEPS=gcrypt.h
SRC=main.c gcrypt.c
OBJ=${SRC:%.c=%.o}
PREFIX = /usr/local

%.o:		%.c $(DEPS)
			$(CC) -c -o $@ $< $(CFLAGS)

gcrypt:		$(OBJ)
			$(CC) -o $@ $^

# win32:		$(DEPS) $(SRC)
#             i686-w64-mingw32-gcc $(CFLAGS) -static -static-libgcc -o gcrypt32.exe $(SRC)
#
# win64:		$(DEPS) $(SRC)
#             x86_64-w64-mingw32-gcc $(CFLAGS) -static -static-libgcc -o gcrypt64.exe $(SRC)

clean:
			rm -f $(OBJ) gcrypt

install:	gcrypt
			mkdir -p ${DESTDIR}${PREFIX}/bin
			cp -f gcrypt ${DESTDIR}${PREFIX}/bin
			chmod 755 ${DESTDIR}${PREFIX}/bin/gcrypt

uninstall:
			rm -f ${DESTDIR}${PREFIX}/bin/gcrypt

.PHONY:		gcrypt clean install uninstall
