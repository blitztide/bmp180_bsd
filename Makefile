PROG=bmp180
SRCS=src/main.c src/i2c.c src/temperature.c src/pressure.c
CFLAGS=-O -g
MAN=
.include <bsd.prog.mk>
