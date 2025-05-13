#include "common.hpp"
#include <cstdlib>
#include <random>

std::string key_from_number(int number);

std::string SimpleRandom(std::mt19937 &gen)
{
    std::uniform_int_distribution<> dis(0, MAX_KEY);
    return key_from_number(dis(gen));
}
std::string SimpleHot(std::mt19937 &gen)
{
    std::normal_distribution<float> dis((double)MAX_KEY / 2.0, sqrt((double)MAX_KEY / 2.0));
    return key_from_number(std::max((size_t)0, std::min(MAX_KEY, (size_t)dis(gen))));
}
std::string SimpleWave(std::mt19937 &gen)
{
    static size_t mean_value = 0;
    static int addition = 100;
    std::normal_distribution<float> dis(mean_value, sqrt(MAX_KEY / 2));
    mean_value = mean_value + addition;
    if (mean_value >= MAX_KEY - addition)
    {
        addition *= -1;
    }

    return key_from_number(std::max((size_t)0, std::min(MAX_KEY, (size_t)dis(gen))));
}

std::string GenKey()
{
    std::string result;
    std::random_device rd;
    std::mt19937 gen(rd());
    do
    {
        const char *env_p = std::getenv("KEY_DISTRIBUTION");
        if (env_p && std::string(env_p) == "0")
        {
            result = SimpleRandom(gen);
        }
        else if (env_p && std::string(env_p) == "1")
        {
            result = SimpleHot(gen);
        }
        else if (env_p && std::string(env_p) == "2")
        {
            result = SimpleWave(gen);
        }
        else
        {
            throw std::runtime_error("can't understand the KEY_DISTRIBUTION");
        }
    } while (result == std::string(KEY_SIZE, '9'));
    return result;
}
std::string key_from_number(int number)
{
    std::string result(KEY_SIZE, '0');

    for (size_t i = 0; i < KEY_SIZE; ++i)
    {
        result[i] += (number / (int)pow(10, KEY_SIZE - i - 1)) % 10;
    }
    return result;
}

std::pair<StringRange, StringRange> SplitKeys(StringRange range)
{
    int left = atoi(range.first.c_str());
    int right = atoi(range.second.c_str());
    std::string middle = key_from_number(left + (right - left) / 2);
    return std::make_pair(std::make_pair(range.first, middle), std::make_pair(middle, range.second));
}

StringRange MergeKeys(StringRange left, StringRange right)
{
    if (left.second != right.first)
    {
        throw std::runtime_error("tried to merge not adjence ranges");
    }
    return std::make_pair(left.first, right.second);
}
