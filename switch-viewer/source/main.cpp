#include <switch.h>

#include "AchievementPack.hpp"
#include "PackLoader.hpp"
#include "ProfileStore.hpp"
#include "Version.hpp"

#include <algorithm>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>

namespace {
constexpr const char* kPacksPath = "/switch/NXAchievements/packs";
constexpr const char* kProfilePath = "/switch/NXAchievements/profile/unlocked.json";

enum class ScreenState { GameList, GameDetails, Errors };

struct LoadedGame {
    std::string packPath;
    AchievementPack pack;
    std::string loadError;
    bool ok{false};
    int unlockedCount{0};
};

struct PackError {
    std::string packPath;
    std::string error;
};

bool endsWithJson(const std::string& s) {
    if (s.size() < 5) return false;
    std::string suffix = s.substr(s.size() - 5);
    std::transform(suffix.begin(), suffix.end(), suffix.begin(), ::tolower);
    return suffix == ".json";
}

const char* badgeToString(BadgeTier b) {
    switch (b) {
        case BadgeTier::Leaf: return "leaf";
        case BadgeTier::Compass: return "compass";
        case BadgeTier::Bonfire: return "bonfire";
        case BadgeTier::Star: return "star";
    }
    return "unknown";
}

std::vector<std::string> listPackFiles() {
    std::vector<std::string> files;
    DIR* dir = opendir(kPacksPath);
    if (!dir) return files;

    while (dirent* ent = readdir(dir)) {
        if (ent->d_type == DT_DIR) continue;
        std::string name = ent->d_name;
        if (endsWithJson(name)) files.push_back(std::string(kPacksPath) + "/" + name);
    }
    closedir(dir);
    return files;
}

void drawGameList(const std::vector<LoadedGame>& games, int selected, int invalidCount) {
    consoleClear();
    std::cout << "NXAchievements v" << NXACHIEVEMENTS_VERSION << "\n";
    std::cout << "Game List\n";
    std::cout << "=================================\n";
    std::cout << "Packs: " << kPacksPath << "\n";
    std::cout << "Profile: " << kProfilePath << "\n";
    std::cout << "Invalid packs: " << invalidCount << "\n\n";

    if (games.empty()) {
        std::cout << "No valid packs found.\n";
        std::cout << "Place pack JSON files in:\n" << kPacksPath << "\n\n";
    } else {
        for (int i = 0; i < static_cast<int>(games.size()); ++i) {
            const auto& g = games[i];
            std::cout << (i == selected ? "> " : "  ");
            int total = static_cast<int>(g.pack.achievements.size());
            int pct = total > 0 ? (g.unlockedCount * 100) / total : 0;
            std::cout << g.pack.game.name << " (" << g.pack.titleId << ")\n";
            std::cout << "    Unlocked: " << g.unlockedCount << " / " << total << " (" << pct << "%)\n";
        }
        std::cout << "\n";
    }

    std::cout << "Controls: UP/DOWN=Select  A=Details  X=Errors  B=Exit  +=Exit\n";
}

void drawErrorScreen(const std::vector<PackError>& errors, int scroll) {
    consoleClear();
    std::cout << "NXAchievements v" << NXACHIEVEMENTS_VERSION << "\n";
    std::cout << "Pack Validation Errors\n";
    std::cout << "======================\n\n";

    if (errors.empty()) {
        std::cout << "No errors.\n\n";
    } else {
        const int pageSize = 10;
        int start = std::max(0, scroll);
        int end = std::min(static_cast<int>(errors.size()), start + pageSize);
        for (int i = start; i < end; ++i) {
            std::cout << "- " << errors[i].packPath << "\n";
            std::cout << "  " << errors[i].error << "\n\n";
        }
        std::cout << "Showing " << (start + 1) << "-" << end << " of " << errors.size() << "\n\n";
    }

    std::cout << "Controls: UP/DOWN=Scroll  B=Back  +=Exit\n";
}

void drawGameDetails(const LoadedGame& g, const ProfileStore& profile, int scroll) {
    consoleClear();

    int total = static_cast<int>(g.pack.achievements.size());
    int pct = total > 0 ? (g.unlockedCount * 100) / total : 0;

    std::cout << g.pack.game.name << "\n";
    std::cout << "Title ID: " << g.pack.titleId << "\n";
    std::cout << "Unlocked: " << g.unlockedCount << " / " << total << " (" << pct << "%)\n";
    std::cout << "----------------------------------------\n";

    const int pageSize = 11;
    int start = std::max(0, scroll);
    int end = std::min(total, start + pageSize);

    for (int i = start; i < end; ++i) {
        const auto& a = g.pack.achievements[i];
        bool isUnlocked = profile.isUnlocked(a.id);
        std::cout << "[" << (isUnlocked ? "\u2713" : " ") << "] ";
        std::cout << "(" << badgeToString(a.badge) << ") " << a.name << "\n";
        if (!a.description.empty()) {
            std::cout << "    " << a.description << "\n";
        }
    }

    std::cout << "\nShowing " << (total == 0 ? 0 : start + 1) << "-" << end << " of " << total << "\n";
    std::cout << "Controls: UP/DOWN=Scroll  B=Back  +=Exit\n";
}

}  // namespace

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    consoleInit(nullptr);

    ProfileStore profile;
    std::string profileError;
    profile.load(kProfilePath, profileError); // missing file => empty profile

    auto packFiles = listPackFiles();
    std::vector<LoadedGame> games;
    std::vector<PackError> errors;

    for (const auto& packPath : packFiles) {
        LoadedGame g;
        g.packPath = packPath;
        g.ok = loadAchievementPack(packPath, g.pack, g.loadError);
        if (g.ok) {
            int unlocked = 0;
            for (const auto& a : g.pack.achievements) {
                if (profile.isUnlocked(a.id)) unlocked++;
            }
            g.unlockedCount = unlocked;
            games.push_back(std::move(g));
        } else {
            errors.push_back({packPath, g.loadError});
        }
    }

    std::sort(games.begin(), games.end(), [](const LoadedGame& a, const LoadedGame& b) {
        return a.pack.game.name < b.pack.game.name;
    });

    ScreenState screen = ScreenState::GameList;
    int selected = 0;
    int detailsScroll = 0;
    int errorScroll = 0;
    bool running = true;

    while (appletMainLoop() && running) {
        switch (screen) {
            case ScreenState::GameList:
                drawGameList(games, selected, static_cast<int>(errors.size()));
                break;
            case ScreenState::GameDetails:
                if (!games.empty()) drawGameDetails(games[selected], profile, detailsScroll);
                break;
            case ScreenState::Errors:
                drawErrorScreen(errors, errorScroll);
                break;
        }

        hidScanInput();
        u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

        if (kDown & KEY_PLUS) {
            running = false;
        } else if (screen == ScreenState::GameList) {
            if ((kDown & KEY_DOWN) && !games.empty()) selected = (selected + 1) % static_cast<int>(games.size());
            if ((kDown & KEY_UP) && !games.empty()) selected = (selected - 1 + static_cast<int>(games.size())) % static_cast<int>(games.size());
            if ((kDown & KEY_A) && !games.empty()) {
                screen = ScreenState::GameDetails;
                detailsScroll = 0;
            }
            if (kDown & KEY_X) {
                screen = ScreenState::Errors;
                errorScroll = 0;
            }
            if (kDown & KEY_B) running = false;
        } else if (screen == ScreenState::GameDetails) {
            int total = games.empty() ? 0 : static_cast<int>(games[selected].pack.achievements.size());
            int maxScroll = std::max(0, total - 11);
            if ((kDown & KEY_DOWN) && detailsScroll < maxScroll) detailsScroll++;
            if ((kDown & KEY_UP) && detailsScroll > 0) detailsScroll--;
            if (kDown & KEY_B) screen = ScreenState::GameList;
        } else if (screen == ScreenState::Errors) {
            int maxScroll = std::max(0, static_cast<int>(errors.size()) - 10);
            if ((kDown & KEY_DOWN) && errorScroll < maxScroll) errorScroll++;
            if ((kDown & KEY_UP) && errorScroll > 0) errorScroll--;
            if (kDown & KEY_B) screen = ScreenState::GameList;
        }

        consoleUpdate(nullptr);
    }

    consoleExit(nullptr);
    return 0;
}
