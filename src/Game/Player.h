#pragma once

#include "Actions/Action.h"
#include <cstdint>
#include "ItemCollection.h"
#include "LevelObject.h"
#include <memory>
#include "PlayerClass.h"
#include <queue>

class Level;

class Player : public LevelObject
{
private:
	sf::Sprite sprite;
	sf::Vector2i mapPosition;

	std::queue<sf::Vector2i> walkPath;

	std::shared_ptr<PlayerClass> class_;

	PlayerDirection direction{ PlayerDirection::All };
	PlayerStatus status { PlayerStatus::Stand1 };

	size_t celIdx{ 0 };
	size_t palette{ 0 };

	CelTextureCacheVector* celTexture{ nullptr };
	std::pair<size_t, size_t> frameRange;
	size_t currentFrame{ 0 };

	sf::Time m_frameTime{ sf::milliseconds(50) };
	sf::Time m_currentTime;

	std::shared_ptr<Action> action;

	bool hovered{ false };

	ItemCollection inventory1;
	ItemCollection inventory2;

	std::string id;
	std::string name;

	int32_t level{ 0 };
	int32_t experience{ 0 };
	int32_t expNextLevel{ 0 };
	int32_t points{ 0 };
	int32_t gold{ 0 };

	int32_t strengthBase{ 0 };
	int32_t strengthNow{ 0 };
	int32_t magicBase{ 0 };
	int32_t magicNow{ 0 };
	int32_t dexterityBase{ 0 };
	int32_t dexterityNow{ 0 };
	int32_t vitalityBase{ 0 };
	int32_t vitalityNow{ 0 };

	int32_t lifeBase{ 0 };
	int32_t lifeNow{ 0 };
	int32_t manaBase{ 0 };
	int32_t manaNow{ 0 };

	int32_t armorClass{ 0 };
	int32_t toHit{ 0 };
	int32_t damageMin{ 0 };
	int32_t damageMax{ 0 };

	int32_t resistMagic{ 0 };
	int32_t resistFire{ 0 };
	int32_t resistLightning{ 0 };

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

public:
	Player() {}
	Player(const std::shared_ptr<PlayerClass>& class__) : class_(class__)
	{
		calculateRange();
	}

	virtual const sf::Vector2f& Position() const { return sprite.getPosition(); }
	virtual void Position(const sf::Vector2f& position) { sprite.setPosition(position); }
	virtual sf::Vector2f Size() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}
	virtual void Size(const sf::Vector2f& size) {}

	virtual const sf::Vector2i& MapPosition() const { return mapPosition; }

	virtual void MapPosition(const sf::Vector2i& pos) { mapPosition = pos; }
	virtual void MapPosition(Level& level, const sf::Vector2i& pos);

	virtual void executeAction(Game& game) const;
	virtual bool Passable() const { return true; }
	virtual void setAction(const std::shared_ptr<Action>& action_) { action = action_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}
	virtual void update(Game& game, Level& level);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual void setProperty(const std::string& prop, const Variable& val);

	void setWalkPath(const std::queue<sf::Vector2i> walkPath_) { walkPath = walkPath_; }

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

	ItemCollection& Inventory1() { return inventory1; }
	const ItemCollection& Inventory1() const { return inventory1; }

	ItemCollection& Inventory2() { return inventory2; }
	const ItemCollection& Inventory2() const { return inventory2; }

	const std::string& Id() const { return id; }
	const std::string& Name() const { return name; }
	const std::string& Class() const { return class_->Name(); }

	int32_t Level_() const { return level; }
	int32_t Experience() const { return experience; }
	int32_t ExpNextLevel() const { return expNextLevel; }
	int32_t Points() const { return points; }
	int32_t Gold() const { return gold; }

	int32_t StrengthBase() const { return strengthBase; }
	int32_t StrengthNow() const { return strengthNow; }
	int32_t MagicBase() const { return magicBase; }
	int32_t MagicNow() const { return magicNow; }
	int32_t DexterityBase() const { return dexterityBase; }
	int32_t DexterityNow() const { return dexterityNow; }
	int32_t VitalityBase() const { return vitalityBase; }
	int32_t VitalityNow() const { return vitalityNow; }

	int32_t LifeBase() const { return lifeBase; }
	int32_t LifeNow() const { return lifeNow; }
	int32_t ManaBase() const { return manaBase; }
	int32_t ManaNow() const { return manaNow; }

	int32_t ArmorClass() const { return armorClass; }
	int32_t ToHit() const { return toHit; }
	int32_t DamageMin() const { return damageMin; }
	int32_t DamageMax() const { return damageMax; }

	int32_t ResistMagic() const { return resistMagic; }
	int32_t ResistFire() const { return resistFire; }
	int32_t ResistLightning() const { return resistLightning; }

	void Id(const std::string& id_) { id = id_; }
	void Name(const std::string& name_) { name = name_; }
	void Class(const std::shared_ptr<PlayerClass>& class__) { class_ = class__; }

	void Level_(int32_t level_) { level = level_; }
	void Experience(int32_t experience_) { experience = experience_; }
	void ExpNextLevel(int32_t expNextLevel_) { expNextLevel = expNextLevel_; }
	void Points(int32_t points_) { points = points_; }
	void Gold(int32_t gold_) { gold = gold_; }

	void StrengthBase(int32_t strengthBase_) { strengthBase = strengthBase_; }
	void StrengthNow(int32_t strengthNow_) { strengthNow = strengthNow_; }
	void MagicBase(int32_t magicBase_) { magicBase = magicBase_; }
	void MagicNow(int32_t magicNow_) { magicNow = magicNow_; }
	void DexterityBase(int32_t dexterityBase_) { dexterityBase = dexterityBase_; }
	void DexterityNow(int32_t dexterityNow_) { dexterityNow = dexterityNow_; }
	void VitalityBase(int32_t vitalityBase_) { vitalityBase = vitalityBase_; }
	void VitalityNow(int32_t vitalityNow_) { vitalityNow = vitalityNow_; }

	void LifeBase(int32_t lifeBase_) { lifeBase = lifeBase_; }
	void LifeNow(int32_t lifeNow_) { lifeNow = lifeNow_; }
	void ManaBase(int32_t manaBase_) { manaBase = manaBase_; }
	void ManaNow(int32_t manaNow_) { manaNow = manaNow_; }

	void ArmorClass(int32_t armorClass_) { armorClass = armorClass_; }
	void ToHit(int32_t toHit_) { toHit = toHit_; }
	void DamageMin(int32_t damage_) { damageMin = damage_; }
	void DamageMax(int32_t damage_) { damageMax = damage_; }

	void ResistMagic(int32_t resistMagic_) { resistMagic = resistMagic_; }
	void ResistFire(int32_t resistFire_) { resistFire = resistFire_; }
	void ResistLightning(int32_t resistLightning_) { resistLightning = resistLightning_; }
};
