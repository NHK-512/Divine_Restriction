#include "EquippingDetector.h"

void EquippingDetector::isViolatingRestriction(int skillIndex, RE::ActorEquipManager* equipManager)
{
    if(std::ranges::contains(equipSkills, skillIndex))
    {
        //Checking for weapon equips for skill: One-handed, Two-handed, Archery
        if(skillIndex <= 2)
        {
            checkEquippedWeapon(skillIndex, equipManager);
        }

        if(skillIndex == 5 || skillIndex == 6)
        {
            checkEquippedArmor(skillIndex, equipManager);
        }

        if(skillIndex >= 12)
        {
            checkEquippedSpell(skillIndex, equipManager);
        }
    }
}

void EquippingDetector::checkEquippedWeapon(int skillIndex, RE::ActorEquipManager* equipManager)
{
    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;

    // 1. Get the equipped objects from both hands
    auto rightHandObj = player->GetEquippedObject(false); // false = Right Hand

    // 2. Cast and check the Right Hand
    if (rightHandObj && rightHandObj->IsWeapon()) 
    {
        auto weapon = rightHandObj->As<RE::TESObjectWEAP>();
        RE::WEAPON_TYPE weaponType = weapon->weaponData.animationType.get();

        if (skillIndex == 0) 
        {
            if (weaponType == RE::WEAPON_TYPE::kOneHandSword ||
                weaponType == RE::WEAPON_TYPE::kOneHandDagger ||
                weaponType == RE::WEAPON_TYPE::kOneHandAxe ||
                weaponType == RE::WEAPON_TYPE::kOneHandMace)
            {
                ForceUnequipItem(weapon, equipManager);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from using One-Handed weapons");
                return;
            }
        }

        if(skillIndex == 1)
        {
            if (weaponType == RE::WEAPON_TYPE::kTwoHandSword || 
                weaponType == RE::WEAPON_TYPE::kTwoHandAxe)
            {
                ForceUnequipItem(weapon, equipManager);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from using Two-Handed weapons");
                return;
            }
        }

        if(skillIndex == 2)
        {
            if (weaponType == RE::WEAPON_TYPE::kBow || 
                weaponType == RE::WEAPON_TYPE::kCrossbow)
            {
                ForceUnequipItem(weapon, equipManager);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from using Archery weapons");
                return;
            }
        }
    }

    auto leftHandObj  = player->GetEquippedObject(true);  // true = Left Hand

    if (leftHandObj && leftHandObj->IsWeapon()) 
    {
        auto weapon = leftHandObj->As<RE::TESObjectWEAP>();
        RE::WEAPON_TYPE weaponType = weapon->weaponData.animationType.get();

        if (skillIndex == 0) 
        {
            if (weaponType == RE::WEAPON_TYPE::kOneHandSword ||
                weaponType == RE::WEAPON_TYPE::kOneHandDagger ||
                weaponType == RE::WEAPON_TYPE::kOneHandAxe ||
                weaponType == RE::WEAPON_TYPE::kOneHandMace)
            {
                ForceUnequipItem(weapon, equipManager);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from using One-Handed weapons");
                return;
            }
        }
    }
}

void EquippingDetector::checkEquippedArmor(int skillIndex, RE::ActorEquipManager *equipManager)
{
    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;

    using BodySlot = RE::BGSBipedObjectForm::BipedObjectSlot;

    // Get item equipped on the Chest/Body slot
    for(auto it = armorSlots.begin(); it != armorSlots.end(); ++it)
    {
        auto armorObj = player->GetWornArmor(*it);

        if (armorObj) 
        {
            // Check armor weight type (Light, Heavy, or None/Clothing)
            if (armorObj->IsHeavyArmor() && skillIndex == 5) 
            {
                ForceUnequipItem(armorObj, equipManager);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from wearing Heavy Armor.");
            } else if (armorObj->IsLightArmor() && skillIndex == 6) 
            {
                ForceUnequipItem(armorObj, equipManager);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from wearing Light Armor.");
            }
        }
    }
    
}

