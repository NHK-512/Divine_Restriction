#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include "ConfigLoader.h"
#include <unordered_map>
#include <array>
#include <string_view>
#include <algorithm>
#include <cctype>
#include <string>
#define MAX_SKYRIM_SKILL_COUNTS 18

namespace ConfigFilter
{
    enum class SkillState
    {
        Neutral,
        Blessed,
        Restricted
    };

    struct SkillMapping
    {
        std::string skillName;
        SkillState state = SkillState::Neutral;
        float cachedXP = 0.0f;
        float mult = 0.0f;  //if this skill is restricted, this value is -1.0
    };

    // Helper to remove spaces/hyphens and convert to lowercase
    inline std::string normalizeString(std::string_view input) {
        std::string result;
        result.reserve(input.size());
        
        for (char c : input) {
            if (c != ' ' && c != '-') {
                result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
            }
        }
        return result;
    }

    void getSkillConfig(std::array<SkillMapping, 18> &skillMap);
    void ConfigToSkillMap(std::array<ConfigFilter::SkillMapping, 18Ui64> &skillMap, ConfigLoader::Config &cfg);
    void cleanUnrecognizedSkills(ConfigLoader::Config& cfg, std::array<SkillMapping, 18>& skillMap);
    
    inline bool blessAll = false;
    inline bool restrictAll = false;
}
