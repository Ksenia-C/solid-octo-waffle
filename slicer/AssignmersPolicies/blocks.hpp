#pragma once
#include <vector>

class Assignment;

void Move(std::vector<std::vector<double>> &hotness, Assignment &assignment_, size_t ind_from, size_t ind, size_t ind_to);
void Copy(std::vector<std::vector<double>> &hotness, Assignment &assignment_, size_t ind_from, size_t ind, size_t ind_to);

void Split(std::vector<std::vector<double>> &hotness, Assignment &assignment_, size_t task_ind_split, size_t split_slice);
bool Merge(std::vector<std::vector<double>> &hotness, Assignment &assignment_, size_t task_ind, size_t merge_one, size_t merge_two);
