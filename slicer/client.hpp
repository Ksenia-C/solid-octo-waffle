#pragma once
#include <atomic>
#include <memory>

class Router;
class Client;
class Stb
{
public:
    Stb(Client &client);
    void Return(bool result);

private:
    Client &who_waits;
};
class Client
{
public:
    Client(Router &router);

    void Next(bool result);
    void Stop();
    bool continue_thread();

private:
    std::atomic_int stop{false};
    std::atomic_int exit_thread{false};

    int successes = 0;
    int failures = 0;
    Router &router_;
    std::unique_ptr<Stb> waiting_stb{};
};
