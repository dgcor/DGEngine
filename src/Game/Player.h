#pragma once

#include "Actions/Action.h"
#include <cstdint>
#include "ItemCollection.h"
#include "LevelObject.h"
#include <memory>
#include "PlayerClass.h"

class Player : public LevelObject
{
private:
	const PlayerClass* class_{ nullptr };

	sf::Sprite sprite;
	MapCoord mapPosition;
	MapCoord mapPositionMoveTo;
	sf::Vector2f drawPosA;
	sf::Vector2f drawPosB;
	float currPositionStep = 0.f;

	std::vector<MapCoord> walkPath;

	PlayerDirection direction{ PlayerDirection::All };
	PlayerStatus status{ PlayerStatus::Size };

	uint8_t restStatus{ 0 };

	size_t celIdx{ 0 };
	size_t palette{ 0 };

	CelTextureCacheVector* celTexture{ nullptr };
	std::pair<size_t, size_t> frameRange;
	size_t currentFrame{ 0 };

	AnimationSpeed speed;
	AnimationSpeed defaultSpeed{ sf::Time::Zero, sf::Time::Zero };

	sf::Time currentAnimationTime;
	sf::Time currentWalkTime;

	std::shared_ptr<Action> action;

	bool enableHover{ true };
	bool hovered{ false };

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
	LevelObjValue resistFire{ 0 };
	LevelObjValue resistLightning{ 0 };

	std::array<std::pair<uint16_t, Number32>, 4> customProperties;
	size_t customPropsSize{ 0 };

	void calculateRange();

	void updateMapPosition(Level& level, const MapCoord& pos);

	void updateSpeed();

	void updateWalkPathStep(sf::Vector2f& newDrawPos);
	void updateWalkPath(Game& game, Level& level);

	bool parseInventoryAndItem(const std::string& str,
		std::string& props, size_t& invIdx, size_t& itemIdx) const;

	void updateGoldAdd(const std::shared_ptr<Item>& item);
	void updateGoldRemove(const std::shared_ptr<Item>& item);

	void updateBodyItemValues();

public:
	Player(const PlayerClass* class__) : class_(class__)
	{
		calculateRange();
	}

	sf::Vector2f getBasePosition() const;

	virtual const sf::Vector2f& Position() const { return sprite.getPosition(); }
	virtual sf::Vector2f Size() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}

	virtual const MapCoord& MapPosition() const { return mapPosition; }
	virtual void MapPosition(const MapCoord& pos) { mapPosition = pos; }
	void MapPosition(Level& level, const MapCoord& pos);

	void move(Level& level, const MapCoord& pos);

	const MapCoord& MapPositionMoveTo() const { return mapPositionMoveTo; }

	virtual void executeAction(Game& game) const;
	virtual bool getNumberProp(const char* prop, Number32& value) const;
	virtual bool Passable() const { return false; }
	virtual void setAction(const std::shared_ptr<Action>& action_) { action = action_; }

	virtual bool Hoverable() const { return enableHover; }
	virtual void Hoverable(bool hoverable) { enableHover = hoverable; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}
	virtual void update(Game& game, Level& level);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual void setProperty(const std::string& prop, const Variable& val);
	virtual const Queryable* getQueryable(const std::string& prop) const;

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
	bool setIntByHash(uint16_t propHash, LevelObjValue value);
	bool setInt(const char* prop, LevelObjValue value);
	bool setInt(const std::string& prop, LevelObjValue value)
	{
		return setInt(prop.c_str(), value);
	}
	bool setUIntByHash(uint16_t propHash, uint32_t value);
	bool setUInt(const char* prop, uint32_t value);
	bool setUInt(const std::string& prop, uint32_t value)
	{
		return setUInt(prop.c_str(), value);
	}
	bool getNumberByHash(uint16_t propHash, Number32& value) const;
	bool setNumberByHash(uint16_t propHash, LevelObjValue value);
	bool setNumberByHash(uint16_t propHash, const Number32& value);
	bool setNumber(const char* prop, LevelObjValue value);
	bool setNumber(const char* prop, const Number32& value);
	bool setNumber(const std::string& prop, LevelObjValue value)
	{
		return setNumber(prop.c_str(), value);
	}
	bool setNumber(const std::string& prop, const Number32& value)
	{
		return setNumber(prop.c_str(), value);
	}

	void updateDrawPosition(sf::Vector2f pos);
	void updateDrawPosition() { updateDrawPosition(drawPosA); }

	void updateTexture();

	void clearWalkPath() { walkPath.clear(); }
	void setWalkPath(const std::vector<MapCoord>& walkPath_);

	void setDefaultSpeed(const AnimationSpeed& speed_)
	{
		defaultSpeed = speed_;
		speed = class_->getSpeed(status);
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
	void setStatus(PlayerStatus status_)
	{
		if (status != status_)
		{
			status = status_;
			speed = class_->getSpeed(status);
			updateSpeed();
			calculateRange();
		}
	}
	void setPalette(size_t palette_)
	{
		if (palette_ >= PlayerPaletteSize)
		{
			palette_ = 0;
		}
		if (palette != palette_)
		{
			palette = palette_;
			calculateRange();
		}
	}

	void setRestStatus(uint16_t restStatus_) { restStatus = std::min(restStatus_, (uint16_t)1); }

	void setStandStatus()
	{
		setStatus((PlayerStatus)((size_t)PlayerStatus::Stand1 + restStatus));
	}
	void setWalkStatus()
	{
		setStatus((PlayerStatus)((size_t)PlayerStatus::Walk1 + restStatus));
	}
	bool hasWalkingStatus()
	{
		return status >= PlayerStatus::Walk1 && status <= PlayerStatus::Walk2;
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

	void updatePlayerProperties();

	void applyDefaults();

	bool canEquipItem(const Item& item) const;

	bool addGold(const Level& level, LevelObjValue amount);

	uint32_t getMaxGoldCapacity(const Level& level) const;

	const PlayerClass* getPlayerClass() const { return class_; }

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
