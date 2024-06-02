all: 
	gcc -Wall -c common.c -o common.o -lm
	gcc -Wall server.c ./common.o -o server
	gcc -Wall client.c ./common.o -o client