#pragma once
#include <thread>
#include <atomic>
#include "assignment.hpp"
#include "common.hpp"

class Stb;

class Router
{
public:
    Router(std::vector<Task> &tasks);
    bool Route(std::string key, Stb &stb);
    void WaitToStop();
    Assignment &GetAssignment();

private:
    std::atomic_int stop_{false};
    std::mutex mutex;
    Assignment assignment_;
    std::thread assigner_;
};