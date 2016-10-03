#pragma once

#include "Actions/Action.h"
#include <cstdint>
#include "ItemCollection.h"
#include "LevelObject.h"
#include <memory>
#include "PlayerClass.h"
#include <queue>

class Level;

class CelLevelObject : public LevelObject
{
private:
	sf::Sprite sprite;
	sf::Vector2i mapPosition;

	size_t celIdx{ 0 };
	size_t palette{ 0 };

	CelTextureCacheVector* celTexture{ nullptr };
	std::pair<size_t, size_t> frameRange;
	size_t currentFrame{ 0 };

	sf::Time m_frameTime{ sf::milliseconds(50) };
	sf::Time m_currentTime;

	std::shared_ptr<Action> action;

	bool hovered{ false };

	std::string id;
	std::string name;

public:
	CelLevelObject() {}

	virtual const sf::Vector2f& Position() const { return sprite.getPosition(); }
	virtual void Position(const sf::Vector2f& position) { sprite.setPosition(position); }
	virtual sf::Vector2f Size() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}
	virtual void Size(const sf::Vector2f& size) {}

	virtual const sf::Vector2i& MapPosition() const { return mapPosition; }
	virtual void MapPosition(const sf::Vector2i& pos) { mapPosition = pos; }

	virtual void executeAction(Game& game) const;
	virtual bool Passable() const { return true; }
	virtual void setAction(const std::shared_ptr<Action>& action_) { action = action_; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}
	virtual void update(Game& game, Level& level);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual void setProperty(const std::string& prop, const Variable& val) {}

	const std::string& Id() const { return id; }
	const std::string& Name() const { return name; }
	void Id(const std::string& id_) { id = id_; }
	void Name(const std::string& name_) { name = name_; }
};
