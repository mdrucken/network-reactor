#ifndef UdpClient_H
#define UdpClient_H

class UdpClient
{
public:
    UdpClient(const char* addr, int port);
    void Run();
protected:
    const char* mAddress;
    int mPort;
};

#endif

