CC=gcc
CFLAGS=-std=c11 -g -Wall -pedantic -Wall -Wstringop-overflow=0

server: src/server.c src/helpers/helpers.c src/helpers/helpers.h src/socket/socket.c src/socket/socket.h src/list/list.c src/list/list.h src/command/command.c src/command/command.h src/middlewares/command-validator.h src/middlewares/command-validator.c
	$(CC) -o $@ src/server.c src/helpers/helpers.c src/socket/socket.c src/list/list.c src/command/command.c src/middlewares/command-validator.c $(CFLAGS)
