#include "blocks.hpp"
#include "../common.hpp"
#include "../task.hpp"
#include "../assignment.hpp"
void Move(std::vector<std::vector<double>> &hotness, Assignment &assignment_, size_t ind_from, size_t ind, size_t ind_to)
{

    auto slice_to_move = assignment_.Get(ind_from, ind);
    assignment_.MakeOperationOverTask(ind_to, [&slice_to_move](Task task)
                                      { task->AssignRange(slice_to_move); });
    assignment_.AssignRangeToTask(ind_to, slice_to_move);
    hotness[ind_to].push_back(hotness[ind_from][ind]);
    assignment_.MakeOperationOverTask(ind_from, [&slice_to_move](Task task)
                                      { task->RemoveRange(slice_to_move); });
    assignment_.EraseRangeFromTask(ind_from, ind);
    hotness[ind_from].erase(hotness[ind_from].begin() + ind);
}
void Copy(std::vector<std::vector<double>> &hotness, Assignment &assignment_, size_t ind_from, size_t ind, size_t ind_to)
{
    auto slice_to_move = assignment_.Get(ind_from, ind);
    assignment_.MakeOperationOverTask(ind_to, [&slice_to_move](Task task)
                                      { task->AssignRange(slice_to_move); });
    assignment_.AssignRangeToTask(ind_to, slice_to_move);
    hotness[ind_to].push_back(hotness[ind_from][ind] / 2);
}

void Split(std::vector<std::vector<double>> &hotness, Assignment &assignment_, size_t task_ind_split, size_t split_slice_ind)
{
    auto split_slice = assignment_.Get(task_ind_split, split_slice_ind);
    auto [slice_left, slice_right] = SplitKeys(split_slice);
    assignment_.MakeOperationOverTask(task_ind_split, [&slice_left, &slice_right](Task task)
                                      { task->SplitRange(slice_left, slice_right); });
    assignment_.EraseRangeFromTask(task_ind_split, split_slice_ind);
    assignment_.AssignRangeToTask(task_ind_split, slice_left);
    assignment_.AssignRangeToTask(task_ind_split, slice_right);
    hotness[task_ind_split].push_back(hotness[task_ind_split][split_slice_ind] / 2);
    hotness[task_ind_split].push_back(hotness[task_ind_split][split_slice_ind] / 2);
    hotness[task_ind_split].erase(hotness[task_ind_split].begin() + split_slice_ind);
}
bool Merge(std::vector<std::vector<double>> &hotness, Assignment &assignment_, size_t task_ind, size_t merge_one, size_t merge_two)
{
    auto one_slice = assignment_.Get(task_ind, merge_one);
    auto two_slice = assignment_.Get(task_ind, merge_two);
    StringRange merging_slice;
    if (one_slice.second == two_slice.first)
    {
        assignment_.MakeOperationOverTask(task_ind, [&one_slice, &two_slice](Task task)
                                          { task->MergeRanges(one_slice, two_slice); });
        merging_slice = std::make_pair(one_slice.first, two_slice.second);
    }
    else if (one_slice.first == two_slice.second)
    {
        assignment_.MakeOperationOverTask(task_ind, [&one_slice, &two_slice](Task task)
                                          { task->MergeRanges(two_slice, one_slice); });
        merging_slice = std::make_pair(two_slice.first, one_slice.second);
    }
    else
    {
        return false;
    }
    assignment_.EraseRangeFromTask(task_ind, std::max(merge_one, merge_two));
    assignment_.EraseRangeFromTask(task_ind, std::min(merge_one, merge_two));
    assignment_.AssignRangeToTask(task_ind, merging_slice);
    hotness[task_ind].push_back(std::max(hotness[task_ind][merge_one], hotness[task_ind][merge_two]));
    hotness[task_ind].erase(hotness[task_ind].begin() + std::max(merge_one, merge_two));
    hotness[task_ind].erase(hotness[task_ind].begin() + std::min(merge_one, merge_two));
    return true;
}
