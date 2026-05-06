#pragma once
#include "Achievement.hpp"
#include <string>
#include <vector>

struct SupportedVersion {
    std::string gameVersion;
    std::string buildId;
};

struct GameMetadata {
    std::string name;
    std::string developer;
};

struct AchievementPack {
    int schemaVersion{1};
    std::string titleId;
    GameMetadata game;
    std::vector<SupportedVersion> supportedVersions;
    std::vector<Achievement> achievements;
};
