#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "common.h"

void logexit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int sockaddr_init(const char *proto, struct sockaddr_storage *storage, int port)
{
    // identificação local
    memset(storage, 0, sizeof(storage));

    if (strcmp(proto, "ipv4") == 0)
    {
        struct sockaddr_in *address = (struct sockaddr_in *)storage;
        address->sin_family = AF_INET;
        address->sin_addr.s_addr = INADDR_ANY;
        address->sin_port = htons(port); // host to network short
    }
    else if (strcmp(proto, "ipv6") == 0)
    {
        struct in6_addr inaddr6;
        struct sockaddr_in6 *address = (struct sockaddr_in6 *)storage;
        address->sin6_family = AF_INET6;
        address->sin6_port = htons(port); // host to network short
        address->sin6_addr = in6addr_any;
        // memcpy(&(address->sin6_addr), &inaddr6, sizeof(inaddr6));
    }
    else
    {
        return -1;
    }
}
