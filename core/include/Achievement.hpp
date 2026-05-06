#pragma once
#include "Condition.hpp"
#include <string>
#include <vector>

enum class BadgeTier {
    Leaf,
    Compass,
    Bonfire,
    Star
};

struct Achievement {
    std::string id;
    std::string name;
    std::string description;
    BadgeTier badge;
    bool hidden{false};
    std::vector<Condition> conditions;
};
