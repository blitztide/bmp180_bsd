CC=cc
CFLAGS=-g -Wall
INC=inc


build/bmp180: src/main.c inc/bmp180.h
	${CC} ${CFLAGS} -I${INC} src/main.c src/i2c.c -o build/bmp180

clean:
	rm build/bmp180
