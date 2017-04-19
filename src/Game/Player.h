#pragma once

#include "Actions/Action.h"
#include <cstdint>
#include "ItemCollection.h"
#include "LevelObject.h"
#include <memory>
#include "PlayerClass.h"
#include <queue>

class Player : public LevelObject
{
private:
	sf::Sprite sprite;
	MapCoord mapPosition;
	MapCoord mapPositionMoveTo;

	std::queue<MapCoord> walkPath;

	std::shared_ptr<PlayerClass> class_;

	PlayerDirection direction{ PlayerDirection::All };
	PlayerStatus status{ PlayerStatus::Stand1 };

	size_t celIdx{ 0 };
	size_t palette{ 0 };

	CelTextureCacheVector* celTexture{ nullptr };
	std::pair<size_t, size_t> frameRange;
	size_t currentFrame{ 0 };

	sf::Time frameTime{ sf::milliseconds(50) };
	sf::Time currentTime;

	std::shared_ptr<Action> action;

	bool enableHover{ true };
	bool hovered{ false };

	std::shared_ptr<Item> selectedItem;

	std::array<ItemCollection, (size_t)PlayerInventory::Size> inventories;

	std::string id;
	std::string name;

	uint32_t currentLevel{ 0 };
	uint32_t experience{ 0 };
	uint32_t expNextLevel{ 0 };
	uint32_t points{ 0 };
	uint32_t gold{ 0 };

	LevelObjValue strengthBase{ 0 };
	LevelObjValue strengthNow{ 0 };
	LevelObjValue magicBase{ 0 };
	LevelObjValue magicNow{ 0 };
	LevelObjValue dexterityBase{ 0 };
	LevelObjValue dexterityNow{ 0 };
	LevelObjValue vitalityBase{ 0 };
	LevelObjValue vitalityNow{ 0 };

	LevelObjValue lifeBase{ 0 };
	LevelObjValue lifeNow{ 0 };
	LevelObjValue manaBase{ 0 };
	LevelObjValue manaNow{ 0 };

	LevelObjValue armorClass{ 0 };
	LevelObjValue toHit{ 0 };
	LevelObjValue damageMin{ 0 };
	LevelObjValue damageMax{ 0 };

	LevelObjValue resistMagic{ 0 };
	LevelObjValue resistFire{ 0 };
	LevelObjValue resistLightning{ 0 };

	void calculateRange()
	{
		celTexture = class_->getCelTexture(palette);
		if (celTexture != nullptr
			&& direction < PlayerDirection::Size)
		{
			celIdx = class_->getStatusCelIndex(status);
			auto numFrames = celTexture->size(celIdx);
			if (direction == PlayerDirection::All)
			{
				frameRange.first = 0;
				frameRange.second = numFrames;
			}
			else
			{
				auto period = (numFrames / 8);
				frameRange.first = (size_t)direction * period;
				frameRange.second = frameRange.first + period;
			}
		}
		else
		{
			celIdx = 0;
			frameRange.first = 0;
			frameRange.second = 0;
		}
	}

	void updateWalkPath(Game& game, Level& level, const sf::Vector2u& texSize);

	bool parseInventoryAndItem(const std::string& str,
		std::string& props, size_t& invIdx, size_t& itemIdx) const;

public:
	Player(const std::shared_ptr<PlayerClass>& class__) : class_(class__)
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

	const MapCoord& MapPositionMoveTo() const { return mapPositionMoveTo; }

	virtual void executeAction(Game& game) const;
	virtual bool Passable() const { return true; }
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

	bool getPlayerPropertyByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getPlayerProperty(const char* prop, LevelObjValue& value) const;
	bool getPlayerProperty(const std::string& prop, LevelObjValue& value) const
	{
		return getPlayerProperty(prop.c_str(), value);
	}
	void setPlayerPropertyByHash(uint16_t propHash, LevelObjValue value);
	void setPlayerProperty(const char* prop, LevelObjValue value);
	void setPlayerProperty(const std::string& prop, LevelObjValue value) const
	{
		return setPlayerProperty(prop.c_str(), value);
	}

