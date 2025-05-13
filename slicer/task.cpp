#include <iostream>
#include <algorithm>
#include "task.hpp"
#include "client.hpp"

void TaskImpl::CompleteTask(std::string key, Stb &stb)
{
    {
        size_t ind_of_chosen = 0;
        std::lock_guard lock(mutex);
        for (auto key_range : our_keys)
        {
            if (key_range.first <= key && key_range.second > key)
            {
                break;
            }
            ++ind_of_chosen;
        }
        if (ind_of_chosen == our_keys.size())
        {
            stb.Return(false);
        }
        hotness_of_ranges[ind_of_chosen] = (hotness_of_ranges[ind_of_chosen] * 0.5 + 1);
    }
    rate_requests.fetch_add(1);

    std::thread([this, &stb]()
                { std::this_thread::sleep_for(std::chrono::seconds(1));
                    stb.Return(true);
                    this->rate_requests.fetch_sub(1); })
        .detach();
}
double TaskImpl::HotnessOftheRange(StringRange asked_range)
{
    std::lock_guard lock(mutex);
    size_t ind = 0;
    for (auto &range : our_keys)
    {
        if (asked_range == range)
        {
            break;
        }
        ++ind;
    }
    if (hotness_of_ranges.size() <= ind)
    {
        return 0;
    }
    return hotness_of_ranges[ind];
}
int TaskImpl::get_rate_load()
{
    return rate_requests.load();
}
void TaskImpl::AssignRange(StringRange new_range)
{
    std::lock_guard lock(mutex);
    our_keys.push_back(new_range);
    hotness_of_ranges.emplace_back(0);
}
void TaskImpl::RemoveRange(StringRange new_range)
{
    std::lock_guard lock(mutex);
    auto whole_found = std::find(our_keys.begin(), our_keys.end(), new_range);
    if (whole_found != our_keys.end())
    {
        hotness_of_ranges.erase(whole_found - our_keys.begin() + hotness_of_ranges.begin());
        our_keys.erase(whole_found);
    }
}
void TaskImpl::MergeRanges(StringRange left_range, StringRange right_range)
{
    std::lock_guard lock(mutex);

    auto left_found = std::find(our_keys.begin(), our_keys.end(), left_range);
    auto right_found = std::find(our_keys.begin(), our_keys.end(), right_range);
    if (left_range.second == right_range.first && left_found != our_keys.end() && right_found != our_keys.end())
    {
        if (left_found < right_found)
        {
            hotness_of_ranges.erase(right_found - our_keys.begin() + hotness_of_ranges.begin());
            hotness_of_ranges.erase(left_found - our_keys.begin() + hotness_of_ranges.begin());
            our_keys.erase(right_found);
            our_keys.erase(left_found);
        }
        else
        {
            hotness_of_ranges.erase(left_found - our_keys.begin() + hotness_of_ranges.begin());
            hotness_of_ranges.erase(right_found - our_keys.begin() + hotness_of_ranges.begin());
            our_keys.erase(left_found);
            our_keys.erase(right_found);
        }
        our_keys.push_back(std::make_pair(left_range.first, right_range.second));
        hotness_of_ranges.emplace_back(0);
    }
    else
    {
        throw std::runtime_error(std::format("something is wrong during merge {} {} {}", left_range.second == right_range.first, left_found != our_keys.end(), right_found != our_keys.end()));
    }
}
void TaskImpl::SplitRange(StringRange new_range, StringRange new_range_two)
{
    std::lock_guard lock(mutex);

    auto whole_found = std::find(our_keys.begin(), our_keys.end(), std::make_pair(new_range.first, new_range_two.second));
    if (whole_found != our_keys.end() && new_range.second == new_range_two.first)
    {
        hotness_of_ranges.erase(whole_found - our_keys.begin() + hotness_of_ranges.begin());

        our_keys.erase(whole_found);
        our_keys.push_back(new_range);
        hotness_of_ranges.emplace_back(0);
        our_keys.push_back(new_range_two);
        hotness_of_ranges.emplace_back(0);
    }
    else
    {
        throw std::runtime_error("something went wrong during split");
    }
}
