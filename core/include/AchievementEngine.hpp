#pragma once
#include "AchievementPack.hpp"
#include "ProfileStore.hpp"
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

using MockStateValue = std::variant<bool, double, std::string, int>;
using MockState = std::unordered_map<std::string, MockStateValue>;

struct UnlockResult {
    std::string achievementId;
    std::string achievementName;
};

class AchievementEngine {
public:
    std::vector<UnlockResult> evaluateAndUnlock(
        const AchievementPack& pack,
        const MockState& state,
        ProfileStore& profile,
        const std::string& timestampIso8601);
};

bool evaluateCondition(const Condition& cond, const MockState& state);
