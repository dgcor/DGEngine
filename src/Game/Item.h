#pragma once

#include "Actions/Action.h"
#include "CelCache.h"
#include <cstdint>
#include <memory>
#include "Variable.h"

class Level;

class Item : public sf::Drawable
{
private:
	sf::Sprite sprite;
	sf::Vector2u position;
	std::shared_ptr<CelTextureCache> celTexture;
	std::pair<size_t, size_t> frameRange;
	sf::Vector2u inventorySize;

	std::shared_ptr<Action> clickAction;

	std::string id;
	std::string name;
	std::string class_;
	std::string description1;
	std::string description2;
	std::string description3;

public:
	Item() {}
	Item(const std::shared_ptr<CelTextureCache>& celTexture_, const std::pair<size_t, size_t>& frameRange_)
		: celTexture(celTexture_), frameRange(frameRange_) {}

	const sf::Vector2u& PlayerPosition() const { return position; }
	void PlayerPosition(const sf::Vector2u& position_) { position = position_; }

	const sf::Vector2f& Position() const { return sprite.getPosition(); }
	void Position(const sf::Vector2f& position) { sprite.setPosition(position); }
	sf::Vector2f Size() const
	{
		return sf::Vector2f((float)sprite.getTextureRect().width, (float)sprite.getTextureRect().height);
	}
	void Size(const sf::Vector2f& size) {}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}
	void update(Game& game, const Level& level);

	Variable getProperty(const std::string& prop) const;

	const sf::Vector2u& InventorySize() const { return inventorySize; }
	void InventorySize(const sf::Vector2u& size_) { inventorySize = size_; }

	void setCelTexture(const std::shared_ptr<CelTextureCache>& celTexture_) { celTexture = celTexture_; }
	void setClickAction(const std::shared_ptr<Action>& action) { clickAction = action; }

	const std::string& Id() const { return id; }
	const std::string& Name() const { return name; }
	const std::string& Class() const { return class_; }
	const std::string& Description1() const { return description1; }
	const std::string& Description2() const { return description2; }
	const std::string& Description3() const { return description3; }

	void Id(const std::string& id_) { id = id_; }
	void Name(const std::string& name_) { name = name_; }
	void Class(const std::string& class__) { class_ = class__; }
	void Description1(const std::string& description) { description1 = description; }
	void Description2(const std::string& description) { description2 = description; }
	void Description3(const std::string& description) { description3 = description; }
};
