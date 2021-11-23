CC=gcc
CFLAGS=-std=c11 -Wall -Werror -pedantic -O3

server: src/server.c src/helpers/helpers.c src/helpers/helpers.h
	$(CC) -o $@ src/server.c src/helpers/helpers.c $(CFLAGS)
