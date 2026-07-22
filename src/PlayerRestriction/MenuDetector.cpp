#include "MenuDetector.h"

RE::BSEventNotifyControl MenuDetector::ProcessEvent(const RE::MenuOpenCloseEvent *a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent> *)
{
    if (a_event && a_event->opening) {
        REX::INFO("[Menu Detector] Menu opening detected.");

        auto menuName = a_event->menuName;
        auto& mainHook = MainHook::Get();

        // Standard menus like Lockpicking are ready instantly
        if (menuName == RE::LockpickingMenu::MENU_NAME
        &&  mainHook.manager->isLockpickingRestricted()) 
        {
            RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from Lockpicking");
            RE::UIMessageQueue::GetSingleton()->AddMessage(RE::LockpickingMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
        }
        // Crafting menu needs a 1-frame delay for sub-menus to load
        else if (menuName == RE::CraftingMenu::MENU_NAME) 
        {
            auto player = RE::PlayerCharacter::GetSingleton();
            auto sitState = player->GetSitSleepState();
            if (sitState == RE::SIT_SLEEP_STATE::kNormal)
            {
                REX::INFO("[Menu Detector] Crafting menu is opening when player is not at any kind workbench. Allowing menu to open...");
                return RE::BSEventNotifyControl::kContinue;
            }


            REX::INFO("[Menu Detector] Current menu is crafting menu. Checking with [Skill Manager]");
            mainHook.manager->validateCraftingMenu();
        }
    }

    return RE::BSEventNotifyControl::kContinue;
}