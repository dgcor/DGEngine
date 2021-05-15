#pragma once

#include "Actions/Action.h"
#include "GameHashes.h"
#include "Inventories.h"
#include "LevelObject.h"
#include "PlayerClass.h"
#include "Spell.h"
#include <SFML/Audio/Sound.hpp>
#include "Utils/FixedMap.h"
#include "Utils/LRUCache.h"
#include "Utils/UnorderedStringMap.h"

class PlayerBase : public LevelObject
{
protected:
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
	std::shared_ptr<Item> selectedItem;
	const SpellInstance* selectedSpell{ nullptr };

	Inventories<(size_t)PlayerInventory::Size> inventories;
	size_t bodyInventoryIdx{ (size_t)PlayerInventory::Size };

	UnorderedStringMap<SpellInstance> spells;

	mutable std::string name;
	mutable std::array<std::string, 2> descriptions;

	mutable bool updateNameAndDescr{ true };

	uint32_t currentLevel{ 0 };
	uint32_t experience{ 0 };
	uint32_t expNextLevel{ 0 };
	uint32_t points{ 0 };

	LevelObjValue strength{ 0 };
	LevelObjValue strengthItems{ 0 };
	LevelObjValue magic{ 0 };
	LevelObjValue magicItems{ 0 };
	LevelObjValue dexterity{ 0 };
	LevelObjValue dexterityItems{ 0 };
	LevelObjValue vitality{ 0 };
	LevelObjValue vitalityItems{ 0 };

	LevelObjValue life{ 1 };
	LevelObjValue lifeItems{ 0 };
	LevelObjValue lifeDamage{ 0 };
	LevelObjValue mana{ 0 };
	LevelObjValue manaItems{ 0 };
	LevelObjValue manaDamage{ 0 };

	LevelObjValue armor{ 0 };
	LevelObjValue armorItems{ 0 };
	LevelObjValue toArmor{ 0 };
	LevelObjValue toHit{ 0 };
	LevelObjValue toHitItems{ 0 };
	LevelObjValue damageMin{ 0 };
	LevelObjValue damageMax{ 0 };
	LevelObjValue damageMinItems{ 0 };
	LevelObjValue damageMaxItems{ 0 };
	LevelObjValue toDamage{ 0 };

	LevelObjValue resistMagic{ 0 };
	LevelObjValue resistMagicItems{ 0 };
	LevelObjValue resistFire{ 0 };
	LevelObjValue resistFireItems{ 0 };
	LevelObjValue resistLightning{ 0 };
	LevelObjValue resistLightningItems{ 0 };

	FixedMap<uint16_t, Number32, 8> customProperties;

	mutable LRUCache<uint16_t, uint32_t, 3> itemQuantityCache;

	sf::Sound currentSound;

	void updateAnimation();

	void updateNameAndDescriptions() const;

	void updateSpeed();

	void updateWalkPathStep(PairFloat& newMapPos);
	void updateWalkPath(Game& game, LevelMap& map);

	void updateBodyItemValues();

	bool getCustomIntByHash(uint16_t propHash, Number32& value) const noexcept;
	bool getCustomInt(const std::string_view prop, Number32& value) const noexcept;

	void playSound(const std::string_view key, size_t soundNum = 0);

public:
	PlayerBase(const PlayerClass* class__, const Level& level);

	constexpr const PlayerClass* Class() const noexcept
	{
		return (PlayerClass*)class_;
	}

	using LevelObject::MapPosition;
	bool MapPosition(LevelMap& map, const PairFloat& pos) override;
	using LevelObject::move;
	bool move(LevelMap& map, const PairFloat& pos) override;

	const PairFloat& MapPositionMoveTo() const noexcept { return mapPositionMoveTo; }

	bool getTexture(uint32_t textureNumber, TextureInfo& ti) const override;

	bool getNumberProp(const std::string_view prop, Number32& value) const override;
	bool Passable() const noexcept override { return false; }

	bool getProperty(const std::string_view prop, Variable& var) const override;
	void setProperty(const std::string_view prop, const Variable& val) override;
	const Queryable* getQueryable(const std::string_view prop) const override;

	std::vector<std::variant<const Queryable*, Variable>> getQueryableList(
		const std::string_view prop) const;

