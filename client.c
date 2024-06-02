#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>

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
struct sockaddr_in address, server_address;
int address_size = sizeof(server_address);
client_info client;
pthread_t tid;

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

void *client_thread(void *args)
{
    // identificação local

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    srand(time(NULL));
    int random = 10000 + rand() % 50000;
    address.sin_port = htons(random);
    printf("%d\n", random);

    if (0 != bind(client.socket, (struct sockaddr *)&address, sizeof(address)))
    {
        logexit("bind");
    }

    // identificação do servidor

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(IPAddress);
    server_address.sin_port = htons(port);
    do
    {
        printMovieMenu();
        scanf("%d", &option);

        sprintf(msg, "%d", option);

        nbytes = sendto(client.socket, msg, 100, 0, (struct sockaddr *)&server_address, sizeof(server_address));

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
            nbytes = recvfrom(client.socket, msg, 100, 0, (struct sockaddr *)&server_address, &address_size);

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

    client.socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client.socket < 0)
    {
        logexit("socket");
    }

    port = atoi(argv[3]);
    IPAddress = argv[2];

    if (0 != pthread_create(&tid, NULL, client_thread, NULL))
    {
        logexit("pthread_create");
    }

    pthread_join(tid, NULL);

    printf("terminei\n");
    close(client.socket);

    return 0;
}