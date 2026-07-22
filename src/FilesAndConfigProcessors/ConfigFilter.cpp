#include "ConfigFilter.h"

using namespace ConfigLoader;

// float GetSkillMult(int dividend, int divisor)
// {
//     int quotient = (int)((float)(dividend / divisor) * 100.0); //Cut off to only 2nd point precision of float
//     return (float)quotient / 10000.0;
// }

void ConfigFilter::getSkillConfig(std::array<ConfigFilter::SkillMapping, 18>& skillMap)
{
    Config cfg;
    LoadConfig(cfg);

    //Case 1: when the config itself is empty from the start
    //Only runs if either contains some skill entries
    if(cfg.blessed.empty() && cfg.restricted.empty())   return;

    cleanUnrecognizedSkills(cfg, skillMap);

    //Case 2: when there are no valid skills set and just one or more invalid skills -> Both arrays are EMPTY
    ConfigToSkillMap(skillMap, cfg);
}

//Main filter
void ConfigFilter::ConfigToSkillMap(std::array<ConfigFilter::SkillMapping, 18Ui64> &skillMap, ConfigLoader::Config &cfg)
{
    REX::INFO("[ConfigFilter] Number of set Blessed Skills: {}", cfg.blessed.size());
    REX::INFO("[ConfigFilter] Number of set Restricted Skills: {}", cfg.restricted.size());

    for (auto &skillEntry : skillMap)
    {
        //Case 0: Both arrays are empty as described in Case 2 in function above
        if (cfg.blessed.empty() && cfg.restricted.empty())
        {
            //Case 0.1: User only sets dummy skill in restricted array to bless all skills 
            //          = no specific percentage set in blessed array
            //          = Uses Override for default skill bonus
            if (blessAll)
            {
                REX::INFO("Blessed skill {} set successfully!", skillEntry.skillName);

                skillEntry.mult = cfg.blessOverride;
            }
            
            if (restrictAll)
            {
                skillEntry.state = SkillState::Restricted;
                REX::INFO("Restricted skill {} set successfully!", skillEntry.skillName);
            }
            
            continue;
        }
        

        // Case 1: when the user sets ONLY restricted skills
        if (cfg.blessed.empty())
        {
            if (std::ranges::find(cfg.restricted, skillEntry.skillName) != cfg.restricted.end())
            {
                skillEntry.state = SkillState::Restricted;
                REX::INFO("Restricted skill {} set successfully!", skillEntry.skillName);
            }
            else
            {
                skillEntry.state = SkillState::Blessed;
                skillEntry.mult = cfg.blessOverride;
                REX::INFO("Blessed skill {} set successfully!", skillEntry.skillName);
            }

            continue;
        }

        // Case 2: when the user sets ONLY blessed skills
        if (cfg.restricted.empty())
        {
            if (auto blessedSkill = cfg.blessed.find(skillEntry.skillName);
                blessedSkill != cfg.blessed.end())
            {
                skillEntry.state = SkillState::Blessed;
                REX::INFO("Blessed skill {} set successfully!", skillEntry.skillName);

                if (blessedSkill->second != 0)
                    //User sets specific mult for skill in blessed array
                    skillEntry.mult = blessedSkill->second;//((float)blessedSkill->second) / 100.0;
                else
                {
                    skillEntry.mult = cfg.blessOverride;
                    // //User leaves mult at 0 = Uses Override as default
                    // if(cfg.blessOverride <= 0)  
                    //     skillEntry.mult = 1;    //safely sets mult as 0 if Override is 0 or less
                    // else 
                    //     skillEntry.mult = ((float)cfg.blessOverride) / 100.0;
                }
            }
            else if(!blessAll)  //Can disable setting restricted skill if there is nothing in restricted array other than a dummy skill -> setting only blessed skills with no restrictions
            {
                skillEntry.state = SkillState::Restricted;
                REX::INFO("Restricted skill {} set successfully!", skillEntry.skillName);
            }

            continue;
        }

        // Case 3: Overlap in both arrays -> Eliminate overlapped skill entry in both arrays
        //Finding overlapping skills
        for (auto it = cfg.restricted.begin(); it != cfg.restricted.end();)
        {
            if (auto overlap = cfg.blessed.find(*it);
                overlap != cfg.blessed.end())
            {
                REX::WARN("[Config Filter] Detected overlapping skill [{}] in both Restricted and Blessed. Eliminating overlap...", *it);
                cfg.blessed.erase(overlap);
                it = cfg.restricted.erase(it);
            }
            else
                ++it;
        }

        //Setting restricted skill in Restricted array
        if (std::ranges::find(cfg.restricted, skillEntry.skillName) != cfg.restricted.end())
        {
            skillEntry.state = SkillState::Restricted;
            REX::INFO("Restricted skill {} set successfully!", skillEntry.skillName);
        }

        //Setting blessed skill in Bless array
        if (auto blessedSkill = cfg.blessed.find(skillEntry.skillName);
            blessedSkill != cfg.blessed.end())
        {
            skillEntry.state = SkillState::Blessed;
            REX::INFO("Blessed skill {} set successfully!", skillEntry.skillName);

            //Setting multiplier of blessed skill
            if (blessedSkill->second != 0)
                skillEntry.mult = blessedSkill->second;//((float)blessedSkill->second) / 100.0;
            else
            {
                skillEntry.mult = cfg.blessOverride;
            }
        }
    }
}

