PACKAGE=runtime
PROG=	bmp180
BINDIR= /usr/local/bin
SRCS=src/main.c src/i2c.c src/temperature.c src/pressure.c src/altitude.c
CFLAGS=-Wall -Wextra -Werror
LIBADD=m
MAN=doc/bmp180.1
MK_DEBUG_FILES=no
.include <bsd.prog.mk>
