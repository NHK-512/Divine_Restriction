#include "SkillsManager.h"

void SkillsManager::saveSkillsXP(RE::PlayerCharacter* player)
{
    for(auto it = skillMapTable.begin(); it != skillMapTable.end(); ++it)
    {
        auto index = std::distance(skillMapTable.begin(), it);
        if(it->state != ConfigFilter::SkillState::Neutral)
        {
            it->cachedXP = player->skills->data->skills[index].xp;
        }
    }
}

void SkillsManager::refreshMapTable(RE::PlayerCharacter *player)
{
    for(auto it = skillMapTable.begin(); it != skillMapTable.end(); ++it)
    {
        it->cachedXP = 0.0f;
        it->mult = 0.0f;
        it->state = SkillState::Neutral;
    }
}

void SkillsManager::LoadConfigs(RE::PlayerCharacter* player)
{
    if(!ConfigLoader::IfConfigChanged())    return;     //Due to how IfConfigChanged() is made, it will always return true the first time

    RE::SendHUDMessage::ShowHUDMessage("Divine Restriction Configs Loaded!");

    refreshMapTable(player);
    ConfigFilter::getSkillConfig(skillMapTable);
    saveSkillsXP(player);  

    REX::INFO("[Skills Manager] Restricted Skills: ");
    for(const auto& skill : skillMapTable)
    {
        if(skill.state == ConfigFilter::SkillState::Restricted)
        {
            REX::INFO("Name: {} | XP is capped at: {:.2f}", skill.skillName, skill.cachedXP);
        }
    }

    REX::INFO("[Skills Manager] Blessed Skills: ");
    for(const auto& skill : skillMapTable)
    {
        if(skill.state == ConfigFilter::SkillState::Blessed)
        {
            REX::INFO("Name: {} | Bonus: {:.2f}", skill.skillName, skill.mult);
        }
    }
}

void SkillsManager::Update(RE::PlayerCharacter *player, float dtime)
{
    LoadConfigs(player);
    //this makes the code below runs once every second
    timer += dtime;

    if (!equipManager)
    {
        equipManager = RE::ActorEquipManager::GetSingleton();
    }
    

    for(auto it = skillMapTable.begin(); it != skillMapTable.end(); ++it)
    {
        auto index = std::distance(skillMapTable.begin(), it);
        if (it->state == ConfigFilter::SkillState::Restricted)
        {
            EquippingDetector::isViolatingRestriction(index, equipManager);   //Checking restriction violation 
        }

        auto& currentXP = player->skills->data->skills[index].xp;
        auto xpGain = currentXP - it->cachedXP;
        if(xpGain > 0.0f)
        {
            if(it->state == ConfigFilter::SkillState::Restricted)
            {
                currentXP = it->cachedXP;
                REX::INFO("[SkillManager] Restricted skill {}'s XP is at = {:.2f}", it->skillName, currentXP);
            }

            if(it->state == ConfigFilter::SkillState::Blessed)
            {
                REX::INFO("[SkillManager] Blessed Skill {}'s current XP = {:.2f}", it->skillName, it->cachedXP);
                REX::INFO("[SkillManager] Blessed Skill {}'s base XP gain = {:.2f}", it->skillName, xpGain);
                currentXP = it->cachedXP + (xpGain * it->mult);
                
                REX::INFO("[SkillManager] Blessed Skill {}'s XP gain after bonus = {:.2f}", it->skillName, (currentXP - it->cachedXP));
                it->cachedXP = currentXP;
            }
        } 

        if(xpGain < 0.0f)
        {
            it->cachedXP = currentXP;
        }
    }
}