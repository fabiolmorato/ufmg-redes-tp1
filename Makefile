CC=gcc
CFLAGS=-std=c11 -g -Wall -pedantic -Wall -Wstringop-overflow=0

server: src/server.c src/helpers/helpers.c src/helpers/helpers.h src/socket/socket.c src/socket/socket.h src/list/list.c src/list/list.h src/command/command.c src/command/command.h
	$(CC) -o $@ src/server.c src/helpers/helpers.c src/socket/socket.c src/list/list.c src/command/command.c $(CFLAGS)
