#pragma once
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REX/LOG.h>
#include <vector>
#include "SkillsManager.h"
#include "PlayerRestriction/MenuDetector.h"

class MainHook {
    static void PlayerUpdate(RE::PlayerCharacter* a, float dtime);
    static inline REL::Relocation<decltype(PlayerUpdate)> _PlayerUpdate;


public:
    static void Hook();
    static MainHook& Get()
        {
            static MainHook instance;
            return instance;
        }

    static inline SkillsManager* manager;
};