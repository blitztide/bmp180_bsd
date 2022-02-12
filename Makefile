PACKAGE=runtime
PROG=	bmp180
BINDIR= /usr/local/bin
SRCS=src/main.c src/i2c.c src/temperature.c src/pressure.c src/altitude.c src/security.c
CFLAGS=-Wall -Wextra -Werror
LDFLAGS= -lm 
MAN=doc/bmp180.1
MK_DEBUG_FILES=no
.include <bsd.prog.mk>
