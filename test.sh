cc -g -c test/main.c -o test/main.o
cc -o test/test test/main.o src/temperature.o src/pressure.o
