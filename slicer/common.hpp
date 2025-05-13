#pragma once
#include <chrono>
#include <stdexcept>
#include <cmath>
#include <string>
#include <vector>
#include <mutex>
#include <memory>

const size_t KEY_SIZE = 5;
const size_t MAX_KEY = pow(10, KEY_SIZE) - 1;
const size_t TASKS_CNT = 5;
const size_t CLIENT_CNT = 10;
const std::chrono::seconds TIME_OF_EXPERIMENT = std::chrono::seconds(20);

std::string GenKey();
std::string key_from_number(int number);
typedef std::pair<std::string, std::string> StringRange;

std::pair<StringRange, StringRange> SplitKeys(StringRange range);

StringRange MergeKeys(StringRange left, StringRange right);
typedef std::vector<StringRange> StringCollection;

class TaskImpl;
typedef std::shared_ptr<TaskImpl> Task;