	const std::string_view getType() const override { return "player"; }

	bool hasIntByHash(uint16_t propHash) const noexcept;
	bool hasInt(const std::string_view prop) const noexcept;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const noexcept;
	bool getInt(const std::string_view prop, LevelObjValue& value) const noexcept;
	bool getUIntByHash(uint16_t propHash, uint32_t& value) const noexcept;
	bool getUInt(const std::string_view prop, uint32_t& value) const noexcept;
	bool setIntByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept;
	bool setInt(const std::string_view prop, LevelObjValue value, const Level* level) noexcept;
	bool setUIntByHash(uint16_t propHash, uint32_t value, const Level* level) noexcept;
	bool setUInt(const std::string_view prop, uint32_t value, const Level* level) noexcept;

	bool getNumberByHash(uint16_t propHash, const std::string_view props, Number32& value) const noexcept;
	bool setNumberByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept;
	bool setNumberByHash(uint16_t propHash, const Number32& value, const Level* level) noexcept;
	bool setNumber(const std::string_view prop, LevelObjValue value, const Level* level) noexcept;
	bool setNumber(const std::string_view prop, const Number32& value, const Level* level) noexcept;

	void clearWalkPath() noexcept { walkPath.clear(); }
	void setWalkPath(const std::vector<PairFloat>& walkPath_, bool doAction);
	void Walk(const LevelMap& map, const PairFloat& walkToMapPos, bool doAction);
	void Walk(const LevelMap& map, const PlayerDirection direction, bool doAction);

	void setDefaultSpeed(const AnimationSpeed& speed_);

	PlayerDirection getDirection() const noexcept { return playerDirection; }

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

	Item* SelectedItem() const noexcept { return selectedItem.get(); }
	std::shared_ptr<Item> SelectedItem(std::shared_ptr<Item> item) noexcept;

	void SelectedSpell(const std::string_view id) noexcept;

	Inventory& getInventory(PlayerInventory inv) noexcept { return inventories[(size_t)inv]; }
	const Inventory& getInventory(PlayerInventory inv) const noexcept { return inventories[(size_t)inv]; }

	Inventory& getInventory(size_t idx) noexcept { return inventories[idx]; }
	const Inventory& getInventory(size_t idx) const noexcept { return inventories[idx]; }

	size_t getInventorySize() const noexcept { return inventories.size(); }

	void setBodyInventoryIdx(size_t idx) noexcept
	{
		bodyInventoryIdx = std::min(idx, (size_t)PlayerInventory::Size);
	}

	bool hasSpell(const std::string_view key) const;
	void addSpell(const std::string_view key, Spell* obj, LevelObjValue spellLevel = 1);
	Spell* getSpell(const std::string_view key) const;
	const SpellInstance* getSpellInstance(const std::string_view key) const;

	bool hasEquipedItemType(const std::string_view type) const;
	bool hasEquipedItemSubType(const std::string_view type) const;

	bool getFreeItemSlot(const Item& item, size_t& invIdx,
		size_t& itemIdx, InventoryPosition invPos) const;

	bool hasFreeItemSlot(const Item& item) const;

	bool setItem(PlayerInventory inv, size_t itemIdx, std::shared_ptr<Item>& item)
	{
		return setItem((size_t)inv, itemIdx, item);
	}
	bool setItem(PlayerInventory inv, size_t itemIdx, std::shared_ptr<Item>& item,
		std::shared_ptr<Item>& oldItem)
	{
		return setItem((size_t)inv, itemIdx, item, oldItem);
	}
	bool setItem(size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item);
	bool setItem(size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item,
		std::shared_ptr<Item>& oldItem);

	bool setItemInFreeSlot(PlayerInventory inv, std::shared_ptr<Item>& item,
		InventoryPosition invPos, bool splitIntoMultipleQuantities)
	{
		return setItemInFreeSlot((size_t)inv, item, invPos, splitIntoMultipleQuantities);
	}

	// sets an item in a free slot. If the item has quantity/capacity
	// it tries to fit into the smallest quantity item of the same class first
	// if splitIntoMultiple is true, it tries to fit into all items of the same class.
	bool setItemInFreeSlot(size_t invIdx, std::shared_ptr<Item>& item,
		InventoryPosition invPos, bool splitIntoMultiple);

