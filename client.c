#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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
    int address_size = sizeof(server_address);
    client_info client;

    client.socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client.socket < 0)
    {
        logexit("socket");
    }

    port = atoi(argv[3]);
    IPAddress = argv[2];

    // identificação local

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(40000);

    if (0 != bind(client.socket, (struct sockaddr *)&address, sizeof(address)))
    {
        logexit("bind");
    }

    // identificação do servidor

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(IPAddress);
    server_address.sin_port = htons(port);

    int option, nbytes;

    char msg[100];
    do
    {
        printMovieMenu();
        scanf("%d", &option);

        sprintf(msg, "%d", option);

        if (option == 0)
        {
            close(client.socket);
            exit(EXIT_SUCCESS);
        }

        nbytes = sendto(client.socket, msg, 100, 0, (struct sockaddr *)&server_address, sizeof(server_address));

        if (nbytes < 0)
        {
            logexit("sendto");
        }

        int cnt = 0;

        while (cnt < 5)
        {
            nbytes = recvfrom(client.socket, msg, 100, 0, (struct sockaddr *)&server_address, &address_size);

            if (nbytes < 0)
            {
                logexit("recvfrom");
            }

            printf("%s\n", msg);

            cnt++;
        }

    } while (option != 0);

    close(client.socket);

    return 0;
}