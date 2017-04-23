#include "Server.h"

#include "Reactor.h"
#include "UdpService.h"
#include "TcpService.h"


Server::Server() 
{
}

void Server::AddUdpServer(int port)
{
    new UdpAcceptor(mReactor, port, IUdpService::CreateService);
}

void Server::AddTcpServer(int port)
{
    new TcpAcceptor(mReactor, port, ITcpService::CreateService);
}

void Server::Run()
{
    mReactor.HandleEvents();
}

