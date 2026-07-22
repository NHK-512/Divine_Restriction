#pragma once
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REX/LOG.h>
#include "../Hooks.h"

class MenuDetector : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
private:
    MenuDetector() = default;
    ~MenuDetector() override = default;
    MenuDetector(const MenuDetector&) = delete;
    MenuDetector& operator=(const MenuDetector&) = delete;

    bool initiatedDialogue = false;
public:
    static MenuDetector* GetSingleton()
    {
        static MenuDetector singleton;
        return std::addressof(singleton);
        //code referenced from skyrim-contextual-crosshair
    }

    RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent* a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override;
};
