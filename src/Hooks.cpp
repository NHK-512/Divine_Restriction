#include "Hooks.h"

void MainHook::PlayerUpdate(RE::PlayerCharacter* player, float dtime)
{
    _PlayerUpdate(player, dtime);
    if(player && player->Is3DLoaded() && manager)
        manager->Update(player, dtime);

}

void MainHook::Hook()
{
    manager = SkillsManager::GetSingleton();
    REX::INFO("[Hooks] Skill Manager initialized!");

    auto ui = RE::UI::GetSingleton();
    if(ui)
    {
        ui->GetEventSource<RE::MenuOpenCloseEvent>()->AddEventSink(MenuDetector::GetSingleton());
        REX::INFO("[Hooks] Menu event sink added");
    }
    
    _PlayerUpdate = REL::Relocation<uintptr_t>(RE::VTABLE_PlayerCharacter[0]).write_vfunc(0xAD, PlayerUpdate);
}
