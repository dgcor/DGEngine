#pragma once

#include "Actions/Action.h"
#include "BaseLevelObject.h"
#include "ItemCollection.h"
#include "PlayerClass.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/System/Time.hpp>

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

	sf::Time currentAnimationTime;
	sf::Time currentWalkTime;

	std::shared_ptr<Action> action;

	bool useAI{ false };

	std::shared_ptr<Item> selectedItem;

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

	void updateGoldAdd(const std::shared_ptr<Item>& item);
	void updateGoldRemove(const std::shared_ptr<Item>& item);

	void updateBodyItemValues();

	void updateAnimation(Game& game);

	void updateWalk(Game& game, Level& level);
	void updateAttack(Game& game, Level& level);
	void updateDead(Game& game, Level& level);

	void updateExperience(const Level& level);

	bool getCustomIntByHash(uint16_t propHash, Number32& value) const;
	bool getCustomInt(const char* prop, Number32& value) const;
	bool getCustomInt(const std::string& prop, Number32& value) const
	{
		return getCustomInt(prop.c_str(), value);
	}

	void playSound(int16_t soundIdx);

public:
	Player(const PlayerClass* class__, const Level& level);

	sf::Vector2f getBasePosition(const Level& level) const;

	virtual const sf::Vector2f& Position() const { return base.sprite.getPosition(); }
	virtual sf::Vector2f Size() const { return base.getSize(); }

	virtual const MapCoord& MapPosition() const { return base.mapPosition; }
	virtual void MapPosition(const MapCoord& pos) { base.mapPosition = pos; }
	void MapPosition(Level& level, const MapCoord& pos);

	void move(Level& level, const MapCoord& pos);

	const MapCoord& MapPositionMoveTo() const { return mapPositionMoveTo; }

	virtual void executeAction(Game& game) const;
	virtual bool getNumberProp(const char* prop, Number32& value) const;
	virtual bool Passable() const { return false; }
	virtual void setAction(const std::shared_ptr<Action>& action_) { action = action_; }
	virtual void setColor(const sf::Color& color) { base.sprite.setColor(color); }
	virtual void setOutline(const sf::Color& outline, const sf::Color& ignore)
	{
		base.sprite.setOutline(outline, ignore);
	}
	virtual void setOutlineOnHover(bool outlineOnHover_) { base.outlineOnHover = outlineOnHover_; }
	virtual void setPalette(const std::shared_ptr<Palette>& palette) { base.sprite.setPalette(palette); }
	virtual bool hasPalette() const { return base.sprite.hasPalette(); }

	virtual bool Hoverable() const { return base.enableHover; }
	virtual void Hoverable(bool hoverable) { base.enableHover = hoverable; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(base.sprite, states);
	}
	virtual void update(Game& game, Level& level);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual void setProperty(const std::string& prop, const Variable& val);
	virtual const Queryable* getQueryable(const std::string& prop) const;

	bool hasIntByHash(uint16_t propHash) const;
	bool hasInt(const char* prop) const;
	bool hasInt(const std::string& prop) const
	{
		return hasInt(prop.c_str());
	}

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getInt(const char* prop, LevelObjValue& value) const;
	bool getInt(const std::string& prop, LevelObjValue& value) const
	{
		return getInt(prop.c_str(), value);
	}
	bool getUIntByHash(uint16_t propHash, uint32_t& value) const;
	bool getUInt(const char* prop, uint32_t& value) const;
	bool getUInt(const std::string& prop, uint32_t& value) const
	{
		return getUInt(prop.c_str(), value);
	}
	bool setIntByHash(uint16_t propHash, LevelObjValue value, const Level* level);
	bool setInt(const char* prop, LevelObjValue value, const Level* level);
	bool setInt(const std::string& prop, LevelObjValue value, const Level* level)
	{
		return setInt(prop.c_str(), value, level);
	}
	bool setUIntByHash(uint16_t propHash, uint32_t value, const Level* level);
	bool setUInt(const char* prop, uint32_t value, const Level* level);
	bool setUInt(const std::string& prop, uint32_t value, const Level* level)
	{
		return setUInt(prop.c_str(), value, level);
	}
	bool getNumberByHash(uint16_t propHash, Number32& value) const;
	bool setNumberByHash(uint16_t propHash, LevelObjValue value, const Level* level);
	bool setNumberByHash(uint16_t propHash, const Number32& value, const Level* level);
	bool setNumber(const char* prop, LevelObjValue value, const Level* level);
	bool setNumber(const char* prop, const Number32& value, const Level* level);
	bool setNumber(const std::string& prop, LevelObjValue value, const Level* level)
	{
		return setNumber(prop.c_str(), value, level);
	}
	bool setNumber(const std::string& prop, const Number32& value, const Level* level)
	{
		return setNumber(prop.c_str(), value, level);
	}

	void updateDrawPosition(const Level& level) { base.updateDrawPosition(level, drawPosA); }

	void updateTexture();

	void clearWalkPath() { walkPath.clear(); }
	void setWalkPath(const std::vector<MapCoord>& walkPath_);

	void setDefaultSpeed(const AnimationSpeed& speed_)
	{
		defaultSpeed = speed_;
		speed = class_->getSpeed(animation);
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

	void setRestStatus(uint16_t restStatus_) { restStatus = std::min(restStatus_, (uint16_t)1); }

	void setStandAnimation()
	{
		setAnimation((PlayerAnimation)((size_t)PlayerAnimation::Stand1 + restStatus));
	}
	void setWalkAnimation()
	{
		setAnimation((PlayerAnimation)((size_t)PlayerAnimation::Walk1 + restStatus));
	}
	bool hasWalkingAnimation()
	{
		return animation >= PlayerAnimation::Walk1 && animation <= PlayerAnimation::Walk2;
	}

	void resetAnimationTime() { currentAnimationTime = speed.animation; }

	const std::shared_ptr<Item>& SelectedItem() const { return selectedItem; }
	void SelectedItem(const std::shared_ptr<Item>& item) { selectedItem = item; }

	ItemCollection& getInventory(PlayerInventory inv) { return inventories[(size_t)inv]; }
	const ItemCollection& getInventory(PlayerInventory inv) const { return inventories[(size_t)inv]; }

	ItemCollection& getInventory(size_t idx) { return inventories[idx]; }
	const ItemCollection& getInventory(size_t idx) const { return inventories[idx]; }

	size_t getInventorySize() const { return inventories.size(); }

	void setBodyInventoryIdx(size_t idx)
	{
		bodyInventoryIdx = std::min(idx, (size_t)PlayerInventory::Size);
	}

	bool hasEquipedItemType(const std::string& type) const;
	bool hasEquipedItemSubType(const std::string& type) const;

	bool getItemSlot(const Item& item, size_t& invIdx, size_t& itemIdx,
		InventoryPosition invPos = InventoryPosition::TopLeft) const;

	bool hasItemSlot(const Item& item) const;

	bool findItem(uint16_t itemTypeHash16, size_t& invIdx,
		size_t& itemIdx, std::shared_ptr<Item>& item) const;

	unsigned countFreeSlots(const ItemClass& itemClass) const;

	bool setItem(PlayerInventory inv, size_t itemIdx, const std::shared_ptr<Item>& item)
	{
		return setItem((size_t)inv, itemIdx, item);
	}
	bool setItem(PlayerInventory inv, size_t itemIdx, const std::shared_ptr<Item>& item,
		std::shared_ptr<Item>& oldItem)
	{
		return setItem((size_t)inv, itemIdx, item, oldItem);
	}
	bool setItem(size_t invIdx, size_t itemIdx, const std::shared_ptr<Item>& item);
	bool setItem(size_t invIdx, size_t itemIdx, const std::shared_ptr<Item>& item,
		std::shared_ptr<Item>& oldItem);

	bool setItemInFreeSlot(PlayerInventory inv, const std::shared_ptr<Item>& item,
		InventoryPosition invPos = InventoryPosition::TopLeft)
	{
		return setItemInFreeSlot((size_t)inv, item, invPos);
	}
	bool setItemInFreeSlot(size_t invIdx, const std::shared_ptr<Item>& item,
		InventoryPosition invPos = InventoryPosition::TopLeft);

	void updateProperties();

	void applyDefaults(const Level& level);

	bool canUseItem(const Item& item) const;

	bool addGold(const Level& level, LevelObjValue amount);

	uint32_t getMaxGoldCapacity(const Level& level) const;

	const PlayerClass* getPlayerClass() const { return class_; }

	bool isAI() const { return useAI; }
	void setAI(bool ai_) { useAI = ai_; }

	bool hasMaxStats() const;

	void setAttackSounds(int16_t soundIdx) { attackSound = soundIdx; }
	void setDefendSounds(int16_t soundIdx) { defendSound = soundIdx; }
	void setDieSounds(int16_t soundIdx) { dieSound = soundIdx; }
	void setHitSounds(int16_t soundIdx) { hitSound = soundIdx; }
	void setWalkSounds(int16_t soundIdx) { walkSound = soundIdx; }

	const std::string& Id() const { return id; }
	const std::string& Name() const { return name; }
	const std::string& Class() const { return class_->Name(); }

	LevelObjValue StrengthNow() const { return strength + strengthItems; }
	LevelObjValue MagicNow() const { return magic + magicItems; }
	LevelObjValue DexterityNow() const { return dexterity + dexterityItems; }
	LevelObjValue VitalityNow() const { return vitality + vitalityItems; }

	LevelObjValue LifeNow() const { return life + lifeItems - lifeDamage; }
	LevelObjValue ManaNow() const { return mana + manaItems - manaDamage; }

	void Id(const std::string& id_) { id = id_; }
	void Name(const std::string& name_) { name = name_; }

	void CurrentLevel(uint32_t level_) { currentLevel = level_; }
	void Experience(uint32_t experience_) { experience = experience_; }
	void ExpNextLevel(uint32_t expNextLevel_) { expNextLevel = expNextLevel_; }
	void Points(uint32_t points_) { points = points_; }

	void Strength(LevelObjValue strength_) { strength = strength_; }
	void Magic(LevelObjValue magic_) { magic = magic_; }
	void Dexterity(LevelObjValue dexterity_) { dexterity = dexterity_; }
	void Vitality(LevelObjValue vitality_) { vitality = vitality_; }

	void Life(LevelObjValue life_) { life = life_; }
	void LifeDamage(LevelObjValue lifeDamage_) { lifeDamage = lifeDamage_; }
	void Mana(LevelObjValue manaBase_) { mana = manaBase_; }
	void ManaDamage(LevelObjValue manaDamage_) { manaDamage = manaDamage_; }

	void Armor(LevelObjValue armor_) { armor = armor_; }
	void ToHit(LevelObjValue toHit_) { toHit = toHit_; }

	void ResistMagic(LevelObjValue resistMagic_) { resistMagic = resistMagic_; }
	void ResistFire(LevelObjValue resistFire_) { resistFire = resistFire_; }
	void ResistLightning(LevelObjValue resistLightning_) { resistLightning = resistLightning_; }
};
