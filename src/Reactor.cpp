#include "Reactor.h"

#include <stdio.h>
#include <sys/time.h>
#include <sys/select.h>

Reactor::Reactor() : mHandlerVector(255)
{
}

void Reactor::RegisterHandler(IEventHandler& handler)
{
    int fd = handler.GetHandle();
    mHandlerVector[fd] = &handler;
    mHandlerList.push_back(&handler);
}

void Reactor::UnregisterHandler(IEventHandler& handler)
{
    int fd = handler.GetHandle();
    mHandlerList.remove(&handler);
    mHandlerVector[fd] = nullptr;
}

void Reactor::HandleEvents()
{
    printf("Starting event loop\n");
    struct timeval timeout;
    while(!mHandlerList.empty())
    {
        int maxFd = 0;;
        fd_set readFd;
        FD_ZERO(&readFd);
        for(auto handler : mHandlerList)
        {
            int fd = handler->GetHandle();
            FD_SET(fd, &readFd);
            if(fd >= maxFd)
                maxFd = fd+1;
        }
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int ret = select(maxFd, &readFd, NULL, NULL, &timeout); 
        if(ret < 0)
        {
            perror("select error");
            return;
        }
        else if(ret == 0)
        {
            printf("Select timeout\n");
            return;
        }
        else
        {
            printf("Select event %d\n", ret);
            for(auto handler : mHandlerList)
            {
                if(FD_ISSET(handler->GetHandle(), &readFd))
                {
                    handler->HandleEvent();
                    break;
                }
            }
        }
    }
}


