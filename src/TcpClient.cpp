#include "TcpClient.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <strings.h>
#include <netdb.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


TcpClient::TcpClient(const char* addr, int port) : mAddress(addr), mPort(port)
{
}

void TcpClient::Run()
{
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0)
    {
        perror("Cannot open socket");
        exit(-1);
    }

    struct sockaddr_in serveraddr;
    struct hostent *server;
    server = gethostbyname(mAddress);
    if (server == nullptr) 
    {
        perror("gethostbyname failed");
        exit(-1);
    }

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(mPort);

    if (connect(s, (sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
    {
        perror("ERROR connecting");
        exit(-1);
    }

    char txBuffer[2000];
    char rxBuffer[2000];

    for(int i=0; i<70; i++)
    {
        if(i == 0)
            snprintf(txBuffer, sizeof(txBuffer), "Connect");
        else if((i>0) && (i<60))
            snprintf(txBuffer, sizeof(txBuffer), "Packet %d", i);
        else if(i == 60)
            snprintf(txBuffer, sizeof(txBuffer), "Disconnect");
        else
            txBuffer[0] = 0;
        if(strlen(txBuffer) > 0)
        {
            int tx = write(s, txBuffer, strlen(txBuffer) + 1);
            printf("Sent %d bytes: %s\n", tx, txBuffer);
        }
        sleep(1);
    }    

}
