#pragma once
#include <vector>
#include "common.hpp"
#include <shared_mutex>

class Assignment
{
public:
    Assignment(std::vector<Task> tasks);

    template <typename FunctionT>
    void MakeOperationOverTask(size_t task_ind, FunctionT fun)
    {
        std::shared_lock lock_1_level(mutex);
        std::unique_lock lock_2_level(mutexes_[task_ind]);
        fun(assignment_[task_ind].first);
    }
    void AssignRangeToTask(size_t task_ind, StringRange new_range);

    void EraseRangeFromTask(size_t task_ind, size_t range_ind);
    StringRange Get(size_t task_ind, size_t range_ind);
    std::vector<std::vector<double>> get_hotness();
    std::vector<Task> get_choosing_from(std::string key);
    void PrintDestribution();

private:
    std::shared_mutex mutex;
    std::vector<std::shared_mutex> mutexes_;
    std::vector<std::pair<Task, StringCollection>> assignment_;
};