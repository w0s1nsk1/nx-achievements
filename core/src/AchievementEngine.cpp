#include "AchievementEngine.hpp"

std::vector<UnlockResult> AchievementEngine::evaluateAndUnlock(
    const AchievementPack& pack,
    const MockState& state,
    ProfileStore& profile,
    const std::string& timestampIso8601) {

    std::vector<UnlockResult> unlockedNow;

    for (const auto& a : pack.achievements) {
        if (profile.isUnlocked(a.id)) continue;

        bool allOk = true;
        for (const auto& c : a.conditions) {
            if (!evaluateCondition(c, state)) {
                allOk = false;
                break;
            }
        }

        if (allOk && profile.unlock(a.id, timestampIso8601)) {
            unlockedNow.push_back({a.id, a.name});
        }
    }

    return unlockedNow;
}