	void setWalkPath(const std::queue<MapCoord> walkPath_)
	{
		walkPath = walkPath_;
		if (walkPath.empty() == false)
		{
			mapPositionMoveTo = walkPath.back();
		}
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

	const std::shared_ptr<Item>& SelectedItem() const { return selectedItem; }
	void SelectedItem(const std::shared_ptr<Item>& item) { selectedItem = item; }

	ItemCollection& getInventory(PlayerInventory inv) { return inventories[(size_t)inv]; }
	const ItemCollection& getInventory(PlayerInventory inv) const { return inventories[(size_t)inv]; }

	ItemCollection& getInventory(size_t idx) { return inventories[idx]; }
	const ItemCollection& getInventory(size_t idx) const { return inventories[idx]; }

	size_t getInventorySize() { return inventories.size(); }

	bool getItemSlot(const Item& item, size_t& invIdx, size_t& itemIdx,
		InventoryPosition invPos = InventoryPosition::TopLeft) const;

	bool hasItemSlot(const Item& item) const;

	bool findItem(uint16_t itemTypeHash16, size_t& invIdx,
		size_t& itemIdx, std::shared_ptr<Item>& item) const;

	unsigned countFreeSlots(const ItemClass& itemClass) const;

	void updatePlayerProperties(PlayerInventory inv) { updatePlayerProperties((size_t)inv); }
	void updatePlayerProperties(size_t idx);

	void applyDefaults();

	bool canEquipItem(const Item& item) const;

	bool addGold(const Level& level, LevelObjValue amount);
	void updateGoldAdd(const std::shared_ptr<Item>& item);
	void updateGoldRemove(const std::shared_ptr<Item>& item);

	uint32_t getMaxGoldCapacity(const Level& level) const;

	const std::string& Id() const { return id; }
	const std::string& Name() const { return name; }
	const std::string& Class() const { return class_->Name(); }

	void Id(const std::string& id_) { id = id_; }
	void Name(const std::string& name_) { name = name_; }

	void CurrentLevel(uint32_t level_) { currentLevel = level_; }
	void Experience(uint32_t experience_) { experience = experience_; }
	void ExpNextLevel(uint32_t expNextLevel_) { expNextLevel = expNextLevel_; }
	void Points(uint32_t points_) { points = points_; }

	void StrengthBase(LevelObjValue strengthBase_) { strengthBase = strengthBase_; }
	void StrengthNow(LevelObjValue strengthNow_) { strengthNow = strengthNow_; }
	void MagicBase(LevelObjValue magicBase_) { magicBase = magicBase_; }
	void MagicNow(LevelObjValue magicNow_) { magicNow = magicNow_; }
	void DexterityBase(LevelObjValue dexterityBase_) { dexterityBase = dexterityBase_; }
	void DexterityNow(LevelObjValue dexterityNow_) { dexterityNow = dexterityNow_; }
	void VitalityBase(LevelObjValue vitalityBase_) { vitalityBase = vitalityBase_; }
	void VitalityNow(LevelObjValue vitalityNow_) { vitalityNow = vitalityNow_; }

	void LifeBase(LevelObjValue lifeBase_) { lifeBase = lifeBase_; }
	void LifeNow(LevelObjValue lifeNow_) { lifeNow = lifeNow_; }
	void ManaBase(LevelObjValue manaBase_) { manaBase = manaBase_; }
	void ManaNow(LevelObjValue manaNow_) { manaNow = manaNow_; }

	void ArmorClass(LevelObjValue armorClass_) { armorClass = armorClass_; }
	void ToHit(LevelObjValue toHit_) { toHit = toHit_; }
	void DamageMin(LevelObjValue damage_) { damageMin = damage_; }
	void DamageMax(LevelObjValue damage_) { damageMax = damage_; }

	void ResistMagic(LevelObjValue resistMagic_) { resistMagic = resistMagic_; }
	void ResistFire(LevelObjValue resistFire_) { resistFire = resistFire_; }
	void ResistLightning(LevelObjValue resistLightning_) { resistLightning = resistLightning_; }
};
