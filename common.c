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

int server_sockaddr_init(const char *proto, struct sockaddr_in *server_address, int port)
{
    // identificação local
    memset(server_address, 0, sizeof(server_address));

    if (strcmp(proto, "ipv4") == 0)
    {
        server_address->sin_family = AF_INET;
        server_address->sin_addr.s_addr = INADDR_ANY;
        server_address->sin_port = htons(port);
    }
    else if (strcmp(proto, "ipv6") == 0)
    {
        // server_address = (struct sockaddr_in6)server_address;
        // server_address.sin_family = AF_INET6;
        // server_address.sin_addr.s_addr = INADDR_ANY;
        // server
    }
    else
    {
        return -1;
    }
}