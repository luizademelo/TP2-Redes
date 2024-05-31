#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct
{
    int socket;
    int escolha; // Opção escolhida pelo cliente
    int frase;   // Última frase exibida
} client_info;

void logexit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void printMovieMenu()
{
    printf("-----------------------------------\n");
    printf("| $ 0 - Sair\n");
    printf("| $ 1 - Senhor dos aneis\n");
    printf("| $ 2 - O poderoso chefao\n");
    printf("| $ 3 - Clube da luta\n");
    printf("-----------------------------------\n");
}

int main(int argc, const char *argv[])
{

    int port;
    char *IPAddress; 
    struct sockaddr_in address, server_address;
    client_info client; 

    client.socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client.socket < 0)
    {
        logexit("socket");
    }

    port = atoi(argv[3]);
    IPAddress = argv[2]; 

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (0 != bind(client.socket, (struct sockaddr *)&address, sizeof(address)))
    {
        logexit("bind");
    }

    return 0;
}