#pragma once

#include "Game/GameHashes.h"
#include "Game/LevelObject/LevelObject.h"
#include "Game/Properties/AnimationSpeed.h"
#include "Game/VarOrQueryObject.h"
#include "PlayerClass.h"
#include "PlayerInventories.h"
#include "PlayerProperties.h"
#include "PlayerSpells.h"
#include <SFML/Audio/Sound.hpp>
#include "Utils/FixedMap.h"

class PlayerBase : public LevelObject
{
protected:
	friend class PlayerLevelObject;
	friend class PlayerMove;
	friend class PlayerSave;

	PairFloat mapPositionMoveTo;
	PairFloat mapPosA;
	PairFloat mapPosB;
	float positionStep = 0.f;
	float currPositionStep = 0.f;
	bool executeActionOnDestination{ false };

	std::vector<PairFloat> walkPath;

	PlayerStatus playerStatus{ PlayerStatus::Stand };

	PlayerDirection playerDirection{ PlayerDirection::All };
	PlayerAnimation playerAnimation{ PlayerAnimation::Size };

	uint16_t restStatus{ 0 };

	size_t textureIdx{ 0 };

	AnimationSpeed speed;
	AnimationSpeed defaultSpeed{ sf::Time::Zero, sf::Time::Zero };

	sf::Time currentWalkTime;

	int aiType{ 0 };

	PlayerInventories inventories;
	PlayerSpells spells;
	PlayerProperties properties;

	sf::Sound currentSound;

	void applyDefaults(const Level& level) noexcept;

	void updateAnimation();

	void updateSpeed();

	void playSound(const std::string_view key, size_t soundNum = 0);

public:
	PlayerBase(const PlayerClass* class__, const Level& level);

	void serialize(const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props) const;

	constexpr auto Class() const noexcept { return (const PlayerClass*)class_; }

	bool Passable() const noexcept override { return false; }

	const std::string_view getType() const override { return "player"; }

	using LevelObject::MapPosition;
	bool MapPosition(LevelMap& map, const PairFloat& pos) override;
	using LevelObject::move;
	bool move(LevelMap& map, const PairFloat& pos) override;

	auto& MapPositionMoveTo() const noexcept { return mapPositionMoveTo; }

	bool getNumber(const std::string_view prop, Number32& value) const override;

	bool getProperty(const std::string_view prop, Variable& var) const override;

	bool getNumberByHash(const Queryable& owner, uint16_t propHash, LevelObjValue& value) const override;

	bool setProperty(const std::string_view prop, const Variable& var) override;

	bool setNumberByHash(uint16_t propHash, const Number32& value) noexcept override;

	QueryObject getQueryable(const std::string_view prop) const override;

	std::vector<VarOrQueryObject> getQueryableList(const std::string_view prop) const;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const noexcept;
	bool setIntByHash(uint16_t propHash, LevelObjValue value) noexcept;

	bool getUIntByHash(uint16_t propHash, uint32_t& value) const noexcept;
	bool getUInt(const std::string_view prop, uint32_t& value) const noexcept;

	bool setUIntByHash(uint16_t propHash, uint32_t value, const Level* level) noexcept;
	bool setUInt(const std::string_view prop, uint32_t value, const Level* level) noexcept;

	bool getNumberByHash(uint16_t propHash, const std::string_view props, Number32& value) const noexcept;
	bool setNumberByHash(uint16_t propHash, const Number32& value, const Level* level) noexcept;
	bool setNumber(const std::string_view prop, const Number32& value, const Level* level) noexcept;

	void clearWalkPath() noexcept { walkPath.clear(); }
	void setWalkPath(const std::vector<PairFloat>& walkPath_, bool doAction);
	void Walk(const LevelMap& map, const PairFloat& walkToMapPos, bool doAction);
	void Walk(const LevelMap& map, const PlayerDirection direction, bool doAction);

	void setDefaultSpeed(const AnimationSpeed& speed_);

	auto getDirection() const noexcept { return playerDirection; }

	void setDirection(PlayerDirection direction_);

	void setAnimation(PlayerAnimation animation_);

	void setTextureIdx(size_t idx_);

	void setRestStatus(uint16_t restStatus_) noexcept;

	void setStatus(PlayerStatus status_) noexcept;

	void setStandAnimation()
	{
		setAnimation((PlayerAnimation)((size_t)PlayerAnimation::Stand1 + restStatus));
	}
	void setWalkAnimation()
	{
		setAnimation((PlayerAnimation)((size_t)PlayerAnimation::Walk1 + restStatus));
	}
	bool hasWalkingAnimation() noexcept
	{
		return playerAnimation >= PlayerAnimation::Walk1 &&
			playerAnimation <= PlayerAnimation::Walk2;
	}

	void resetAnimationTime() noexcept { animation.elapsedTime.currentTime = speed.animation; }

	bool blockWalk() const noexcept { return currPositionStep > 0.f && currPositionStep < 0.9f; }

	auto& SelectedItem() const noexcept { return inventories.SelectedItem(); }
	std::shared_ptr<Item> SelectedItem(std::shared_ptr<Item> item) noexcept;

