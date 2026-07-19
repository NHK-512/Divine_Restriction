#pragma once
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REX/LOG.h>
#include "SkillsManager.h"

class MainHook {
    static void PlayerUpdate(RE::PlayerCharacter* a, float dtime);
    static inline REL::Relocation<decltype(PlayerUpdate)> _PlayerUpdate;

public:
    static void Hook();

    static inline SkillsManager* manager;
};