#pragma once

#include "Actions/Action.h"
#include "BaseLevelObject.h"
#include <memory>
#include <SFML/System/Time.hpp>

class SimpleLevelObject : public LevelObject
{
private:
	BaseLevelObject base;

	sf::Time frameTime{ sf::milliseconds(50) };
	sf::Time currentTime;

	std::shared_ptr<Action> action;

	std::string id;
	std::string name;

public:
	SimpleLevelObject() {}
	SimpleLevelObject(const sf::Texture& texture) : base(texture) {}
	SimpleLevelObject(const TexturePack& texturePack_,
		const std::pair<size_t, size_t>& frameRange_)
		: base(texturePack_, frameRange_)
	{
		base.updateTexture();
	}

	virtual const sf::Vector2f& Position() const { return base.sprite.getPosition(); }
	virtual sf::Vector2f Size() const { return base.getSize(); }

	virtual const MapCoord& MapPosition() const { return base.mapPosition; }
	virtual void MapPosition(const MapCoord& pos) { base.mapPosition = pos; }

	virtual void executeAction(Game& game) const;
	virtual bool getNumberProp(const char* prop, Number32& value) const { return false; }
	virtual bool Passable() const { return true; }
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
		if (base.sprite.getTexture() != NULL)
		{
			target.draw(base.sprite, states);
		}
	}
	virtual void update(Game& game, Level& level);

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual void setProperty(const std::string& prop, const Variable& val) {}
	virtual const Queryable* getQueryable(const std::string& prop) const { return nullptr; }

	sf::Time getFrameTime() const { return frameTime; }
	void setFrameTime(sf::Time time) { frameTime = time; }

	void setTextureRect(const sf::IntRect& rectangle) { base.sprite.setTextureRect(rectangle); }

	const std::string& Id() const { return id; }
	const std::string& Name() const { return name; }
	void Id(const std::string& id_) { id = id_; }
	void Name(const std::string& name_) { name = name_; }
};
