
#include <numeric>
#include "blocks.hpp"
#include "../assignment.hpp"
#include "../common.hpp"
#include "../task.hpp"

/**
 * 1. split the hottest (as twice as initial average hotness)
 * 2. move the hottest ranges from hottest tasks to the coolest while can
 * 3. merge ranges a bit
 */

namespace KseniaPetrenko
{
    void MakeAssignment(Assignment &assignment_)
    {

        std::vector<std::vector<double>> hotness = assignment_.get_hotness();
        double average_temp = 0;
        for (auto els : hotness)
        {
            average_temp += std::accumulate(els.begin(), els.end(), 0);
        }
        average_temp /= hotness.size();
        // step 1: Split
        size_t ind_task = 0;
        for (auto &task_hotness : hotness)
        {
            size_t i = 0;
            while (i < task_hotness.size())
            {
                if (task_hotness[i] > 2 * average_temp)
                {
                    Split(hotness, assignment_, ind_task, i);
                }
                else
                {
                    ++i;
                }
            }
            ++ind_task;
        }

        // step 2: Move
        std::vector<size_t> sorting_order(hotness.size());
        std::generate(sorting_order.begin(), sorting_order.end(), [n = 0]() mutable
                      { return n++; });
        std::sort(sorting_order.begin(), sorting_order.end(), [&hotness](size_t a, size_t b)
                  { return std::accumulate(hotness[a].begin(), hotness[a].end(), 0) < std::accumulate(hotness[b].begin(), hotness[b].end(), 0); });

        size_t hottest = sorting_order.size() - 1;
        size_t coolest = 0;

        int churns = 0;
        while (hottest > coolest && (size_t)churns * 100 < MAX_KEY)
        {
            auto &hottest_collection = hotness[sorting_order[hottest]];
            double sum_hottest = 0;
            auto get_hotencet_slice = [&hottest_collection, &sum_hottest]()
            {
                size_t hotencet_slice = 0;
                sum_hottest = 0;
                for (size_t i = 0; i < hottest_collection.size(); ++i)
                {
                    sum_hottest += hottest_collection[i];
                    if (hottest_collection[i] > hottest_collection[hotencet_slice])
                    {
                        hotencet_slice = i;
                    }
                }
                return hotencet_slice;
            };
            double sum_coolest = std::accumulate(hotness[sorting_order[coolest]].begin(), hotness[sorting_order[coolest]].end(), 0);
            while (size_t hotencet_slice = get_hotencet_slice() && sum_hottest - hottest_collection[hotencet_slice] > average_temp && (size_t)churns * 100 < MAX_KEY)
            {
                if (sum_coolest + hottest_collection[hotencet_slice] < average_temp)
                {
                    auto slice_to_move = assignment_.Get(sorting_order[hottest], hotencet_slice);

                    Move(hotness, assignment_, sorting_order[hottest], hotencet_slice, sorting_order[coolest]);

                    sum_hottest -= hottest_collection[hotencet_slice];

                    churns += std::atoi(slice_to_move.second.c_str()) - atoi(slice_to_move.first.c_str());
                    hottest_collection[hotencet_slice] = 0;
                }
                else if (sum_coolest + hottest_collection[hotencet_slice] / 2 < average_temp)
                {

                    auto slice_to_move = assignment_.Get(sorting_order[hottest], hotencet_slice);
                    Copy(hotness, assignment_, sorting_order[hottest], hotencet_slice, sorting_order[coolest]);

                    hotness[sorting_order[hottest]][hotencet_slice] /= 2;

                    churns += std::atoi(slice_to_move.second.c_str()) - atoi(slice_to_move.first.c_str());

                    sum_hottest -= hottest_collection[hotencet_slice] / 2;
                    hottest_collection[hotencet_slice] /= 2;
                }
                else
                {
                    ++coolest;
                    if (coolest == sorting_order.size())
                        break;
                    sum_coolest = std::accumulate(hotness[sorting_order[coolest]].begin(), hotness[sorting_order[coolest]].end(), 0);
                }
            }
            --hottest;
        }
        // Step 3: Merge
        for (int iters = 0; iters < 100; ++iters)
        {
            for (size_t i = 0; i < hotness.size(); ++i)
            {
                bool need_continue = false;
                size_t j = 0;
                while (j < hotness[i].size())
                {
                    size_t k = j + 1;
                    while (k < hotness[i].size())
                    {
                        if (hotness[i][j] + hotness[i][k] <= average_temp && Merge(hotness, assignment_, i, j, k))
                        {
                            need_continue = true;
                            break;
                        }
                        else
                        {
                            ++k;
                        }
                    }
                    if (need_continue)
                        break;
                    ++j;
                }
                if (need_continue)
                    break;
            }
        }
    }
}
