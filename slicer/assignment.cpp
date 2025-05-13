#include <iostream>
#include <vector>
#include "task.hpp"
#include "common.hpp"
#include "assignment.hpp"
#include <shared_mutex>

Assignment::Assignment(std::vector<Task> tasks) : mutexes_(tasks.size())
{
    std::string left = key_from_number(0);
    std::string right = key_from_number(MAX_KEY / TASKS_CNT + 1);
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        assignment_.push_back(std::make_pair(tasks[i], StringCollection{std::make_pair(left, right)}));
        left = right;
        right = key_from_number(std::min(atoi(right.c_str()) + MAX_KEY / TASKS_CNT + 1, MAX_KEY));
    }
    for (auto &[task, collection] : assignment_)
    {
        for (auto &range : collection)
        {
            task->AssignRange(range);
        }
    }
}

void Assignment::AssignRangeToTask(size_t task_ind, StringRange new_range)
{
    std::shared_lock<std::shared_mutex> lock_1_level(mutex);
    std::unique_lock<std::shared_mutex> lock_2_level(mutexes_[task_ind]);
    assignment_[task_ind].second.push_back(new_range);
}

void Assignment::EraseRangeFromTask(size_t task_ind, size_t range_ind)
{
    std::shared_lock<std::shared_mutex> lock_1_level(mutex);
    std::unique_lock<std::shared_mutex> lock_2_level(mutexes_[task_ind]);
    assignment_[task_ind].second.erase(assignment_[task_ind].second.begin() + range_ind);
}
StringRange Assignment::Get(size_t task_ind, size_t range_ind)
{
    std::shared_lock<std::shared_mutex> lock_1_level(mutex);
    std::shared_lock<std::shared_mutex> lock_2_level(mutexes_[task_ind]);
    return assignment_[task_ind].second[range_ind];
}

std::vector<std::vector<double>> Assignment::get_hotness()
{
    std::unique_lock<std::shared_mutex> lock(mutex);
    std::vector<std::vector<double>> hotness(assignment_.size());
    size_t ind = 0;
    for (auto &[task, collection] : assignment_)
    {
        hotness[ind].reserve(collection.size());
        for (auto &range : collection)
        {
            hotness[ind].push_back(task->HotnessOftheRange(range));
        }
        ++ind;
    }
    return hotness;
}
std::vector<Task> Assignment::get_choosing_from(std::string key)
{
    std::unique_lock<std::shared_mutex> lock(mutex);

    std::vector<Task> choose_from;
    for (auto &[task, collection] : assignment_)
    {
        for (auto &range : collection)
        {
            if (range.first <= key && key < range.second)
            {
                choose_from.push_back(task);
            }
        }
    }
    return choose_from;
}

void Assignment::PrintDestribution()
{
    std::unique_lock<std::shared_mutex> lock(mutex);

    std::cout << "We: \n";
    for (auto &[task, collection] : assignment_)
    {
        std::cout << "\ttask #" << &task << '\n';
        for (auto &range : collection)
        {
            std::cout << "\t\t[" << range.first << ' ' << range.second << "]\n";
        }
    }
    std::cout << std::endl;
}