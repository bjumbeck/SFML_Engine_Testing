#include "EventManager.hpp"

std::size_t BaseEvent::familyCounter = 0;

std::size_t EventManager::connectReceivers() const
{
    std::size_t size = 0;
    for (std::shared_ptr<EventSignal> handler : eventHandlers)
    {
        if (handler)
        {
            size += handler->size();
        }
    }

    return size;
}

std::shared_ptr<EventSignal>& signalFor(std::size_t handlerId)
{
    if (handlerId >= eventHandlers.size())
    {
        eventHandlers.resize(handlerId + 1);
    }

    if (!eventHandlers[handlerId])
    {
        eventHandlers[handlerId] = std::make_shared<EventSignal>();
    }

    return eventHandlers[handlerId];
}