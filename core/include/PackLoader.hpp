#pragma once
#include "AchievementPack.hpp"
#include <string>

bool loadAchievementPack(const std::string& path, AchievementPack& outPack, std::string& error);
