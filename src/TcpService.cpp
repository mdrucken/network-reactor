#include "TcpService.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>


ITcpService& ITcpService::CreateService(class TcpAcceptor& acceptor)
{
    return *new TcpService(acceptor.GetReactor(), acceptor.GetHandle());
}



TcpAcceptor::TcpAcceptor(class Reactor& reactor, int port, ServiceFactory factory)
  : mReactor(reactor),
    mPort(port),
    mFactory(factory)
{
    printf("TcpAcceptor CTor\n");
    mSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(mSocket < 0)
    {
        perror("TcpAcceptor: Cannot open socket");
        exit(-1);
    }      
    int enable = 1;
    if (setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(-1);
    }

    sockaddr_in serveraddr;
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)mPort);
    if (bind(mSocket, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0) 
    {
        perror("TcpAcceptor: ERROR on binding");
        exit(-1);
    }

    if (listen(mSocket, 10) < 0)
    {
        perror("TcpAcceptor: Cannot listen\n");
        exit(-1);
    }

    mReactor.RegisterHandler(*this);
}

TcpAcceptor::~TcpAcceptor()
{
    printf("TcpAcceptor DTor\n");
}

int TcpAcceptor::GetHandle() const
{
    return mSocket;
}

void TcpAcceptor::HandleEvent()
{
    printf("TcpAcceptor: Handle event\n");
    ITcpService& service = mFactory(*this);
}

///////////////////////////////////////////////////////////////////////////////
TcpService::TcpService(Reactor& reactor, int socket) : mReactor(reactor)
{
    printf("TcpService CTor\n");
    mSocket = accept(socket, (sockaddr*)&mAddress, &mAddressLength);
    if (mSocket < 0)
    {
        perror("ERROR on accept");
        exit(-1);
    }
    
    mReactor.RegisterHandler(*this);
}

int TcpService::GetHandle() const
{
    return mSocket;
}

void TcpService::HandleEvent() 
{
    char buffer[2000];
    int ret = recv(mSocket, buffer, sizeof(buffer), 0);
    if(ret > 0)
    {
        printf("TcpService received %d bytes: %s\n", ret, buffer);
    }
    else
    {
        delete this;
    }
}

TcpService::~TcpService()
{
    printf("TcpService DTor\n");
    mReactor.UnregisterHandler(*this);
    close(mSocket);
    mSocket = -1;
}

