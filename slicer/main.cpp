#include <ctime>
#include <algorithm>
#include <cmath>
#include <vector>
#include <atomic>
#include <chrono>
#include <thread>
#include <memory>
#include <string>
#include <iostream>
#include <set>

#include "common.hpp"
#include "router.hpp"
#include "client.hpp"
#include "task.hpp"

std::atomic_int g_stop{false};
void make_readness(std::vector<Task> tasks)
{
    while (!g_stop.load())
    {
        std::vector<double> loads_in_the_moment;
        loads_in_the_moment.reserve(TASKS_CNT);
        for (auto task : tasks)
        {
            loads_in_the_moment.push_back(
                task->get_rate_load());
        }
        std::cout << "loads: ";
        for (auto el : loads_in_the_moment)
        {
            std::cout << el << ' ';
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}

void make_distribution(std::reference_wrapper<Assignment> assignment)
{
    while (!g_stop.load())
    {
        std::cout << "\033c";
        assignment.get().PrintDestribution();
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}
void make_disbalance(std::reference_wrapper<Assignment> assignment)
{
    while (!g_stop.load())
    {
        auto hotness = assignment.get().get_hotness();
        double max_hot = 0, sum_hot = 0;
        for (auto task_hot : hotness)
        {
            double slicer_task_hot = 0;
            for (auto taskskalsjf : task_hot)
            {
                slicer_task_hot += taskskalsjf;
            }
            max_hot = std::max(max_hot, slicer_task_hot);
            sum_hot += slicer_task_hot;
        }
        std::cout << "Dis balance: " << max_hot << ' ' << sum_hot << ' ' << max_hot - sum_hot / hotness.size() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}

int main()
{
    srand(time(NULL));
    std::vector<Task> tasks;
    tasks.reserve(TASKS_CNT);
    for (size_t i = 0; i < TASKS_CNT; ++i)
    {
        tasks.emplace_back(std::make_shared<TaskImpl>());
    }
    Router router(tasks);
    std::vector<std::thread> clients_threads;
    clients_threads.reserve(CLIENT_CNT);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    for (size_t i = 0; i < CLIENT_CNT; ++i)
    {
        clients_threads.push_back(std::thread([&router]()
                                              {
                                                  Client client(router);
                                                  std::this_thread::sleep_for(TIME_OF_EXPERIMENT);
                                                  client.Stop();
                                                  while (client.continue_thread()) {
                                                    
                                                  } }));
    }
    auto printing_thread = [&tasks, &router]()
    {
        const char *env_p = std::getenv("PRINTING");
        if (env_p && std::string(env_p) == "0")
        {
            return std::thread(make_readness, tasks);
        }
        else if (env_p && std::string(env_p) == "1")
        {
            return std::thread(make_distribution, std::ref(router.GetAssignment()));
        }
        else if (env_p && std::string(env_p) == "2")
        {
            return std::thread(make_disbalance, std::ref(router.GetAssignment()));
        }
        else
        {
            throw std::runtime_error("not implemented PRINTING");
        }
    }();

    for (size_t i = 0; i < CLIENT_CNT; ++i)
    {
        clients_threads[i].join();
    }
    g_stop.store(true);
    printing_thread.join();
    router.WaitToStop();
    for (auto task : tasks)
    {
        while (task->get_rate_load() != 0)
        {
            continue;
        }
    }
}