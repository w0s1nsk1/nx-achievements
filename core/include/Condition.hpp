#pragma once
#include <string>
#include <variant>

enum class ConditionType {
    StateEquals,
    StateGreaterThan,
    StateLessThan,
    StateBitSet
};

using ConditionValue = std::variant<bool, double, std::string, int>;

struct Condition {
    ConditionType type;
    std::string key;
    ConditionValue value;
};
