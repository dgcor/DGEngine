#pragma once

#include "Actions/Action.h"
#include "BaseLevelObject.h"
#include "ItemCollection.h"
#include "PlayerClass.h"
#include <SFML/Audio/Sound.hpp>

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

	PlayerAction playerAction{ PlayerAction::Stand };

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

	std::array<ItemCollection, (size_t)PlayerInventory::Size> inventories;
	size_t bodyInventoryIdx{ (size_t)PlayerInventory::Size };

	std::string id;
	std::string name;

	uint32_t currentLevel{ 0 };
	uint32_t experience{ 0 };
	uint32_t expNextLevel{ 0 };
	uint32_t points{ 0 };
	uint32_t gold{ 0 };

	LevelObjValue strength{ 0 };
	LevelObjValue strengthItems{ 0 };
	LevelObjValue magic{ 0 };
	LevelObjValue magicItems{ 0 };
	LevelObjValue dexterity{ 0 };
	LevelObjValue dexterityItems{ 0 };
	LevelObjValue vitality{ 0 };
	LevelObjValue vitalityItems{ 0 };

	LevelObjValue life{ 0 };
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

	std::array<std::pair<uint16_t, Number32>, 8> customProperties;
	size_t customPropsSize{ 0 };

	sf::Sound currentSound;

	int16_t attackSound{ -1 };
	int16_t defendSound{ -1 };
	int16_t dieSound{ -1 };
	int16_t hitSound{ -1 };
	int16_t walkSound{ -1 };

	void calculateRange();

	void updateAI(Level& level);

	void updateSpeed();

	void updateWalkPathStep(sf::Vector2f& newDrawPos);
	void updateWalkPath(Game& game, Level& level);

	bool parseInventoryAndItem(const std::string& str,
		std::string& props, size_t& invIdx, size_t& itemIdx) const;

	void updateGoldAdd(const Item* item);
	void updateGoldRemove(const Item* item);

	void updateBodyItemValues();

	void updateAnimation(const Game& game);

	void updateWalk(Game& game, Level& level);
	void updateAttack(Game& game, Level& level);
	void updateDead(Game& game, Level& level);

	void updateExperience(const Level& level);

	bool getCustomIntByHash(uint16_t propHash, Number32& value) const noexcept;
	bool getCustomInt(const char* prop, Number32& value) const noexcept;
	bool getCustomInt(const std::string& prop, Number32& value) const noexcept
	{
		return getCustomInt(prop.c_str(), value);
	}

	void playSound(int16_t soundIdx);

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

	virtual void executeAction(Game& game) const;
	virtual bool getNumberProp(const char* prop, Number32& value) const noexcept;
	virtual bool Passable() const noexcept { return false; }
	virtual void setAction(const std::shared_ptr<Action>& action_) noexcept { action = action_; }
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

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(base.sprite, states);
	}
	virtual void update(Game& game, Level& level);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual void setProperty(const std::string& prop, const Variable& val);
	virtual const Queryable* getQueryable(const std::string& prop) const;

	bool hasIntByHash(uint16_t propHash) const noexcept;
	bool hasInt(const char* prop) const noexcept;
	bool hasInt(const std::string& prop) const noexcept
	{
		return hasInt(prop.c_str());
	}

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const noexcept;
	bool getInt(const char* prop, LevelObjValue& value) const noexcept;
	bool getInt(const std::string& prop, LevelObjValue& value) const noexcept
	{
		return getInt(prop.c_str(), value);
	}
	bool getUIntByHash(uint16_t propHash, uint32_t& value) const noexcept;
	bool getUInt(const char* prop, uint32_t& value) const noexcept;
	bool getUInt(const std::string& prop, uint32_t& value) const noexcept
	{
		return getUInt(prop.c_str(), value);
	}
	bool setIntByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept;
	bool setInt(const char* prop, LevelObjValue value, const Level* level) noexcept;
	bool setInt(const std::string& prop, LevelObjValue value, const Level* level) noexcept
	{
		return setInt(prop.c_str(), value, level);
	}
	bool setUIntByHash(uint16_t propHash, uint32_t value, const Level* level) noexcept;
	bool setUInt(const char* prop, uint32_t value, const Level* level) noexcept;
	bool setUInt(const std::string& prop, uint32_t value, const Level* level) noexcept
	{
		return setUInt(prop.c_str(), value, level);
	}
	bool getNumberByHash(uint16_t propHash, Number32& value) const noexcept;
	bool setNumberByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept;
	bool setNumberByHash(uint16_t propHash, const Number32& value, const Level* level) noexcept;
	bool setNumber(const char* prop, LevelObjValue value, const Level* level) noexcept;
	bool setNumber(const char* prop, const Number32& value, const Level* level) noexcept;
	bool setNumber(const std::string& prop, LevelObjValue value, const Level* level) noexcept
	{
		return setNumber(prop.c_str(), value, level);
	}
	bool setNumber(const std::string& prop, const Number32& value, const Level* level) noexcept
	{
		return setNumber(prop.c_str(), value, level);
	}

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

	void setRestStatus(uint16_t restStatus_) noexcept
	{
		restStatus = std::min(restStatus_, (uint16_t)1);
	}

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
	std::unique_ptr<Item> SelectedItem(std::unique_ptr<Item> item) noexcept
	{
		auto old = std::move(selectedItem);
		selectedItem = std::move(item);
		return old;
	}

	ItemCollection& getInventory(PlayerInventory inv) noexcept { return inventories[(size_t)inv]; }
	const ItemCollection& getInventory(PlayerInventory inv) const noexcept { return inventories[(size_t)inv]; }

	ItemCollection& getInventory(size_t idx) noexcept { return inventories[idx]; }
	const ItemCollection& getInventory(size_t idx) const noexcept { return inventories[idx]; }

	size_t getInventorySize() const noexcept { return inventories.size(); }

	void setBodyInventoryIdx(size_t idx) noexcept
	{
		bodyInventoryIdx = std::min(idx, (size_t)PlayerInventory::Size);
	}

	bool hasEquipedItemType(const std::string& type) const;
	bool hasEquipedItemSubType(const std::string& type) const;

	bool getFreeItemSlot(const Item& item, size_t& invIdx, size_t& itemIdx,
		InventoryPosition invPos = InventoryPosition::TopLeft) const;

	bool hasFreeItemSlot(const Item& item) const;

	bool findItem(uint16_t itemTypeHash16, size_t& invIdx,
		size_t& itemIdx, Item*& item) const;

	unsigned countFreeSlots(const ItemClass& itemClass) const;

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
		InventoryPosition invPos = InventoryPosition::TopLeft)
	{
		return setItemInFreeSlot((size_t)inv, item, invPos);
	}
	bool setItemInFreeSlot(size_t invIdx, std::unique_ptr<Item>& item,
		InventoryPosition invPos = InventoryPosition::TopLeft);

	void updateProperties();

	void applyDefaults(const Level& level) noexcept;

	bool canUseItem(const Item& item) const;

	bool addGold(const Level& level, LevelObjValue amount);

	uint32_t getMaxGoldCapacity(const Level& level) const;

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
	const std::string& Name() const noexcept { return name; }
	const std::string& Class() const noexcept { return class_->Name(); }

	LevelObjValue StrengthNow() const noexcept { return strength + strengthItems; }
	LevelObjValue MagicNow() const noexcept { return magic + magicItems; }
	LevelObjValue DexterityNow() const noexcept { return dexterity + dexterityItems; }
	LevelObjValue VitalityNow() const noexcept { return vitality + vitalityItems; }

	LevelObjValue LifeNow() const noexcept { return life + lifeItems - lifeDamage; }
	LevelObjValue ManaNow() const noexcept { return mana + manaItems - manaDamage; }

	void Id(const std::string& id_) { id = id_; }
	void Name(const std::string& name_) { name = name_; }

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
