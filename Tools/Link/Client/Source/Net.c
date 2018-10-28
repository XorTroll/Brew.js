#include "Net.h"

#include <switch.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct sockaddr_in server;
struct sockaddr_in client;
int sock;
int inew,i;
int sockaddr_len = sizeof(struct sockaddr_in);
int data_len = 1;
char data[4096];
bool disc = false;

void _neterror(const char *errorstr)
{
    gfxExit();
    gfxInitDefault();
    consoleInit(NULL);
    printf("\n\n   Brew.js link aborted execution because an error ocurred:\n   - ");
    printf("%s", errorstr);
    printf("\n\n   Press A to exit this application.");
    while(true)
    {
        hidScanInput();
        int k = hidKeysDown(CONTROLLER_P1_AUTO);
        if(k & KEY_A) break;
        gfxFlushBuffers();
        gfxSwapBuffers();
    }
    gfxExit();
    exit(0);
}

void Net_init(u16 Port)
{
    socketInitializeDefault();
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) _neterror("Error creating socket");
    server.sin_family = AF_INET;
    server.sin_port = ntohs(Port);
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(&server.sin_zero, 8);
    if((bind(sock, (struct sockaddr*)&server, sockaddr_len)) == -1) _neterror("Error binding socket");
    if((listen(sock, 50)) == -1) _neterror("Error listening socket");
}

void Net_waitForPC(void)
{
    if((inew = accept(sock, (struct sockaddr*)&client, &sockaddr_len)) == -1) _neterror("Error accepting socket");
}

void Net_exit(void)
{
    close(sock);
    socketExit();
}

const char *Net_waitRecieve(void)
{
    if(!data_len)
    {
        disc = true;
        close(inew);
        return NULL;
    }
    data[data_len] = '\0';
    data_len = recv(inew, data, sizeof(data), 0);
    if(data_len == -1) _neterror("Error receiving source code from PC client");
    data[data_len] = '\0';
    if(data_len) return data;
    else return NULL;
}

void Net_sendResult(const char *Encoded)
{
    if(send(inew, Encoded, sizeof(Encoded), 0) == -1) _neterror("Error sending evaluation result to PC client");
}

bool Net_ok(void)
{
    return !disc;
}

/*
{
    while(1) // Better signal handling required
    {
        gfxFlushBuffers();
        gfxSwapBuffers();
        if((inew = accept(sock, (struct sockaddr *)&client, &sockaddr_len)) == -1)
        {
            printf("accepted!");
            debugP("accept error!");
        }
        printf("new Client connected from port no %d and IP %s\n", ntohs(client.sin_port), inet_ntoa(client.sin_addr));

        data_len = 1;

        while(data_len)
        {
            gfxFlushBuffers();
            gfxSwapBuffers();
            data_len = recv(inew, data, 1024, 0);
            printf("\nRECEIVED: %s", data);
            if(data_len)
            {
                send(inew, data, data_len, 0);
                data[data_len] = '\0';
                printf("\nSent mesg to client: %s", data);
            }
        }
        printf("Client disconnected\n");
        close(inew);
    }
    close(sock);
    debugP("Done!");
}
*/