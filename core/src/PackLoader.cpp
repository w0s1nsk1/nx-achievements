#include "PackLoader.hpp"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

static BadgeTier parseBadge(const std::string& b) {
    if (b == "leaf") return BadgeTier::Leaf;
    if (b == "compass") return BadgeTier::Compass;
    if (b == "bonfire") return BadgeTier::Bonfire;
    if (b == "star") return BadgeTier::Star;
    throw std::runtime_error("Unknown badge tier: " + b);
}

static ConditionType parseConditionType(const std::string& t) {
    if (t == "state_equals") return ConditionType::StateEquals;
    if (t == "state_greater_than") return ConditionType::StateGreaterThan;
    if (t == "state_less_than") return ConditionType::StateLessThan;
    if (t == "state_bit_set") return ConditionType::StateBitSet;
    throw std::runtime_error("Unknown condition type: " + t);
}

bool loadAchievementPack(const std::string& path, AchievementPack& outPack, std::string& error) {
    std::ifstream in(path);
    if (!in) {
        error = "Cannot open pack file: " + path;
        return false;
    }

    json j;
    try {
        in >> j;

        if (!j.contains("schema_version")) {
            error = "Validation error: missing required field 'schema_version'";
            return false;
        }
        if (!j.contains("title_id")) {
            error = "Validation error: missing required field 'title_id'";
            return false;
        }
        if (!j.contains("achievements")) {
            error = "Validation error: missing required field 'achievements'";
            return false;
        }

        outPack.schemaVersion = j.at("schema_version").get<int>();
        outPack.titleId = j.at("title_id").get<std::string>();
        outPack.game.name = j.at("game").at("name").get<std::string>();
        outPack.game.developer = j.at("game").at("developer").get<std::string>();

        outPack.supportedVersions.clear();
        for (const auto& v : j.at("supported_versions")) {
            outPack.supportedVersions.push_back({
                v.at("game_version").get<std::string>(),
                v.at("build_id").get<std::string>()
            });
        }

        outPack.achievements.clear();
        for (const auto& a : j.at("achievements")) {
            if (!a.contains("id")) {
                error = "Validation error: achievement missing required field 'id'";
                return false;
            }
            if (!a.contains("name")) {
                error = "Validation error: achievement missing required field 'name'";
                return false;
            }
            if (!a.contains("badge")) {
                error = "Validation error: achievement missing required field 'badge'";
                return false;
            }
            if (!a.contains("conditions")) {
                error = "Validation error: achievement missing required field 'conditions'";
                return false;
            }

            Achievement ach;
            ach.id = a.at("id").get<std::string>();
            ach.name = a.at("name").get<std::string>();
            ach.description = a.value("description", "");
            ach.badge = parseBadge(a.at("badge").get<std::string>());
            ach.hidden = a.value("hidden", false);

            for (const auto& c : a.at("conditions")) {
                if (!c.contains("type")) {
                    error = "Validation error: condition missing required field 'type'";
                    return false;
                }
                Condition cond;
                cond.type = parseConditionType(c.at("type").get<std::string>());
                cond.key = c.at("key").get<std::string>();

                if (!c.contains("value")) throw std::runtime_error("Condition missing 'value'");

                if (c["value"].is_boolean()) cond.value = c["value"].get<bool>();
                else if (c["value"].is_number_float()) cond.value = c["value"].get<double>();
                else if (c["value"].is_number_integer()) {
                    if (cond.type == ConditionType::StateBitSet) cond.value = c["value"].get<int>();
                    else cond.value = static_cast<double>(c["value"].get<int>());
                }
                else if (c["value"].is_string()) cond.value = c["value"].get<std::string>();
                else throw std::runtime_error("Unsupported condition value type");

                ach.conditions.push_back(cond);
            }

            outPack.achievements.push_back(std::move(ach));
        }

    } catch (const std::exception& e) {
        error = std::string("Malformed pack: ") + e.what();
        return false;
    }

    return true;
}
