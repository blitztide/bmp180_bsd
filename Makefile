PROG=bmp180
SRCS=src/main.c src/i2c.c src/temperature.c src/pressure.c src/altitude.c
CFLAGS=-O -g -lm -Wall -Wextra -Werror
MAN=doc/bmp180.1
.include <bsd.prog.mk>
