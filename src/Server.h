#ifndef server_H
#define server_H

#include "Reactor.h"

class Server
{
public:
    Server();
    void AddUdpServer(int);
    void AddTcpServer(int);
    void Run();
protected:
    Reactor mReactor;
};

#endif

