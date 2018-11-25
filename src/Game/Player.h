#pragma once

#include "Actions/Action.h"
#include "BaseLevelObject.h"
#include "Inventories.h"
#include "PlayerClass.h"
#include "Save/SavePlayer.h"
#include "Spell.h"
#include <SFML/Audio/Sound.hpp>
#include <unordered_map>
#include "Utils/FixedMap.h"
#include "Utils/LRUCache.h"

class Player : public LevelObject
{
private:
	const PlayerClass* class_{ nullptr };

	BaseLevelObject base;
	MapCoord mapPositionMoveTo;
	sf::Vector2f drawPosA;
	sf::Vector2f drawPosB;
	float currPositionStep = 0.f;

	std::vector<MapCoord> walkPath;

	PlayerStatus playerStatus{ PlayerStatus::Stand };

	PlayerDirection direction{ PlayerDirection::All };
	PlayerAnimation animation{ PlayerAnimation::Size };

	uint16_t restStatus{ 0 };

	size_t textureIdx{ 0 };

	AnimationSpeed speed;
	AnimationSpeed defaultSpeed{ sf::Time::Zero, sf::Time::Zero };

	sf::Time currentWalkTime;

	std::shared_ptr<Action> action;

	bool useAI{ false };

	std::unique_ptr<Item> selectedItem;
	Spell* selectedSpell{ nullptr };

	Inventories<(size_t)PlayerInventory::Size> inventories;
	size_t bodyInventoryIdx{ (size_t)PlayerInventory::Size };

	std::unordered_map<std::string, std::unique_ptr<Spell>> spells;

	std::string id;
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

	int16_t attackSound{ -1 };
	int16_t defendSound{ -1 };
	int16_t dieSound{ -1 };
	int16_t hitSound{ -1 };
	int16_t walkSound{ -1 };

	void calculateRange();

	void updateAI(Level& level);

	void updateNameAndDescriptions() const;

	void updateSpeed();

	void updateWalkPathStep(sf::Vector2f& newDrawPos);
	void updateWalkPath(Game& game, Level& level);

	bool parseInventoryAndItem(const std::string_view str,
		std::string_view& props, size_t& invIdx, size_t& itemIdx) const;

	void updateBodyItemValues();

	void updateAnimation(const Game& game);

	void updateWalk(Game& game, Level& level);
	void updateAttack(Game& game, Level& level);
	void updateDead(Game& game, Level& level);

	bool getCustomIntByHash(uint16_t propHash, Number32& value) const noexcept;
	bool getCustomInt(const std::string_view prop, Number32& value) const noexcept;

	void playSound(int16_t soundIdx);

	friend void Save::serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level, const Player& player);

