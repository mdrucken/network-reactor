#include "UdpService.h"

#include <stdio.h>
#include <string.h>

IUdpService& IUdpService::CreateService(UdpAcceptor& d, const sockaddr_in& addr)
{
    return *new UdpService(d, addr);
}

///////////////////////////////////////////////////////////////////////////////
UdpService::UdpService(UdpAcceptor& dispatcher, const sockaddr_in& addr)
  : mAddress(addr),
    mDispatcher(dispatcher)
{
    printf("UdpService CTor (%d)\n", ntohs(addr.sin_port));
    dispatcher.RegisterService(*this, addr);
}

UdpService::~UdpService()
{
    printf("UdpService DTor (%d)\n", ntohs(mAddress.sin_port));
}

void UdpService::ReceivePacket(const char* data, int length)
{
    printf("UdpService (%d): Received %d bytes : %s\n", ntohs(mAddress.sin_port), length, data);
    if(strncmp(data, "Connect", length) == 0)
    {
        printf("UdpService connected\n");
    }
    else if(strncmp(data, "Disconnect", length) == 0)
    {
        printf("UdpService disconnected\n");
        mDispatcher.UnregisterService(*this, mAddress);
        delete this;
    }
}

///////////////////////////////////////////////////////////////////////////////
bool operator<(const sockaddr_in& a0, const sockaddr_in& a1)
{
    return memcmp(&a0, &a1, sizeof(a0)) < 0;
}

UdpAcceptor::UdpAcceptor(class Reactor& reactor, int port, ServiceFactory factory) 
        : mReactor(reactor), 
          mPort(port),
          mFactory(factory)
{
    printf("UdpAcceptor CTor\n");
    mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(mSocket < 0)
    {
        perror("UdpAcceptor: Cannot open socket");
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
        perror("UdpAcceptor: ERROR on binding");
        exit(-1);
    }

    mReactor.RegisterHandler(*this);
}

void UdpAcceptor::RegisterService(IUdpService& service, const sockaddr_in& addr)
{
    mServices[addr] = &service;
}

void UdpAcceptor::UnregisterService(IUdpService&, const sockaddr_in& addr)
{
    mServices.erase(addr);
}

UdpAcceptor::~UdpAcceptor()
{
    printf("UdpAcceptor DTor\n");
}

int UdpAcceptor::GetHandle() const  
{   
    return mSocket;
}

void UdpAcceptor::HandleEvent()
{
    printf("UdpAcceptor: Handle event\n");
    sockaddr_in addr;
    socklen_t addrLength;
    char buffer[2000];
    int ret = recvfrom(mSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&addr, &addrLength);
    if(ret > 0)
    {
        IUdpService* service = nullptr;
        ServiceMap::iterator iter = mServices.find(addr); 
        if(iter == mServices.end())
        {
            service = &mFactory(*this, addr);
        }
        else
        {
            service = iter->second;
        }
        service->ReceivePacket(buffer, ret);
    }
}



