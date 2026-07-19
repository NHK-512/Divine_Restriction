#pragma once
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REX/LOG.h>
#include <array>
#include <iterator>
#include "FilesAndConfigProcessors/ConfigLoader.h"
#include "FilesAndConfigProcessors/ConfigFilter.h"
#include "PlayerRestriction/EquippingDetector.h"
using namespace ConfigFilter;

// struct SkillsXP
// {
//     //Combat
//     float oneHanded = -1.0f;
//     float twoHanded = -1.0f;
//     float archery = -1.0f;
//     float lightArm = -1.0f;
//     float heavyArm = -1.0f;
//     float block = -1.0f;
//     //Magic
//     float alteration = -1.0f;
//     float conjuration = -1.0f;
//     float destruction = -1.0f;
//     float illusion = -1.0f;
//     float restoration = -1.0f;
//     //Crafting
//     float alchemy = -1.0f;
//     float enchanting = -1.0f;
//     float smithing = -1.0f;
//     //Utility
//     float Sneak = -1.0f;
//     float lockpicking = -1.0f;
//     float pickpocket = -1.0f;
//     float speech = -1.0f;
// };

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
    
    //ConfigLoader::Config cachedCfg;

    std::array<SkillMapping, 18> skillMapTable
    {{
        {"onehanded",       SkillState::Neutral, 0.0f, 0.0f},   
        {"twohanded",       SkillState::Neutral, 0.0f, 0.0f},   
        {"archery",         SkillState::Neutral, 0.0f, 0.0f},
        {"block",           SkillState::Neutral, 0.0f, 0.0f},       
        {"smithing",        SkillState::Neutral, 0.0f, 0.0f},    
        {"heavyarmor",      SkillState::Neutral, 0.0f, 0.0f},
        {"lightarmor",      SkillState::Neutral, 0.0f, 0.0f},  
        {"pickpocket",      SkillState::Neutral, 0.0f, 0.0f},  
        {"lockpicking",     SkillState::Neutral, 0.0f, 0.0f},
        {"sneak",           SkillState::Neutral, 0.0f, 0.0f},       
        {"alchemy",         SkillState::Neutral, 0.0f, 0.0f},     
        {"speech",          SkillState::Neutral, 0.0f, 0.0f},
        {"alteration",      SkillState::Neutral, 0.0f, 0.0f},  
        {"conjuration",     SkillState::Neutral, 0.0f, 0.0f}, 
        {"destruction",     SkillState::Neutral, 0.0f, 0.0f},
        {"illusion",        SkillState::Neutral, 0.0f, 0.0f},    
        {"restoration",     SkillState::Neutral, 0.0f, 0.0f}, 
        {"enchanting",      SkillState::Neutral, 0.0f, 0.0f}
    }};

    //the earliest delta time when the player just loaded in with skill data
    RE::ActorEquipManager* equipManager;
    float timer = 0.0f;
};