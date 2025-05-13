#include "../assignment.hpp"
#include "../common.hpp"
#include "../task.hpp"
#include "blocks.hpp"

namespace Article
{

    namespace
    {
        struct RangePointer
        {
            ssize_t task;
            ssize_t ind;
        };
        enum Direction
        {
            DontMove,
            ToFirst,
            ToSecond
        };
        enum Operation
        {
            NothingOp,
            MoveOp,
            CopyOP
        };
        struct MoveCopyData
        {
            ssize_t task_from;
            ssize_t task_to;
            ssize_t range_from_ind;
            Operation direction;
        };
    }

    void MakeAssignment(Assignment &assignment_)
    {

        std::vector<std::vector<double>> hotness = assignment_.get_hotness();
        double average_temperature_sum = 0;
        long long ranges_cnt = 0;
        for (auto &task_hotness : hotness)
        {
            for (auto &range_hotness : task_hotness)
            {
                average_temperature_sum += range_hotness;
                ++ranges_cnt;
            }
        }
        auto get_two_as_cool = [](Assignment &assignment, const std::vector<std::vector<double>> &hot, double average_temperature_sum, long long ranges_cnt)
        {
            for (ssize_t i = 0; i < (ssize_t)hot.size(); ++i)
            {
                for (ssize_t j = 0; j < (ssize_t)hot[i].size(); ++j)
                {
                    for (ssize_t k = 0; k < (ssize_t)hot.size(); ++k)
                    {
                        for (ssize_t t = 0; t < (ssize_t)hot[k].size(); ++t)
                        {
                            if (assignment.Get(i, j).second == assignment.Get(k, t).first)
                            {
                                // TODO: check that the merge result isn't hotter than average
                                if (true /*YOUR CODE*/)
                                {
                                    return std::make_pair(RangePointer(i, j), RangePointer(k, t));
                                }
                            }
                        }
                    }
                }
            }
            return std::make_pair(RangePointer(-1, -1), RangePointer(-1, -1));
        };
        auto merge_direction_based_on_result_hotness = [](const std::vector<std::vector<double>> &hot, RangePointer range_f, RangePointer range_s)
        {
            double max_load = 0;
            if (range_f.task == range_s.task)
            {
                return Direction::DontMove;
            }
            for (size_t i = 0; i < hot.size(); ++i)
            {
                double tsk_load = 0;
                for (auto e : hot[i])
                {
                    tsk_load += e;
                }
                max_load = std::max(max_load, tsk_load);
            }
            // what if we move second to first
            double tsk_load = 0;
            for (auto e : hot[range_f.task])
            {
                tsk_load += e;
            }
            tsk_load += hot[range_s.task][range_s.ind];
            if (tsk_load < max_load)
            {
                return Direction::ToFirst;
            }
            // what if we move first to second
            tsk_load = 0;
            for (auto e : hot[range_s.task])
            {
                tsk_load += e;
            }
            tsk_load += hot[range_f.task][range_f.ind];
            if (tsk_load < max_load)
            {
                return Direction::ToSecond;
            }
            return Direction::DontMove;
        };

        // step 1: Merge
        auto [cool_first, cool_second] = get_two_as_cool(assignment_, hotness, average_temperature_sum, ranges_cnt);
        size_t churn = 0;
        if (cool_first.task != -1 && cool_second.task != -1)
        {

            auto merge_direction = merge_direction_based_on_result_hotness(hotness, cool_first, cool_second);
            // TODO: check conditions
            while (false /*YOUR CODE*/)
            {
                auto range_first = assignment_.Get(cool_first.task, cool_first.ind);
                auto range_second = assignment_.Get(cool_second.task, cool_second.ind);
                if (cool_first.task == cool_second.task)
                {
                }
                else if (merge_direction == Direction::ToFirst)
                {

                    churn += atoi(range_second.second.c_str()) - atoi(range_second.first.c_str());
                    Move(hotness, assignment_, cool_second.task, cool_second.ind, cool_first.task);
                }
                else if (merge_direction == Direction::ToSecond)
                {
                    churn += atoi(range_first.second.c_str()) - atoi(range_first.first.c_str());

                    Move(hotness, assignment_, cool_first.task, cool_first.ind, cool_second.task);
                }
                if (merge_direction == Direction::ToFirst)
                {
                    Merge(hotness, assignment_, cool_first.task, cool_first.ind, hotness[cool_first.ind].size() - 1);
                }
                else if (merge_direction == Direction::ToSecond)
                {
                    Merge(hotness, assignment_, cool_second.task, cool_second.ind, hotness[cool_second.ind].size() - 1);
                }
                else if (cool_second.task == cool_first.task)
                {
                    Merge(hotness, assignment_, cool_second.task, cool_second.ind, cool_first.ind);
                }
                auto [coolest_first_, coolest_second_] = get_two_as_cool(assignment_, hotness, average_temperature_sum, ranges_cnt);
                if (cool_first.task == -1 || cool_second.task == 1)
                    break;
                merge_direction = merge_direction_based_on_result_hotness(hotness, cool_first, cool_second);
                cool_first = coolest_first_;
                cool_second = coolest_second_;
            }
        }
        // step 2: Move
        auto max_task_disbalance_change_strct = [](std::vector<std::vector<double>> hot, Assignment &assignment)
        {
            int ind_max = 0, max_weight = 0;
            int ind_min = 0, min_weight = -1;
            for (size_t i = 1; i < hot.size(); ++i)
            {
                int task_weight = 0;
                for (size_t j = 0; j < hot[i].size(); ++j)
                {
                    task_weight += hot[i][j];
                }
                if (max_weight < task_weight)
                {
                    ind_max = i;
                    max_weight = task_weight;
                }
                if (min_weight == -1 || min_weight > task_weight)
                {
                    ind_min = i;
                    min_weight = task_weight;
                }
            }
            double max_mvcp_weight = 0;
            int max_weight_range_ind = -1;
            Operation direction = Operation::NothingOp;
            for (size_t range_ind = 0; range_ind < hot[ind_max].size(); ++range_ind)
            {
                // TODO: get the maximum change in hotness by either politic (move or copy)
                double diff_in_hotness = 0 /*YOUR CODE*/;
                auto current_range = assignment.Get(ind_max, range_ind);
                double current_weight = diff_in_hotness * 1.0 / (atoi(current_range.second.c_str()) - atoi(current_range.first.c_str()));
                if (max_mvcp_weight < current_weight)
                {
                    max_mvcp_weight = current_weight;
                    max_weight_range_ind = range_ind;
                    if (diff_in_hotness ==
                        max_weight - std::max(max_weight - hot[ind_max][range_ind] / 2, min_weight + hot[ind_max][range_ind] / 2))
                    {
                        direction = Operation::CopyOP;
                    }
                    else
                    {
                        direction = Operation::MoveOp;
                    }
                }
            }
            return MoveCopyData{ind_max, ind_min, max_weight_range_ind, direction};
        };
        auto next_op = max_task_disbalance_change_strct(hotness, assignment_);
        while (churn * 100 < 9 * MAX_KEY)
        {
            if (next_op.range_from_ind < 0)
                break;
            int task_from = next_op.task_from;
            int task_to = next_op.task_to;
            int range_ind_from = next_op.range_from_ind;
            int direction = next_op.direction;

            auto range_to_move = assignment_.Get(task_from, range_ind_from);
            churn += atoi(range_to_move.second.c_str()) - atoi(range_to_move.first.c_str());
            if (direction == Operation::CopyOP)
            {
                Copy(hotness, assignment_, task_from, range_ind_from, task_to);
            }
            else if (direction == Operation::MoveOp)
            {
                Move(hotness, assignment_, task_from, range_ind_from, task_to);
            }
            else
            {
                break;
            }

            next_op = max_task_disbalance_change_strct(hotness, assignment_);
        }
        // step 3: Split
        auto get_hottest_slice = [](std::vector<std::vector<double>> hot)
        {
            RangePointer hottest_ind = {0, 0};
            for (ssize_t i = 0; i < (ssize_t)hot.size(); ++i)
            {
                for (ssize_t j = 0; j < (ssize_t)hot[i].size(); ++j)
                {
                    if (hot[hottest_ind.task][hottest_ind.ind] < hot[i][j])
                    {
                        hottest_ind = RangePointer{i, j};
                    }
                }
            }
            return hottest_ind;
        };
        auto get_avg_ranges_per_task = [](std::vector<std::vector<double>> hot)
        {
            int sum_slices = 0;
            for (size_t i = 0; i < hot.size(); ++i)
            {
                sum_slices += hot[i].size();
            }
            return sum_slices * 1.0 / hot.size();
        };
        auto get_avg_slice = [](std::vector<std::vector<double>> hot)
        {
            double sum_all = 0;
            uint32_t count = 0;
            for (size_t i = 0; i < hot.size(); ++i)
            {
                for (size_t j = 0; j < hot[i].size(); ++j)
                {
                    sum_all += hot[i][j];
                    ++count;
                }
            }
            return sum_all / std::max(count, (uint32_t)1);
        };
        auto ind_to_split = get_hottest_slice(hotness);
        // TODO: follow the steps for split
        while (false /*YOUR CODE*/)
        {
            Split(hotness, assignment_, ind_to_split.task, ind_to_split.ind);
        }
    };
}
