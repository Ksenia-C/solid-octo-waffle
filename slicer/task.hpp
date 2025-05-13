#pragma once

#include "common.hpp"
#include <atomic>
#include <memory>
#include <thread>
class Stb;

class TaskImpl
{
public:
    TaskImpl() = default;
    void CompleteTask(std::string key, Stb &stb);
    double HotnessOftheRange(StringRange asked_range);
    int get_rate_load();
    void AssignRange(StringRange new_range);
    void RemoveRange(StringRange new_range);
    void MergeRanges(StringRange left_range, StringRange right_range);
    void SplitRange(StringRange new_range, StringRange new_range_two);

private:
    std::mutex mutex;
    StringCollection our_keys;
    std::vector<int> hotness_of_ranges;
    std::atomic_int rate_requests{0};
};
