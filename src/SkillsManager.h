#pragma once
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REX/LOG.h>
#include <array>
#include <iterator>
#include <vector>
#include "FilesAndConfigProcessors/ConfigLoader.h"
#include "FilesAndConfigProcessors/ConfigFilter.h"
#include "PlayerRestriction/EquippingDetector.h"
using namespace ConfigFilter;

class SkillsManager
{
public:
    static SkillsManager* GetSingleton()
    {
        static SkillsManager singleton;
        return std::addressof(singleton);
        //code referenced from skyrim-contextual-crosshair
    }
    void LoadConfigs(RE::PlayerCharacter* player);
    void saveSkillsXP(RE::PlayerCharacter* player);
    void refreshMapTable(RE::PlayerCharacter* player);
    void Update(RE::PlayerCharacter* player, float dtime);
    void validateCraftingMenu();

    bool isLockpickingRestricted();
    
    //ConfigLoader::Config cachedCfg;

    std::array<SkillMapping, 18> skillMapTable
    {{
        {"onehanded",       SkillState::Neutral, 0.0f, 0.0f},   // 0   
        {"twohanded",       SkillState::Neutral, 0.0f, 0.0f},   // 1  
        {"archery",         SkillState::Neutral, 0.0f, 0.0f},   // 2
        {"block",           SkillState::Neutral, 0.0f, 0.0f},   // 3     
        {"smithing",        SkillState::Neutral, 0.0f, 0.0f},   // 4   
        {"heavyarmor",      SkillState::Neutral, 0.0f, 0.0f},   // 5
        {"lightarmor",      SkillState::Neutral, 0.0f, 0.0f},   // 6 
        {"pickpocket",      SkillState::Neutral, 0.0f, 0.0f},   // 7 
        {"lockpicking",     SkillState::Neutral, 0.0f, 0.0f},   // 8
        {"sneak",           SkillState::Neutral, 0.0f, 0.0f},   // 9      
        {"alchemy",         SkillState::Neutral, 0.0f, 0.0f},   // 10    
        {"speech",          SkillState::Neutral, 0.0f, 0.0f},   // 11
        {"alteration",      SkillState::Neutral, 0.0f, 0.0f},   // 12 
        {"conjuration",     SkillState::Neutral, 0.0f, 0.0f},   // 13
        {"destruction",     SkillState::Neutral, 0.0f, 0.0f},   // 14
        {"illusion",        SkillState::Neutral, 0.0f, 0.0f},   // 15   
        {"restoration",     SkillState::Neutral, 0.0f, 0.0f},   // 16
        {"enchanting",      SkillState::Neutral, 0.0f, 0.0f}    // 17
    }};

    //the earliest delta time when the player just loaded in with skill data
    RE::ActorEquipManager* equipManager;
    float timer = 0.0f;
};