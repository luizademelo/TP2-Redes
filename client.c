#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include "common.h"

typedef struct
{
    int socket;
    const char **escolha; // Opção escolhida pelo cliente
    struct sockaddr_in address;
} client_info;

int option, nbytes;
char msg[100];
int port;
char *IPAddress;
char *proto;
// struct sockaddr_in address, server_address;
struct sockaddr_storage storage;
struct sockaddr_storage server_storage;
int address_size = sizeof(storage);
client_info client;
pthread_t tid;

void printMovieMenu()
{
    printf("-----------------------------------\n");
    printf("| $ 0 - Sair\n");
    printf("| $ 1 - Senhor dos aneis\n");
    printf("| $ 2 - O poderoso chefao\n");
    printf("| $ 3 - Clube da luta\n");
    printf("-----------------------------------\n");
}

// int client_sockaddr_init(char *proto, struct sockaddr_storage *storage)
// {
//     int port = 10000 + rand() % 50000;
//     // identificação local
//     memset(storage, 0, sizeof(storage));

//     if (strcmp(proto, "ipv4") == 0)
//     {
//         struct sockaddr_in *address = (struct sockaddr_in *)storage;
//         address->sin_family = AF_INET;
//         address->sin_addr.s_addr = INADDR_ANY;
//         address->sin_port = htons(port); // host to network short
//     }
//     else if (strcmp(proto, "ipv6") == 0)
//     {
//         struct sockaddr_in6 *address = (struct sockaddr_in6 *)storage;
//         address->sin6_family = AF_INET6;
//         address->sin6_addr = in6addr_any;
//         address->sin6_port = htons(port); // host to network short
//     }
//     else
//     {
//         return -1;
//     }
// }

void *client_thread(void *args)
{

    if (0 != bind(client.socket, (struct sockaddr *)&storage, sizeof(storage)))
    {
        logexit("bind");
    }

    do
    {
        printMovieMenu();
        scanf("%d", &option);

        sprintf(msg, "%d", option);

        nbytes = sendto(client.socket, msg, 100, 0, (struct sockaddr *)&server_storage, sizeof(server_storage));

        if (nbytes < 0)
        {
            logexit("sendto");
        }

        if (option == 0)
        {
            close(client.socket);
            exit(EXIT_SUCCESS);
        }

        int cnt = 0;

        while (cnt < 5)
        {
            nbytes = recvfrom(client.socket, msg, 100, 0, (struct sockaddr *)&server_storage, &address_size);

            if (nbytes < 0)
            {
                logexit("recvfrom");
            }

            printf("%s\n", msg);

            cnt++;
        }
    } while (option != 0);
}

int main(int argc, const char *argv[])
{

    port = atoi(argv[3]);
    IPAddress = argv[2];
    proto = argv[1];
    srand(time(NULL));
    int random = 10000 + rand() % 50000;

    sockaddr_init(proto, &storage, random);
    sockaddr_init(proto, &server_storage, port);

    client.socket = socket(storage.ss_family, SOCK_DGRAM, 0);
    if (client.socket < 0)
    {
        logexit("socket");
    }

    if (0 != pthread_create(&tid, NULL, client_thread, NULL))
    {
        logexit("pthread_create");
    }

    pthread_join(tid, NULL);

    close(client.socket);

    return 0;
}