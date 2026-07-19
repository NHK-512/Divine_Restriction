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
    
    _PlayerUpdate = REL::Relocation<uintptr_t>(RE::VTABLE_PlayerCharacter[0]).write_vfunc(0xAD, PlayerUpdate);
}
