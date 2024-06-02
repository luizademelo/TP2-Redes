#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int sockaddr_init(const char *proto, struct sockaddr_storage *storage, int port);
void logexit(const char *msg);
