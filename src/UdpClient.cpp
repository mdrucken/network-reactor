#include "UdpClient.h"

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


UdpClient::UdpClient(const char* addr, int port) : mAddress(addr), mPort(port)
{
}

void UdpClient::Run()
{
    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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

    if (fcntl(s, F_SETFL, O_NONBLOCK) < 0) 
    {
        perror("Fail to set socket to non-blocking");
        exit(-1);
    }

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
	  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(mPort);

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
            int tx = sendto(s, txBuffer, strlen(txBuffer) + 1, 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
            printf("Sent %d bytes: %s\n", tx, txBuffer);
        }
        while(1)
        {
            struct sockaddr_in rxAddr;
            socklen_t rxLength;
            int rx = recvfrom(s, rxBuffer, sizeof(rxBuffer), 0, (struct sockaddr*)&rxAddr, &rxLength);
            if (rx > 0) 
                printf("Received %d bytes\n", rx);
            else
                break;
        }
        sleep(1);
    }    

}
