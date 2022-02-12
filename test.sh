cc -g -c test/main.c -o test/main.o
cc -o test/test test/main.o src/temperature.o src/pressure.o src/security.o src/i2c.o
test/test
rm test/main.o test/test
