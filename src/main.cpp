#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UdpClient.h"
#include "TcpClient.h"
#include "Server.h"

void usage(const char* name)
{
    printf("Usage:\n");
    printf("%s -t UdpServer -p <port>\n", name); 
    printf("%s -t TcpServer -p <port>\n", name); 
    printf("%s -t UdpClient -a <addr> -p <port>\n", name); 
    printf("%s -t TcpClient -a <addr> -p <port>\n", name); 
    exit(-1);
}

int main(int argc, char* argv[])
{
    int opt;
    bool udpServer = false;
    bool udpClient = false;
    bool tcpServer = false;
    bool tcpClient = false;
    int port = -1;
    const char* addr = nullptr;
    while((opt = getopt(argc, argv, "t:a:p:")) != -1)
    {
        switch(opt)
        {
        case 'a':
            addr = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 't':
            if(strncmp(optarg, "UdpClient", sizeof("UdpClient")) == 0)
                udpClient = true;
            else if(strncmp(optarg, "UdpServer", sizeof("UdpServer")) == 0)
                udpServer = true;
            else if(strncmp(optarg, "TcpServer", sizeof("TcpServer")) == 0)
                tcpServer = true;
            else if(strncmp(optarg, "TcpClient", sizeof("TcpClient")) == 0)
                tcpClient = true;
            else
                usage(argv[0]);
            break;
        default:
            usage(argv[0]);
            break;
        }
    }

    if(!udpServer && !tcpServer)
    {
        // client mode, only one client is supported
        if(udpClient)
        {
            if((addr == nullptr) || (port < 0))
                usage(argv[0]);
            printf("Running UDP client connecting to %s\n", addr);
            UdpClient client(addr, port);
            client.Run();
        }
        else if(tcpClient)
        {
            if((addr == nullptr) || (port < 0))
                usage(argv[0]);
            printf("Running TCP client connecting to %s\n", addr);
            TcpClient client(addr, port);
            client.Run();
        }
        else
        {
            usage(argv[0]);   
        }
    }
    else
    {
        Server server;
        if(udpServer)
        {
            if(port < 0)
                usage(argv[0]);
            printf("Running udp server on port %d\n", port);
            server.AddUdpServer(port);
        }
        if(tcpServer)
        {
            if(port < 0)
                usage(argv[0]);
            printf("Running server on port %d\n", port);
            server.AddTcpServer(port);
        }
        server.Run();
    }
    return 0;    
}