public:
	Player(const PlayerClass* class__, const Level& level);

	const sf::Vector2f& getBasePosition() const noexcept { return base.basePosition; }

	virtual const sf::Vector2f& Position() const { return base.sprite.getPosition(); }
	virtual sf::Vector2f Size() const { return base.getSize(); }

	virtual const MapCoord& MapPosition() const noexcept { return base.mapPosition; }
	virtual void MapPosition(const MapCoord& pos) noexcept { base.mapPosition = pos; }
	void MapPosition(Level& level, const MapCoord& pos);

	void move(Level& level, const MapCoord& pos);

	const MapCoord& MapPositionMoveTo() const noexcept { return mapPositionMoveTo; }

	virtual bool getTexture(size_t textureNumber, TextureInfo& ti) const;

	virtual void executeAction(Game& game) const;
	virtual bool getNumberProp(const std::string_view prop, Number32& value) const noexcept;
	virtual bool Passable() const noexcept { return false; }
	virtual void setColor(const sf::Color& color) { base.sprite.setColor(color); }
	virtual void setOutline(const sf::Color& outline, const sf::Color& ignore) noexcept
	{
		base.sprite.setOutline(outline, ignore);
	}
	virtual void setOutlineOnHover(bool outlineOnHover_) noexcept { base.outlineOnHover = outlineOnHover_; }
	virtual void setPalette(const std::shared_ptr<Palette>& palette) noexcept { base.sprite.setPalette(palette); }
	virtual bool hasPalette() const noexcept { return base.sprite.hasPalette(); }

	virtual bool Hoverable() const noexcept { return base.enableHover; }
	virtual void Hoverable(bool hoverable) noexcept { base.enableHover = hoverable; }

	virtual void serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level) const
	{
		Save::serialize(serializeObj, props, game, level, *this);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(base.sprite, states);
	}
	virtual void update(Game& game, Level& level);

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
	virtual void setProperty(const std::string_view prop, const Variable& val);
	virtual const Queryable* getQueryable(const std::string_view prop) const;

	virtual std::string_view getId() const { return id; }
	virtual std::string_view getClassId() const { return class_->Id(); }

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
	bool getNumberByHash(uint16_t propHash, Number32& value) const noexcept;
	bool setNumberByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept;
	bool setNumberByHash(uint16_t propHash, const Number32& value, const Level* level) noexcept;
	bool setNumber(const std::string_view prop, LevelObjValue value, const Level* level) noexcept;
	bool setNumber(const std::string_view prop, const Number32& value, const Level* level) noexcept;

	void updateDrawPosition(const Level& level) { base.updateDrawPosition(level, drawPosA); }

	void updateTexture();

	void clearWalkPath() noexcept { walkPath.clear(); }
	void setWalkPath(const std::vector<MapCoord>& walkPath_);

	void setDefaultSpeed(const AnimationSpeed& speed_)
	{
		defaultSpeed = speed_;
		speed = class_->getSpeed(animation);
		base.animation.frameTime = speed.animation;
		updateSpeed();
	}

	PlayerDirection getDirection() const noexcept { return direction; }

	void setDirection(PlayerDirection direction_)
	{
		if (direction != direction_)
		{
			direction = direction_;
			calculateRange();
		}
	}
	void setAnimation(PlayerAnimation animation_)
	{
		if (animation != animation_)
		{
			animation = animation_;
			speed = class_->getSpeed(animation);
			base.animation.frameTime = speed.animation;
			updateSpeed();
			calculateRange();
		}
	}
	void setTextureIdx(size_t idx_)
	{
		if (textureIdx != idx_)
		{
			textureIdx = idx_;
			calculateRange();
		}
	}

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
		return animation >= PlayerAnimation::Walk1 && animation <= PlayerAnimation::Walk2;
	}

	void resetAnimationTime() noexcept { base.animation.currentTime = speed.animation; }

	Item* SelectedItem() const noexcept { return selectedItem.get(); }
	std::unique_ptr<Item> SelectedItem(std::unique_ptr<Item> item) noexcept;

	Spell* SelectedSpell() const noexcept { return selectedSpell; }
	void SelectedSpell(const std::string& id) noexcept;

	Inventory& getInventory(PlayerInventory inv) noexcept { return inventories[(size_t)inv]; }
	const Inventory& getInventory(PlayerInventory inv) const noexcept { return inventories[(size_t)inv]; }

	Inventory& getInventory(size_t idx) noexcept { return inventories[idx]; }
	const Inventory& getInventory(size_t idx) const noexcept { return inventories[idx]; }

	size_t getInventorySize() const noexcept { return inventories.size(); }

	void setBodyInventoryIdx(size_t idx) noexcept
	{
		bodyInventoryIdx = std::min(idx, (size_t)PlayerInventory::Size);
	}

	bool hasSpell(const std::string& key) const
	{
		return spells.find(key) != spells.end();
	}

	void addSpell(const std::string key, std::unique_ptr<Spell> obj)
	{
		spells.insert(std::make_pair(key, std::move(obj)));
	}

	Spell* getSpell(const std::string& key) const
	{
		auto it = spells.find(key);
		if (it != spells.end())
		{
			return it->second.get();
		}
		return nullptr;
	}

	bool hasEquipedItemType(const std::string_view type) const;
	bool hasEquipedItemSubType(const std::string_view type) const;

	bool getFreeItemSlot(const Item& item, size_t& invIdx,
		size_t& itemIdx, InventoryPosition invPos) const;

	bool hasFreeItemSlot(const Item& item) const;

	bool setItem(PlayerInventory inv, size_t itemIdx, std::unique_ptr<Item>& item)
	{
		return setItem((size_t)inv, itemIdx, item);
	}
	bool setItem(PlayerInventory inv, size_t itemIdx, std::unique_ptr<Item>& item,
		std::unique_ptr<Item>& oldItem)
	{
		return setItem((size_t)inv, itemIdx, item, oldItem);
	}
	bool setItem(size_t invIdx, size_t itemIdx, std::unique_ptr<Item>& item);
	bool setItem(size_t invIdx, size_t itemIdx, std::unique_ptr<Item>& item,
		std::unique_ptr<Item>& oldItem);

	bool setItemInFreeSlot(PlayerInventory inv, std::unique_ptr<Item>& item,
		InventoryPosition invPos, bool splitIntoMultipleQuantities)
	{
		return setItemInFreeSlot((size_t)inv, item, invPos, splitIntoMultipleQuantities);
	}

	// sets an item in a free slot. If the item has quantity/capacity
	// it tries to fit into the smallest quantity item of the same class first
	// if splitIntoMultiple is true, it tries to fit into all items of the same class.
	bool setItemInFreeSlot(size_t invIdx, std::unique_ptr<Item>& item,
		InventoryPosition invPos, bool splitIntoMultiple);

	// updates the player's current level, experience required for the
	// next level and points added for having advanced n levels.
	void updateLevelFromExperience(const Level& level, bool updatePoints);

	void updateProperties();

	void applyDefaults(const Level& level) noexcept;

	bool canUseItem(const Item& item) const;

	// returns the remaining quantity to add/remove. 0 if all quantity was added.
	LevelObjValue addItemQuantity(const ItemClass& itemClass,
		const LevelObjValue amount, InventoryPosition invPos);

	void updateItemQuantityCache(uint16_t classIdHash16, LevelObjValue amount) noexcept;
	void updateItemQuantityCache(uint16_t classIdHash16) noexcept;

	uint32_t getMaxItemCapacity(const ItemClass& itemClass) const;

	const PlayerClass* getPlayerClass() const noexcept { return class_; }

	bool isAI() const noexcept { return useAI; }
	void setAI(bool ai_) noexcept { useAI = ai_; }

	bool hasMaxStats() const noexcept;

	void setAttackSounds(int16_t soundIdx) noexcept { attackSound = soundIdx; }
	void setDefendSounds(int16_t soundIdx) noexcept { defendSound = soundIdx; }
	void setDieSounds(int16_t soundIdx) noexcept { dieSound = soundIdx; }
	void setHitSounds(int16_t soundIdx) noexcept { hitSound = soundIdx; }
	void setWalkSounds(int16_t soundIdx) noexcept { walkSound = soundIdx; }

	const std::string& Id() const noexcept { return id; }
	const std::string& Name() const;
	const std::string& SimpleName() const noexcept { return class_->Name(); }
	const std::string& Class() const noexcept { return class_->Id(); }

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
