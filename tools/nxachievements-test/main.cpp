#include "AchievementEngine.hpp"
#include "PackLoader.hpp"
#include "ProfileStore.hpp"
#include "Version.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

static std::string nowIso8601() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#ifdef _WIN32
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <pack.json> <profile.json>\n";
        return 1;
    }

    const std::string packPath = argv[1];
    const std::string profilePath = argv[2];

    AchievementPack pack;
    std::string error;
    if (!loadAchievementPack(packPath, pack, error)) {
        std::cerr << "Pack load error: " << error << "\n";
        return 1;
    }

    ProfileStore profile;
    if (!profile.load(profilePath, error)) {
        std::cerr << "Profile load error: " << error << "\n";
        return 1;
    }

    MockState state;
    state["has_started_game"] = true;
    state["player_level"] = 5.0;
    state["damage_taken"] = 2.0;
    state["flags"] = 0b100;

    AchievementEngine engine;
    auto unlocked = engine.evaluateAndUnlock(pack, state, profile, nowIso8601());

    if (!profile.save(profilePath, error)) {
        std::cerr << "Profile save error: " << error << "\n";
        return 1;
    }

    std::cout << "NXAchievements CLI v" << NXACHIEVEMENTS_VERSION << "\n";
    std::cout << "Loaded pack for: " << pack.game.name << " (" << pack.titleId << ")\n";
    std::cout << "Newly unlocked achievements: " << unlocked.size() << "\n";
    for (const auto& u : unlocked) {
        std::cout << " - " << u.achievementId << " :: " << u.achievementName << "\n";
    }

    if (unlocked.empty()) {
        std::cout << "No new unlocks this run.\n";
    }

    return 0;
}
