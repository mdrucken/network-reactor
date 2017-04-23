#ifndef UdpService_H
#define UdpService_H

#include <arpa/inet.h>
#include <map>
#include <functional>

#include "Reactor.h"

class IUdpService
{
public:
    static IUdpService& CreateService(class UdpAcceptor&, const struct sockaddr_in&);
    virtual void ReceivePacket(const char* data, int length) = 0;
};

class UdpService : public IUdpService
{
public:
    UdpService(class UdpAcceptor&, const struct sockaddr_in&);
    virtual ~UdpService();
    virtual void ReceivePacket(const char*, int);
protected:
    sockaddr_in mAddress;
    class UdpAcceptor& mDispatcher;
};

class UdpAcceptor : public IEventHandler
{
protected:
    using ServiceMap = std::map<sockaddr_in, class IUdpService*>;
    using ServiceFactory = std::function<IUdpService&(class UdpAcceptor&, const sockaddr_in&)>;
public:
    UdpAcceptor(class Reactor&, int, ServiceFactory);
    void RegisterService(IUdpService&, const sockaddr_in&);
    void UnregisterService(IUdpService&, const sockaddr_in&);
    virtual ~UdpAcceptor();
    virtual int GetHandle() const;
    virtual void HandleEvent();
protected:
    class Reactor& mReactor;
    int mPort;
    int mSocket;
    ServiceMap mServices;
    ServiceFactory mFactory;
};


#endif

