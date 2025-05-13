#include <iostream>
#include "client.hpp"
#include "router.hpp"
#include "common.hpp"

Client::Client(Router &router) : router_(router)
{
    waiting_stb = std::make_unique<Stb>(*this);
    if (!router_.Route(GenKey(), *waiting_stb.get()))
    {
        std::cout << "Failure! can't route" << std::endl;
        exit_thread.store(true);
    }
}

void Client::Next(bool result)
{
    (result ? successes : failures)++;
    if (stop.load())
    {
        exit_thread.store(true);
        return;
    }
    waiting_stb = std::make_unique<Stb>(*this);
    if (!router_.Route(GenKey(), *waiting_stb.get()))
    {
        std::cout << "Failure! can't route" << std::endl;
    }
}
void Client::Stop()
{
    stop.store(true);
}
bool Client::continue_thread()
{
    return !exit_thread.load();
}

Stb::Stb(Client &client) : who_waits(client) {}
void Stb::Return(bool result)
{
    who_waits.Next(result);
}
