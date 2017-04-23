#ifndef TcpService_H
#define TcpService_H

#include <arpa/inet.h>
#include <functional>

#include "Reactor.h"

class ITcpService : public IEventHandler
{
public:
    static ITcpService& CreateService(class TcpAcceptor&);
};

class TcpService : public ITcpService
{
public:
    TcpService(class Reactor&, int);
    virtual int GetHandle() const;
    virtual void HandleEvent();
    virtual ~TcpService();
protected:
    class Reactor& mReactor;
    int mSocket;
    sockaddr_in mAddress;
    socklen_t mAddressLength;
};

class TcpAcceptor : public IEventHandler
{
protected:
    using ServiceFactory = std::function<ITcpService&(class TcpAcceptor&)>;
public:
    TcpAcceptor(class Reactor&, int, ServiceFactory);
    virtual ~TcpAcceptor();
    virtual int GetHandle() const;
    virtual void HandleEvent();
    class Reactor& GetReactor(){return mReactor;}
protected:
    class Reactor& mReactor;
    int mPort;
    int mSocket;
    ServiceFactory mFactory;
};

#endif

