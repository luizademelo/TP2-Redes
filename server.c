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

typedef struct
{
    int socket;
    const char **escolha; // Opção escolhida pelo cliente
    struct sockaddr_in address;
} client_info;

sem_t x;
pthread_t tid;
pthread_t tid2;
pthread_t threads[100];
int num_clients = 0, num = 0;
char **selected;
int server_socket;

int server_socket, port, nbytes, err, option;
char msg[100];
// struct sockaddr_in server_address;
struct sockaddr_storage storage;
client_info client_address[1000];
int address_size = sizeof(storage);

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

int receiveClientOption(int num)
{
    nbytes = recvfrom(server_socket, msg, 100, 0, (struct sockaddr *)&client_address[num].address, &address_size);
    if (nbytes < 0)
    {
        logexit("recvfrom");
    }

    if (strcmp(msg, "0") == 0)
    {
        return 0;
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
    client_address[num].escolha = selected;

    return 1;
}

void *sendSentences(void *param)
{

    int num = (int)param;
    int cnt = 0;

    while (cnt < 5)
    {
        int nbytes = sendto(server_socket, client_address[num].escolha[cnt], 100, 0, (struct sockaddr *)&client_address[num].address, sizeof(storage));
        if (nbytes < 0)
        {
            logexit("sendto");
        }
        sleep(3);
        cnt++;
    }

    num_clients--;
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

int main(int argc, const char *argv[])
{

    port = atoi(argv[2]);

    sockaddr_init(argv[1], &storage, port);

    server_socket = socket(storage.ss_family, SOCK_DGRAM, 0);
    if (server_socket < 0)
    {
        logexit("socket");
    }

    if (0 > bind(server_socket, (struct sockaddr *)&storage, sizeof(storage)))
    {
        logexit("bind");
    }

    pthread_create(&tid2, NULL, printNumClients, NULL);

    while (1)
    {

        if (receiveClientOption(num) == 0)
        {
            continue;
        }
        num++;

        if (0 != pthread_create(&threads[++num_clients], NULL, sendSentences, num - 1))
        {
            logexit("pthread_create");
        }
    }

    close(server_socket);

    return 0;
}