	// updates the player's current level, experience required for the
	// next level and points added for having advanced n levels.
	void updateLevelFromExperience(const Level& level, bool updatePoints);

	void updateProperties();

	void applyDefaults(const Level& level) noexcept;

	template <class T>
	bool canUseObject(const T& obj) const
	{
		LevelObjValue value;
		if (obj.getNumberPropByHash(*this, ItemProp::RequiredStrength, value) == true &&
			value > StrengthNow())
		{
			return false;
		}
		if (obj.getNumberPropByHash(*this, ItemProp::RequiredMagic, value) == true &&
			value > MagicNow())
		{
			return false;
		}
		if (obj.getNumberPropByHash(*this, ItemProp::RequiredDexterity, value) == true &&
			value > DexterityNow())
		{
			return false;
		}
		if (obj.getNumberPropByHash(*this, ItemProp::RequiredVitality, value) == true &&
			value > VitalityNow())
		{
			return false;
		}
		if (obj.getNumberPropByHash(*this, ItemProp::RequiredLife, value) == true &&
			value > LifeNow())
		{
			return false;
		}
		if (obj.getNumberPropByHash(*this, ItemProp::RequiredMana, value) == true &&
			value > ManaNow())
		{
			return false;
		}
		return true;
	}

	// returns the remaining quantity to add/remove. 0 if all quantity was added.
	LevelObjValue addItemQuantity(const ItemClass& itemClass,
		const LevelObjValue amount, InventoryPosition invPos);

	void updateItemQuantityCache(uint16_t classIdHash16, LevelObjValue amount) noexcept;
	void updateItemQuantityCache(uint16_t classIdHash16) noexcept;

	uint32_t getMaxItemCapacity(const ItemClass& itemClass) const;

	int AIType() const noexcept { return aiType; }
	void AIType(int ai_) noexcept { aiType = ai_; }

	bool hasMaxStats() const noexcept;

	const std::string& Id() const noexcept { return id; }
	const std::string& Name() const;
	const std::string& SimpleName() const noexcept { return Class()->Name(); }

	LevelObjValue StrengthNow() const noexcept { return strength + strengthItems; }
	LevelObjValue MagicNow() const noexcept { return magic + magicItems; }
	LevelObjValue DexterityNow() const noexcept { return dexterity + dexterityItems; }
	LevelObjValue VitalityNow() const noexcept { return vitality + vitalityItems; }

	LevelObjValue LifeNow() const noexcept { return life + lifeItems - lifeDamage; }
	LevelObjValue ManaNow() const noexcept { return mana + manaItems - manaDamage; }

	void Id(const std::string_view id_) { id = id_; }
	void Name(const std::string_view name_) { name = name_; }

	void CurrentLevel(uint32_t level_) noexcept { currentLevel = level_; }
	void Experience(uint32_t experience_) noexcept { experience = experience_; }
	void ExpNextLevel(uint32_t expNextLevel_) noexcept { expNextLevel = expNextLevel_; }
	void Points(uint32_t points_) noexcept { points = points_; }

	void Strength(LevelObjValue strength_) noexcept { strength = strength_; }
	void Magic(LevelObjValue magic_) noexcept { magic = magic_; }
	void Dexterity(LevelObjValue dexterity_) noexcept { dexterity = dexterity_; }
	void Vitality(LevelObjValue vitality_) noexcept { vitality = vitality_; }

	void Life(LevelObjValue life_) noexcept { life = life_; }
	void LifeDamage(LevelObjValue lifeDamage_) noexcept { lifeDamage = lifeDamage_; }
	void Mana(LevelObjValue manaBase_) noexcept { mana = manaBase_; }
	void ManaDamage(LevelObjValue manaDamage_) noexcept { manaDamage = manaDamage_; }

	void Armor(LevelObjValue armor_) noexcept { armor = armor_; }
	void ToHit(LevelObjValue toHit_) noexcept { toHit = toHit_; }

	void ResistMagic(LevelObjValue resistMagic_) noexcept { resistMagic = resistMagic_; }
	void ResistFire(LevelObjValue resistFire_) noexcept { resistFire = resistFire_; }
	void ResistLightning(LevelObjValue resistLightning_) noexcept { resistLightning = resistLightning_; }
};
