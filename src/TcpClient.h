#ifndef TcpClient_H
#define TcpClient_H

class TcpClient
{
public:
    TcpClient(const char* addr, int port);
    void Run();
protected:
    const char* mAddress;
    int mPort;
};

#endif

