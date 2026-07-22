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
    //timer += dtime;

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

bool isPlayerAtFurnitureName(std::string_view a_furnitureEditorID)
{
    auto player = RE::PlayerCharacter::GetSingleton();
    if(!player) return false;

    auto furnitureRefHandle = player->GetOccupiedFurniture().get();
    if(!furnitureRefHandle)  return false;
    REX::INFO("[Skill Manager] Gotten player-occupied Furniture reference handle.");

    auto furnitureBase = furnitureRefHandle->GetBaseObject();
    if(!furnitureBase)  return false;
    REX::INFO("[Skill Manager] Gotten furniture Base Object");

    auto furniture = furnitureBase->As<RE::TESFurniture>();
    if(!furniture)  return false;

    if (furniture->HasKeywordString(a_furnitureEditorID))
    {   
        REX::INFO("[Skills Manager] Player is currently at furniture: {}", a_furnitureEditorID);
        return true;    
    }
        

    return false;
}


void SkillsManager::validateCraftingMenu()
{
    REX::INFO("[Skill Manager] Checking currently opened menu with restricted skills");

    auto ui = RE::UI::GetSingleton();
    if(ui && ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
    {
        REX::INFO("[Skill Manager] Crafting Menu confirmed");
        auto craftingMenu = skyrim_cast<RE::CraftingMenu*>(ui->GetMenu(RE::CraftingMenu::MENU_NAME).get());
        if(craftingMenu && craftingMenu->subMenu)
        {
            REX::INFO("[Skill Manager] Crafting SUB Menu initialized.");
            auto msgQueue = RE::UIMessageQueue::GetSingleton();

            //Checking Smithing menus
            if (skillMapTable[4].state == SkillState::Restricted
            && (isPlayerAtFurnitureName("CraftingSmithingForge") //specifically for forge because Forge menu doesnt count in Smithing Menu
            ||  isPlayerAtFurnitureName("CraftingSmithingArmorTable") //alternative for all smithing related: skyrim_cast<RE::CraftingSubMenus::SmithingMenu*>(subMenuPtr)
            ||  isPlayerAtFurnitureName("CraftingSmithingSharpeningWheel")
            ||  isPlayerAtFurnitureName("CraftingSmithingSkyforge")
            )) 
            {
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from Smithing");
                msgQueue->AddMessage(RE::CraftingMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
            } 
            else    //Checking Enchanting menu
            if (skillMapTable[17].state == SkillState::Restricted
            &&  isPlayerAtFurnitureName("isEnchanting")) //alternative: skyrim_cast<RE::CraftingSubMenus::EnchantConstructMenu*>(subMenuPtr))
            {
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from Enchanting");
                msgQueue->AddMessage(RE::CraftingMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
            } 
            else    //Checking Alchemy menu
            if (skillMapTable[10].state == SkillState::Restricted
            &&  isPlayerAtFurnitureName("isAlchemy")) //alternative: skyrim_cast<RE::CraftingSubMenus::AlchemyMenu*>(subMenuPtr)) 
            {
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from doing Alchemy");
                msgQueue->AddMessage(RE::CraftingMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
            }
            else    REX::WARN("[Skill Manager] Failed to check type of current crafting menu.");
        }
    }
}

bool SkillsManager::isLockpickingRestricted()
{
    REX::INFO("[Skill Manager] Checking if Lockpicking skill is restricted...");
    return skillMapTable[8].state == SkillState::Restricted;
}
