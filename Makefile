PROG=bmp180
SRCS=src/main.c src/i2c.c src/temperature.c src/pressure.c src/altitude.c
CFLAGS=-O -g -lm
MAN=
.include <bsd.prog.mk>
