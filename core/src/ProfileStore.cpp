#include "ProfileStore.hpp"
#include <filesystem>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

bool ProfileStore::load(const std::string& path, std::string& error) {
    unlocked_.clear();

    if (!std::filesystem::exists(path)) {
        return true;
    }

    std::ifstream in(path);
    if (!in) {
        error = "Cannot open profile file: " + path;
        return false;
    }

    json j;
    try {
        in >> j;
        if (!j.contains("unlocked") || !j["unlocked"].is_object()) {
            error = "Malformed profile: missing 'unlocked' object";
            return false;
        }

        for (auto it = j["unlocked"].begin(); it != j["unlocked"].end(); ++it) {
            if (!it.value().contains("unlocked_at") || !it.value()["unlocked_at"].is_string()) continue;
            unlocked_[it.key()] = {it.value()["unlocked_at"].get<std::string>()};
        }
    } catch (const std::exception& e) {
        error = std::string("Failed to parse profile: ") + e.what();
        return false;
    }

    return true;
}

bool ProfileStore::save(const std::string& path, std::string& error) const {
    json j;
    j["schema_version"] = 1;
    j["unlocked"] = json::object();

    for (const auto& [id, entry] : unlocked_) {
        j["unlocked"][id] = { {"unlocked_at", entry.unlockedAt} };
    }

    std::filesystem::create_directories(std::filesystem::path(path).parent_path());
    std::ofstream out(path);
    if (!out) {
        error = "Cannot write profile file: " + path;
        return false;
    }

    out << j.dump(2) << "\n";
    return true;
}

bool ProfileStore::isUnlocked(const std::string& achievementId) const {
    return unlocked_.find(achievementId) != unlocked_.end();
}

bool ProfileStore::unlock(const std::string& achievementId, const std::string& timestampIso8601) {
    if (isUnlocked(achievementId)) return false;
    unlocked_[achievementId] = {timestampIso8601};
    return true;
}
