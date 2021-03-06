#BMPcoder 

CC = gcc -ansi -pedantic -Wall -Wextra -std=c99 -g

BMPcoder: bitModul.o bmpFileParser.o BMPcoder.o
	$(CC) -o BMPcoder bitModul.o bmpFileParser.o BMPcoder.o

bitModul.o: bitModul.c bitModul.h
	$(CC) -c bitModul.c

bmpFileParser.o: bmpFileParser.c bmpFileParser.h
	$(CC) -c  bmpFileParser.c

BMPcoder.o: BMPcoder.c
	$(CC) -c BMPcoder.c