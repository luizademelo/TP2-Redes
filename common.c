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

int server_sockaddr_init(const char *proto, struct sockaddr_storage *storage, int port)
{
    // identificação local
    memset(storage, 0, sizeof(storage));

    if (strcmp(proto, "ipv4") == 0)
    {
        struct sockaddr_in *server_address = (struct sockaddr_in *)storage;
        server_address->sin_family = AF_INET;
        server_address->sin_addr.s_addr = INADDR_ANY;
        server_address->sin_port = htons(port); // host to network short
    }
    else if (strcmp(proto, "ipv6") == 0)
    {
        struct sockaddr_in6 *server_address = (struct sockaddr_in6 *)storage;
        server_address->sin6_family = AF_INET6;
        server_address->sin6_addr = in6addr_any;
        server_address->sin6_port = htons(port); // host to network short
    }
    else
    {
        return -1;
    }
}