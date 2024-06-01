#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct
{
    int socket;
    const **escolha; // Opção escolhida pelo cliente
    struct sockaddr_in address;
    int frase; // Última frase exibida
} client_info;

sem_t x;
pthread_t tid;
pthread_t tid2;
pthread_t threads[100];
int num_clients = 0;
const **selected;
int server_socket;

int server_socket, port, nbytes, err, option;
char msg[100];
struct sockaddr_in server_address;
client_info client_address[100];
int address_size = sizeof(server_address);

const char *senhor_dos_aneis[] = {
    "Um anel para a todos governar",
    "Na terra de Mordor onde as sombras se deitam",
    "Não é o que temos, mas o que fazemos com o que temos",
    "Não há mal que sempre dure",
    "O mundo está mudando, senhor Frodo"};

const char *o_poderoso_chefao[] = {
    "Vou fazer uma oferta que ele não pode recusar",
    "Mantenha seus amigos por perto e seus inimigos mais perto ainda",
    "É melhor ser temido que amado",
    "A vingança é um prato que se come frio",
    "Nunca deixe que ninguém saiba o que você está pensando"};

const char *clube_da_luta[] = {
    "Primeira regra do Clube da Luta: você não fala sobre o Clube da Luta",
    "Segunda regra do Clube da Luta: você não fala sobre o Clube da Luta",
    "O que você possui acabará possuindo você",
    "É apenas depois de perder tudo que somos livres para fazer qualquer coisa",
    "Escolha suas lutas com sabedoria"};

void *sendSentences(void *param)
{

    int cnt = 0;
    int num = (int)param;

    while (cnt < 5)
    {
        int nbytes = sendto(server_socket, client_address[num].escolha[cnt], 100, 0, (struct sockaddr *)&client_address[num].address, sizeof(client_address[num].address));
        if (nbytes < 0)
        {
            logexit("sendto");
        }
        sleep(3);
        cnt++;
    }
    pthread_exit(NULL);
}

void *printNumClients(void *args)
{
    while (1)
    {
        printf("Clientes: %d\n", num_clients);
        sleep(4);
    }
}

void logexit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, const char *argv[])
{

    port = atoi(argv[2]);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0)
    {
        logexit("socket");
    }

    // identificação local

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (0 > bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)))
    {
        logexit("bind");
    }

    pthread_create(&tid2, NULL, printNumClients, NULL);
    // pthread_join(tid2, NULL);

    while (1)
    {
        nbytes = recvfrom(server_socket, msg, 100, 0, (struct sockaddr *)&client_address[num_clients].address, &address_size);
        if (nbytes < 0)
        {
            logexit("recvfrom");
        }

        // printf("%s\n", msg);

        if (strcmp(msg, "0") == 0)
        {
            num_clients--;
            continue;
        }

        if (strcmp(msg, "1") == 0)
        {
            selected = senhor_dos_aneis;
        }
        if (strcmp(msg, "2") == 0)
        {
            selected = o_poderoso_chefao;
        }
        if (strcmp(msg, "3") == 0)
        {
            selected = clube_da_luta;
        }

        client_address[num_clients].escolha = selected;

        if (0 != pthread_create(&threads[++num_clients], NULL, sendSentences, num_clients))
        {
            logexit("pthread_create");
        }

        // num_clients++;

        // pthread_join(tid, NULL);

        // printf("opt: %d\n", option);
    }

    close(server_socket);

    return 0;
}