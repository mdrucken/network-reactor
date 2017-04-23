#ifndef Reactor_H
#define Reactor_H

#include <list>
#include <vector>

/** Interface for event handlers registered with the reactor */
class IEventHandler
{
public:
    virtual int GetHandle() const = 0;
    virtual void HandleEvent() = 0;
protected:
    virtual ~IEventHandler() {};
};

class Reactor
{
public:
    Reactor(); 
    void RegisterHandler(IEventHandler&);
    void UnregisterHandler(IEventHandler&);
    void HandleEvents();
protected:
    std::vector<IEventHandler*> mHandlerVector;
    std::list<IEventHandler*> mHandlerList;
};

#endif 

