#include "PackLoader.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

struct Finding { std::string level, msg; };

static bool isHexUpper(const std::string& s) {
    static const std::regex re("^[0-9A-F]+$");
    return std::regex_match(s, re);
}

static void validateFile(const fs::path& p, std::vector<Finding>& out) {
    std::ifstream in(p);
    if (!in) { out.push_back({"ERROR", "Cannot open file"}); return; }

    json j;
    try { in >> j; }
    catch (const std::exception& e) { out.push_back({"ERROR", std::string("Malformed JSON: ") + e.what()}); return; }

    std::set<std::string> knownRoot{"schema_version","title_id","game","supported_versions","achievements"};
    for (auto it=j.begin(); it!=j.end(); ++it) if (!knownRoot.count(it.key())) out.push_back({"WARN", "Unknown root field: \""+it.key()+"\""});

    AchievementPack pack;
    std::string err;
    if (!loadAchievementPack(p.string(), pack, err)) {
        out.push_back({"ERROR", err});
        return;
    }

    if (pack.achievements.empty()) out.push_back({"ERROR", "Achievements array is empty"});

    if (!(pack.titleId.size()==16 && isHexUpper(pack.titleId))) out.push_back({"ERROR", "Invalid title_id format (expected 16-char uppercase hex)"});

    for (const auto& v : pack.supportedVersions) {
        if (!v.buildId.empty() && !(v.buildId.size()>=8 && isHexUpper(v.buildId))) {
            out.push_back({"WARN", "build_id looks unusual: " + v.buildId});
        }
    }

    std::set<std::string> ids;
    for (const auto& a : pack.achievements) {
        if (!ids.insert(a.id).second) out.push_back({"ERROR", "Duplicate achievement id: " + a.id});

        std::set<std::string> condKeys;
        for (const auto& c : a.conditions) {
            std::string key = std::to_string((int)c.type) + ":" + c.key;
            if (!condKeys.insert(key).second) out.push_back({"WARN", "Possible duplicate condition in achievement: " + a.id});
        }
    }
}

int main(int argc, char** argv) {
    bool verbose=false, quiet=false, jsonOut=false;
    std::vector<std::string> files;
    for (int i=1;i<argc;++i){
        std::string a=argv[i];
        if (a=="--verbose") verbose=true;
        else if (a=="--quiet") quiet=true;
        else if (a=="--json") jsonOut=true;
        else files.push_back(a);
    }
    if (files.empty()) {
        std::cerr << "Usage: pack-validator [--verbose|--quiet|--json] <pack1.json> [pack2.json ...]\n";
        return 2;
    }

    int failures=0;
    json report=json::array();

    for (const auto& f : files) {
        std::vector<Finding> findings;
        validateFile(f, findings);

        bool hasErr=false;
        for (auto& x:findings) if (x.level=="ERROR") hasErr=true;
        if (hasErr) failures++;

        if (jsonOut) {
            json item;
            item["file"]=f;
            item["status"]=hasErr?"ERROR":"OK";
            item["findings"]=json::array();
            for (auto& x:findings) item["findings"].push_back({{"level",x.level},{"message",x.msg}});
            report.push_back(item);
            continue;
        }

        if (!quiet) std::cout << "[" << (hasErr?"ERROR":"OK") << "] " << f << "\n";
        for (auto& x:findings) {
            if (quiet && x.level=="WARN") continue;
            if (!verbose && x.level=="WARN") continue;
            std::cout << "[" << x.level << "] " << x.msg << "\n";
        }
    }

    if (jsonOut) std::cout << report.dump(2) << "\n";
    return failures>0 ? 1 : 0;
}