void ConfigFilter::cleanUnrecognizedSkills(ConfigLoader::Config& cfg, std::array<ConfigFilter::SkillMapping, 18>& skillMap)
{
    //Reset to false in case player wants to correct any invalid skills in future config updates
    blessAll = false;
    restrictAll = false;

    
    if(!cfg.restricted.empty())
    {
        REX::INFO("[Config Filter] Begin normalizing skill inputs and cleaning invalid skills for Restricted...");
        for(auto it = cfg.restricted.begin(); it != cfg.restricted.end();)
        {
            *it = normalizeString(*it);
            auto foundSkill = std::find_if(skillMap.begin(), skillMap.end(), [&it](const SkillMapping& tempSkill)
            {
                return tempSkill.skillName == *it;
            });

            //if the above algo cant find the skill name even after normalizing;
            if(foundSkill == skillMap.end())    
            {
                REX::WARN("[Config Filter] Invalid skill entry in Restricted skill: {}. Ignoring...", *it);
                it = cfg.restricted.erase(it);
            }
            else    ++it;
        }

        //If there is no valid skill set in Restricted array and this code runs -> dummy skill -> set all skills to blessed
        if(cfg.restricted.empty())  blessAll = true;
        else
            REX::INFO("[Config Filter] Total valid Restricted skills after clean up: {}", cfg.restricted.size());
    }

    if(!cfg.blessed.empty())
    {
        std::unordered_map<std::string, float> tempMap;
        REX::INFO("[Config Filter] Begin normalizing skill inputs and cleaning invalid skills for Blessed...");

        for(const auto& [name, mult] : cfg.blessed)
        {
            //Finding the skill name in the list of all skills
            auto foundSkill = std::find_if(skillMap.begin(), skillMap.end(), [&name](const SkillMapping& tempSkill)
            {
                return tempSkill.skillName == normalizeString(name);
            });
            
            if(foundSkill != skillMap.end())
                tempMap[normalizeString(name)] = mult;
        }

        cfg.blessed.clear();
        cfg.blessed = tempMap;

        //If there is no valid skill set in Blessed array and this code runs -> dummy skill -> set all skills to Restricted
        if(cfg.blessed.empty()) restrictAll = true;
        else
            REX::INFO("[Config Filter] Total valid Blessed skills after clean up: {}", cfg.blessed.size());
    }
}
