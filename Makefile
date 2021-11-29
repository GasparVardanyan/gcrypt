CFLAGS=-std=c18 -pedantic-errors -Werror=pedantic -O3
LIBS=-lbsd # TODO: find a portable way to enable/disable this flag
DEPS=header.h gcrypt.h options.h readopts.h
SRC=gcrypt.c main.c options.c
OBJ=${SRC:%.c=%.o}
PREFIX = /usr/local

%.o:		%.c $(DEPS)
			$(CC) -c -o $@ $< $(CFLAGS)

gcrypt:		$(OBJ)
			$(CC) -o $@ $^ $(LIBS)

# win32:		$(DEPS) $(SRC)
#             i686-w64-mingw32-gcc $(CFLAGS) -static -static-libgcc -o gcrypt32.exe $(SRC)
#
# win64:		$(DEPS) $(SRC)
#             x86_64-w64-mingw32-gcc $(CFLAGS) -static -static-libgcc -o gcrypt64.exe $(SRC)

clean:
			rm -f $(OBJ) gcrypt grypt*.exe

install:	gcrypt
			mkdir -p ${DESTDIR}${PREFIX}/bin
			cp -f gcrypt ${DESTDIR}${PREFIX}/bin
			chmod 755 ${DESTDIR}${PREFIX}/bin/gcrypt

uninstall:
			rm -f ${DESTDIR}${PREFIX}/bin/gcrypt

.PHONY:		gcrypt clean install uninstall