void EquippingDetector::checkEquippedSpell(int skillIndex, RE::ActorEquipManager *equipManager)
{
    auto player = RE::PlayerCharacter::GetSingleton();
    if (!player) return;

    auto rightHandObj = player->GetEquippedObject(false);
    if (rightHandObj && rightHandObj->GetFormType() == RE::FormType::Spell) 
    {
        auto spell = rightHandObj->As<RE::SpellItem>();
        
        // 3. Spells hold their school data inside their primary magic effect
        auto costliestEffect = spell->GetCostliestEffectItem();
        if (costliestEffect && costliestEffect->baseEffect) {
            
            // Get the skill type associated with the effect (this dictates the school)
            RE::ActorValue magicSchool = costliestEffect->baseEffect->GetMagickSkill();

            if (magicSchool == RE::ActorValue::kAlteration && skillIndex == 12) {
                
                interruptCasting(player, false);

                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from casting Alteration spells.");
            } else if (magicSchool == RE::ActorValue::kConjuration && skillIndex == 13) {
                
                interruptCasting(player, false);
                
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from casting Conjuration spells.");
            } else if (magicSchool == RE::ActorValue::kDestruction && skillIndex == 14) {
                
                interruptCasting(player, false);
                
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from casting Destruction spells.");
            } else if (magicSchool == RE::ActorValue::kIllusion && skillIndex == 15) {
                
                interruptCasting(player, false);

                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from casting Illusion spells.");
            } else if (magicSchool == RE::ActorValue::kRestoration && skillIndex == 16) {

                interruptCasting(player, false);

                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from casting Restoration spells.");
            }
        }
    }

    auto leftHandObj = player->GetEquippedObject(true);
    if (leftHandObj && leftHandObj->GetFormType() == RE::FormType::Spell) 
    {
        auto spell = leftHandObj->As<RE::SpellItem>();
        auto spellAlt = leftHandObj->As<RE::MagicItem>();
        
        // 3. Spells hold their school data inside their primary magic effect
        auto costliestEffect = spell->GetCostliestEffectItem();
        if (costliestEffect && costliestEffect->baseEffect) {
            
            // Get the skill type associated with the effect (this dictates the school)
            RE::ActorValue magicSchool = costliestEffect->baseEffect->GetMagickSkill();

            if (magicSchool == RE::ActorValue::kAlteration && skillIndex == 12) {
                interruptCasting(player, true);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from casting Alteration spells.");
            } else if (magicSchool == RE::ActorValue::kConjuration && skillIndex == 13) {
                interruptCasting(player, true);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from casting Conjuration spells.");
            } else if (magicSchool == RE::ActorValue::kDestruction && skillIndex == 14) {
                interruptCasting(player, true);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from casting Destruction spells.");
            } else if (magicSchool == RE::ActorValue::kIllusion && skillIndex == 15) {
                interruptCasting(player, true);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from casting Illusion spells.");
            } else if (magicSchool == RE::ActorValue::kRestoration && skillIndex == 16) {
                interruptCasting(player, true);
                RE::SendHUDMessage::ShowHUDMessage("The Divines have restricted you from casting Restoration spells.");
            }
        }
    }
}

void EquippingDetector::interruptCasting(RE::PlayerCharacter *player, bool isLeftHand)
{
    RE::MagicCaster* magicCaster;
    
    if(isLeftHand) 
        magicCaster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
    else
        magicCaster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kRightHand);

    if (magicCaster)
        magicCaster->InterruptCast(true);
}

void EquippingDetector::ForceUnequipItem(RE::TESBoundObject* item, RE::ActorEquipManager* equipManager) 
{
    auto player = RE::PlayerCharacter::GetSingleton();

    if(!equipManager)
        equipManager = RE::ActorEquipManager::GetSingleton();
    
    if (!player || !equipManager || !item) return;

    // Force the engine to unequip the item
    equipManager->UnequipObject(
        player,       // The target actor
        item,       // The item form pointer (weapon, armor, spell, etc.)
        nullptr,      // Optional extra data (usually leave nullptr)
        1,            // Item count to unequip (1 is standard)
        nullptr,    // Specific slot (crucial for weapons/spells, can be nullptr for armor)
        true,         // Queue the event (true is safest for thread-safety and smooth animations)
        false,        // Force unequip even if flagged as unremovable (Quest items/Cursed)
        false,        // Play sound (false suppresses the unequip sound effect)
        false,        // Apply equilibrium modifiers
        nullptr       // Out parameter for status/results (usually leave nullptr)
    );
}
