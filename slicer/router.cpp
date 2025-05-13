#include <algorithm>
#include <numeric>
#include "router.hpp"
#include "assignment.hpp"
#include "task.hpp"
#include "client.hpp"
#include "common.hpp"
#include "AssignmersPolicies/article.hpp"
#include "AssignmersPolicies/petrenko.hpp"
#include <random>

Router::Router(std::vector<Task> &tasks) : assignment_(tasks)
{
    const char *env_p = std::getenv("MAKE_ASSIGNMENT_OPT");

    assigner_ = std::thread([this, env_p]()
                            {
                                    while (!this->stop_.load()) {
                                        if (env_p && std::string(env_p) == "0") {
                                            Article::MakeAssignment(this->assignment_);
                                        }
                                        else if (env_p && std::string(env_p) == "1") {
                                            KseniaPetrenko::MakeAssignment(this->assignment_);
                                        }
                                        else {
                                            throw std::runtime_error("Not implemented MAKE_ASSIGNMENT_OPT");
                                        }
                                        std::this_thread::sleep_for(std::chrono::seconds(3));
                                    } });
}
bool Router::Route(std::string key, Stb &stb)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::vector<Task> choose_from = assignment_.get_choosing_from(key);
    if (!choose_from.empty())
    {
        auto task = choose_from[gen() % choose_from.size()];
        task->CompleteTask(key, stb);
        return true;
    }
    return false;
}
void Router::WaitToStop()
{
    stop_.store(true);
    assigner_.join();
}

Assignment &Router::GetAssignment()
{
    return assignment_;
}