	auto& getInventory(PlayerInventory inv) noexcept { return inventories[(size_t)inv]; }
	auto& getInventory(PlayerInventory inv) const noexcept { return inventories[(size_t)inv]; }

	auto& getInventory(size_t idx) noexcept { return inventories[idx]; }
	auto& getInventory(size_t idx) const noexcept { return inventories[idx]; }

	constexpr auto getInventorySize() const noexcept { return inventories.size(); }

	constexpr auto hasBodyInventory() const noexcept { return inventories.hasBodyInventory(); }
	auto getBodyInventoryIdx() const noexcept { return inventories.getBodyInventoryIdx(); }
	void setBodyInventoryIdx(size_t idx) noexcept { inventories.setBodyInventoryIdx(idx); }

	bool hasEquipedItemType(const std::string_view type) const;
	bool hasEquipedItemSubType(const std::string_view type) const;

	bool getFreeItemSlot(const Item& item, size_t& invIdx, size_t& itemIdx, InventoryPosition invPos) const;

	bool hasFreeItemSlot(const Item& item) const;

	bool setItem(PlayerInventory inv, size_t itemIdx, std::shared_ptr<Item>& item);
	bool setItem(PlayerInventory inv, size_t itemIdx, std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem);
	bool setItem(size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item);
	bool setItem(size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem);

	bool setItemInFreeSlot(PlayerInventory inv, std::shared_ptr<Item>& item, InventoryPosition invPos, bool splitIntoMultipleQuantities);

	// sets an item in a free slot. If the item has quantity/capacity
	// it tries to fit into the smallest quantity item of the same class first
	// if splitIntoMultiple is true, it tries to fit into all items of the same class.
	bool setItemInFreeSlot(size_t invIdx, std::shared_ptr<Item>& item, InventoryPosition invPos, bool splitIntoMultiple);

	// returns the remaining quantity to add/remove. 0 if all quantity was added.
	LevelObjValue addItemQuantity(const ItemClass& itemClass, const LevelObjValue amount, InventoryPosition invPos);

	uint32_t getMaxItemCapacity(const ItemClass& itemClass) const;

	auto SelectedSpell() const noexcept { return spells.SelectedSpell(); }
	void SelectedSpell(const std::string_view id) noexcept;

	bool hasSpell(const std::string_view key) const;
	void addSpell(const std::string_view key, Spell* spell, LevelObjValue spellLevel = 1);
	Spell* getSpell(const std::string_view key) const;
	const SpellInstance* getSpellInstance(const std::string_view key) const;

	// updates the player's current level, experience required for the
	// next level and points added for having advanced n levels.
	void updateLevelFromExperience(const Level& level, bool updatePoints);

	void updateProperties() { properties.updateProperties(*this); }

	bool canUseObject(const LevelObjectQueryable& obj) const;

	auto AIType() const noexcept { return aiType; }
	void AIType(int ai_) noexcept { aiType = ai_; }

	bool hasMaxStats() const noexcept { return properties.hasMaxStats(*Class()); }

	auto& Id() const noexcept { return id; }
	const std::string& Name() const { return properties.Name(*this); }
	auto& SimpleName() const noexcept { return Class()->Name(); }

	void Id(const std::string_view id_) { id = id_; }
	void Name(const std::string_view name_) { properties.name = name_; }

	void CurrentLevel(uint32_t level_) noexcept { properties.currentLevel = level_; }
	void Experience(uint32_t experience_) noexcept { properties.experience = experience_; }
	void ExpNextLevel(uint32_t expNextLevel_) noexcept { properties.expNextLevel = expNextLevel_; }
	void Points(uint32_t points_) noexcept { properties.points = points_; }

	void Strength(LevelObjValue strength_) noexcept { properties.strength = strength_; }
	void Magic(LevelObjValue magic_) noexcept { properties.magic = magic_; }
	void Dexterity(LevelObjValue dexterity_) noexcept { properties.dexterity = dexterity_; }
	void Vitality(LevelObjValue vitality_) noexcept { properties.vitality = vitality_; }

	void Life(LevelObjValue life_) noexcept { properties.life = life_; }
	void LifeDamage(LevelObjValue lifeDamage_) noexcept { properties.lifeDamage = lifeDamage_; }
	void Mana(LevelObjValue manaBase_) noexcept { properties.mana = manaBase_; }
	void ManaDamage(LevelObjValue manaDamage_) noexcept { properties.manaDamage = manaDamage_; }

	void Armor(LevelObjValue armor_) noexcept { properties.armor = armor_; }
	void ToHit(LevelObjValue toHit_) noexcept { properties.toHit = toHit_; }

	void ResistMagic(LevelObjValue resistMagic_) noexcept { properties.resistMagic = resistMagic_; }
	void ResistFire(LevelObjValue resistFire_) noexcept { properties.resistFire = resistFire_; }
	void ResistLightning(LevelObjValue resistLightning_) noexcept { properties.resistLightning = resistLightning_; }
};
