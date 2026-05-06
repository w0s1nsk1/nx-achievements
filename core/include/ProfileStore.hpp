#pragma once
#include <string>
#include <unordered_map>

struct UnlockedEntry {
    std::string unlockedAt;
};

class ProfileStore {
public:
    bool load(const std::string& path, std::string& error);
    bool save(const std::string& path, std::string& error) const;

    bool isUnlocked(const std::string& achievementId) const;
    bool unlock(const std::string& achievementId, const std::string& timestampIso8601);

    const std::unordered_map<std::string, UnlockedEntry>& unlocked() const { return unlocked_; }

private:
    std::unordered_map<std::string, UnlockedEntry> unlocked_;
};
