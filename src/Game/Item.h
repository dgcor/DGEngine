#pragma once

#include "Actions/Action.h"
#include "CelCache.h"
#include <cstdint>
#include "GameProperties.h"
#include "ItemClass.h"
#include "LevelObject.h"
#include <memory>

typedef std::array<LevelObjProperty, 10> ItemProperties;

class Item : public LevelObject
{
private:
	sf::Sprite sprite;
	MapCoord mapPosition;

	std::shared_ptr<ItemClass> class_;

	std::pair<size_t, size_t> frameRange;
	size_t currentFrame{ 0 };

	sf::Time frameTime{ sf::milliseconds(40) };
	sf::Time currentTime;

	bool wasHoverEnabledOnItemDrop{ false };
	bool enableHover{ true };
	bool hovered{ false };

	mutable bool updateNameAndDescr{ true };

	mutable std::string name;
	mutable std::array<std::string, 5> descriptions;

	size_t propertiesSize{ 0 };
	ItemProperties properties;

	bool identified{ false };

	void updateNameAndDescriptions() const;

public:
	using iterator = ItemProperties::iterator;
	using const_iterator = ItemProperties::const_iterator;
	using reverse_iterator = ItemProperties::reverse_iterator;
	using const_reverse_iterator = ItemProperties::const_reverse_iterator;

	iterator begin() { return properties.begin(); }
	iterator end() { return properties.begin() + propertiesSize; }
	const_iterator begin() const { return properties.begin(); }
	const_iterator end() const { return properties.begin() + propertiesSize; }
	const_iterator cbegin() const { return properties.cbegin(); }
	const_iterator cend() const { return properties.cbegin() + propertiesSize; }
	reverse_iterator rbegin() { return properties.rend() - propertiesSize; }
	reverse_iterator rend() { return properties.rend(); }
	const_reverse_iterator rbegin() const { return properties.rend() - propertiesSize; }
	const_reverse_iterator rend() const { return properties.rend(); }
	const_reverse_iterator crbegin() const { return properties.crend() - propertiesSize; }
	const_reverse_iterator crend() const { return properties.crend(); }

	Item() {}
	Item(const std::shared_ptr<ItemClass>& class__) : class_(class__)
	{
		frameRange.first = 0;
		frameRange.second = class_->getCelDropTextureSize() - 1;
		currentFrame = frameRange.second;
		applyDefaults();
	}

	void resetDropAnimation();

	virtual const sf::Vector2f& Position() const { return sprite.getPosition(); }
	virtual sf::Vector2f Size() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}

	virtual const MapCoord& MapPosition() const { return mapPosition; }
	virtual void MapPosition(const MapCoord& pos) { mapPosition = pos; }
	void MapPosition(Level& level, const MapCoord& pos);

	virtual void executeAction(Game& game) const;
	virtual bool Passable() const { return true; }
	virtual void setAction(const std::shared_ptr<Action>& action_) {}

	virtual bool Hoverable() const { return enableHover; }
	virtual void Hoverable(bool hoverable) { enableHover = hoverable; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}
	virtual void update(Game& game, Level& level);

	void updateDrawPosition(const Level& level);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual void setProperty(const std::string& prop, const Variable& val);
	virtual const Queryable* getQueryable(const std::string& prop) const { return nullptr; }

	ItemClass* Class() const { return class_.get(); }

	bool hasItemProperty(const char* prop) const;
	bool hasItemProperty(const std::string& prop) const
	{
		return hasItemProperty(prop.c_str());
	}

	LevelObjValue getItemPropertyByHash(uint16_t propHash) const;
	LevelObjValue getItemProperty(const char* prop) const;
	LevelObjValue getItemProperty(const std::string& prop) const
	{
		return getItemProperty(prop.c_str());
	}

	bool getItemPropertyByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getItemProperty(const char* prop, LevelObjValue& value) const;
	bool getItemProperty(const std::string& prop, LevelObjValue& value) const
	{
		return getItemProperty(prop.c_str(), value);
	}
	void setItemPropertyByHash(uint16_t propHash, LevelObjValue value);
	void setItemProperty(const char* prop, LevelObjValue value);
	void setItemProperty(const std::string& prop, LevelObjValue value) const
	{
		return setItemProperty(prop.c_str(), value);
	}

	void applyDefaults();

	bool needsRecharge() const;
	bool needsRepair() const;

	const std::string& Name() const { return name; }
	const std::string& ShortName() const { return class_->ShortName(); }
	const std::string& SimpleName() const { return class_->Name(); }
	const std::string& ItemType() const { return class_->Type(); }
	bool Identified() const { return identified; }
};
