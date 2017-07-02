#pragma once

#include "Actions/Action.h"
#include <cstdint>
#include "ItemCollection.h"
#include "LevelObject.h"
#include <memory>
#include "PlayerClass.h"
#include <queue>

class CelLevelObject : public LevelObject
{
private:
	sf::Sprite sprite;
	MapCoord mapPosition;

	size_t celIdx{ 0 };

	CelTextureCacheVector* celTexture{ nullptr };
	std::pair<size_t, size_t> frameRange;
	size_t currentFrame{ 0 };

	sf::Time frameTime{ sf::milliseconds(50) };
	sf::Time currentTime;

	std::shared_ptr<Action> action;

	bool enableHover{ true };
	bool hovered{ false };

	std::string id;
	std::string name;

public:
	CelLevelObject() {}

	virtual const sf::Vector2f& Position() const { return sprite.getPosition(); }
	virtual sf::Vector2f Size() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}

	virtual const MapCoord& MapPosition() const { return mapPosition; }
	virtual void MapPosition(const MapCoord& pos) { mapPosition = pos; }

	virtual void executeAction(Game& game) const;
	virtual bool getNumberProp(const char* prop, Number32& value) const { return false; }
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
	virtual void setProperty(const std::string& prop, const Variable& val) {}
	virtual const Queryable* getQueryable(const std::string& prop) const { return nullptr; }

	const std::string& Id() const { return id; }
	const std::string& Name() const { return name; }
	void Id(const std::string& id_) { id = id_; }
	void Name(const std::string& name_) { name = name_; }
};
