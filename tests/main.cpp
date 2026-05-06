#include "AchievementEngine.hpp"
#include "PackLoader.hpp"
#include "ProfileStore.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

static void writeFile(const fs::path& p, const std::string& content) {
    fs::create_directories(p.parent_path());
    std::ofstream out(p);
    out << content;
}

static AchievementPack loadPackOrFail(const fs::path& p) {
    AchievementPack pack;
    std::string err;
    bool ok = loadAchievementPack(p.string(), pack, err);
    if (!ok) {
        std::cerr << "Unexpected pack load error: " << err << "\n";
    }
    assert(ok);
    return pack;
}

static void test_unlocks_when_conditions_match(const fs::path& tempDir) {
    auto pack = loadPackOrFail("examples/packs/0100000000000000.json");

    ProfileStore profile;
    std::string err;
    assert(profile.load((tempDir / "profile1.json").string(), err));

    MockState state;
    state["has_started_game"] = true;
    state["player_level"] = 5.0;
    state["flags"] = 0b100;

    AchievementEngine engine;
    auto unlocked = engine.evaluateAndUnlock(pack, state, profile, "2026-01-01T00:00:00Z");
    assert(!unlocked.empty());
}

static void test_does_not_unlock_duplicate_twice(const fs::path& tempDir) {
    auto pack = loadPackOrFail("examples/packs/0100000000000000.json");

    ProfileStore profile;
    std::string err;
    assert(profile.load((tempDir / "profile2.json").string(), err));

    MockState state;
    state["has_started_game"] = true;
    state["player_level"] = 5.0;
    state["flags"] = 0b100;

    AchievementEngine engine;
    auto first = engine.evaluateAndUnlock(pack, state, profile, "2026-01-01T00:00:00Z");
    auto second = engine.evaluateAndUnlock(pack, state, profile, "2026-01-01T00:01:00Z");

    assert(!first.empty());
    assert(second.empty());
}

static void test_does_not_unlock_when_conditions_fail(const fs::path& tempDir) {
    auto pack = loadPackOrFail("examples/packs/0100000000000000.json");

    ProfileStore profile;
    std::string err;
    assert(profile.load((tempDir / "profile3.json").string(), err));

    MockState state;
    state["has_started_game"] = false;
    state["player_level"] = 1.0;
    state["flags"] = 0;

    AchievementEngine engine;
    auto unlocked = engine.evaluateAndUnlock(pack, state, profile, "2026-01-01T00:00:00Z");
    assert(unlocked.empty());
}

static void test_handles_malformed_pack_json_with_clear_error(const fs::path& tempDir) {
    fs::path p = tempDir / "malformed.json";
    writeFile(p, "{ this is not json ");

    AchievementPack pack;
    std::string err;
    bool ok = loadAchievementPack(p.string(), pack, err);
    assert(!ok);
    assert(err.find("Malformed pack") != std::string::npos || err.find("Cannot open") != std::string::npos);
}

static void test_handles_unknown_condition_type_gracefully(const fs::path& tempDir) {
    fs::path p = tempDir / "unknown_condition.json";
    writeFile(p, R"({
  "schema_version": 1,
  "title_id": "0100000000000000",
  "game": {"name": "X", "developer": "Y"},
  "supported_versions": [],
  "achievements": [
    {
      "id": "a1",
      "name": "A1",
      "description": "desc",
      "badge": "leaf",
      "hidden": false,
      "conditions": [
        {"type": "state_unknown", "key": "k", "value": true}
      ]
    }
  ]
})");

    AchievementPack pack;
    std::string err;
    bool ok = loadAchievementPack(p.string(), pack, err);
    assert(!ok);
    assert(err.find("Unknown condition type") != std::string::npos);
}

static void test_creates_profile_file_if_missing(const fs::path& tempDir) {
    fs::path p = tempDir / "new_profile.json";
    if (fs::exists(p)) fs::remove(p);

    ProfileStore profile;
    std::string err;
    assert(profile.load(p.string(), err));
    assert(!fs::exists(p));

    assert(profile.unlock("first_steps", "2026-01-01T00:00:00Z"));
    assert(profile.save(p.string(), err));
    assert(fs::exists(p));
}

int main() {
    fs::path tempDir = fs::temp_directory_path() / "nxachievements-tests";
    fs::create_directories(tempDir);

    test_unlocks_when_conditions_match(tempDir);
    test_does_not_unlock_duplicate_twice(tempDir);
    test_does_not_unlock_when_conditions_fail(tempDir);
    test_handles_malformed_pack_json_with_clear_error(tempDir);
    test_handles_unknown_condition_type_gracefully(tempDir);
    test_creates_profile_file_if_missing(tempDir);

    std::cout << "All tests passed.\n";
    return 0;
}
