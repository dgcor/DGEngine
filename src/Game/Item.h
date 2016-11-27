#pragma once

#include "Actions/Action.h"
#include "CelCache.h"
#include <cstdint>
#include "ItemClass.h"
#include "LevelObject.h"
#include <memory>
#include "Variable.h"

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

	sf::Vector2u inventorySize;

	bool enableHover{ true };
	bool hovered{ false };

	std::string name;
	std::string description1;
	std::string description2;
	std::string description3;

public:
	Item() {}
	Item(const std::shared_ptr<ItemClass>& class__) : class_(class__)
	{
		frameRange.first = 0;
		frameRange.second = class_->getCelDropTextureSize() - 1;
		currentFrame = frameRange.second;
	}

	void resetDropAnimation() { currentFrame = 0; }

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
	virtual void setProperty(const std::string& prop, const Variable& val) {};

	const sf::Vector2u& InventorySize() const { return inventorySize; }
	void InventorySize(const sf::Vector2u& size_) { inventorySize = size_; }

	ItemClass* Class() const { return class_.get(); }

	const std::string& Name() const { return name; }
	const std::string& ClassName() const { return class_->Name(); }
	const std::string& Description1() const { return description1; }
	const std::string& Description2() const { return description2; }
	const std::string& Description3() const { return description3; }

	void Name(const std::string& name_) { name = name_; }
	void Description1(const std::string& description) { description1 = description; }
	void Description2(const std::string& description) { description2 = description; }
	void Description3(const std::string& description) { description3 = description; }
};
