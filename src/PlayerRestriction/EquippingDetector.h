#pragma once
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REX/LOG.h>
#include <array>
#include <iterator>
#include <algorithm>

namespace EquippingDetector
{
    // std::array<std::string, 18> skillList
    // {{
    //     {"onehanded"}, 
    //     {"twohanded"}, 
    //     {"archery"},   
    //     {"block"},     
    //     {"smithing"},  
    //     {"heavyarmor"},
    //     {"lightarmor"},
    //     {"pickpocket"},
    //     {"lockpicking"},
    //     {"sneak"},     
    //     {"alchemy"},   
    //     {"speech"},    
    //     {"alteration"},
    //     {"conjuration"},
    //     {"destruction"},
    //     {"illusion"},  
    //     {"restoration"},
    //     {"enchanting"}
    // }};

    const std::array<int, 10> equipSkills = {0, 1, 2, 5, 6, 16, 15, 14, 13, 12};
    const std::array<RE::BGSBipedObjectForm::BipedObjectSlot, 4> armorSlots = {
        RE::BGSBipedObjectForm::BipedObjectSlot::kBody,
        RE::BGSBipedObjectForm::BipedObjectSlot::kHands,
        RE::BGSBipedObjectForm::BipedObjectSlot::kFeet,
        RE::BGSBipedObjectForm::BipedObjectSlot::kHair
    };

    void ForceUnequipItem(RE::TESBoundObject* item, RE::ActorEquipManager* equipManager);
    void checkEquippedWeapon(int skillIndex, RE::ActorEquipManager* equipManager);
    void checkEquippedArmor(int skillIndex, RE::ActorEquipManager* equipManager);
    void checkEquippedSpell(int skillIndex, RE::ActorEquipManager *equipManager);
    void interruptCasting(RE::PlayerCharacter *player, bool isLeftHand);
    void isViolatingRestriction(int skillIndex, RE::ActorEquipManager* equipManager);
}