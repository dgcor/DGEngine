#pragma once

#include "Actions/Action.h"
#include "BaseLevelObject.h"
#include "ItemClass.h"
#include <SFML/System/Time.hpp>

class Player;

class Item : public LevelObject
{
private:
	typedef std::array<LevelObjProperty, 10> ItemProperties;

	const ItemClass* class_;

	BaseLevelObject base;

	sf::Time frameTime{ sf::milliseconds(40) };
	sf::Time currentTime;

	bool wasHoverEnabledOnItemDrop{ false };

	mutable bool updateNameAndDescr{ true };

	mutable std::string name;
	mutable std::array<std::string, 5> descriptions;

	size_t propertiesSize{ 0 };
	ItemProperties properties;

	bool identified{ false };

	void updateNameAndDescriptions() const;

	bool useHelper(uint16_t propHash, uint16_t useOpHash,
		LevelObjValue value, Player& player, const Level* level) const;

	bool useHelper(uint16_t propHash, uint16_t useOpHash, uint16_t valueHash,
		uint16_t valueMaxHash, Player& player, const Level* level) const;

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
	Item(const ItemClass* class__);

	void resetDropAnimation();

	virtual const sf::Vector2f& Position() const { return base.sprite.getPosition(); }
	virtual sf::Vector2f Size() const { return base.getSize(); }

	virtual const MapCoord& MapPosition() const { return base.mapPosition; }
	virtual void MapPosition(const MapCoord& pos) { base.mapPosition = pos; }

	virtual void executeAction(Game& game) const;
	virtual bool getNumberProp(const char* prop, Number32& value) const
	{
		LevelObjValue val;
		bool ret = getInt(prop, val);
		if (ret == true)
		{
			value.setInt32(val);
		}
		return ret;
	}
	virtual bool Passable() const { return true; }
	virtual void setAction(const std::shared_ptr<Action>& action_) {}
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

	void updateDrawPosition(const Level& level) { base.updateDrawPosition(level); }

	virtual bool getProperty(const std::string& prop, Variable& var) const;
	virtual void setProperty(const std::string& prop, const Variable& val);
	virtual const Queryable* getQueryable(const std::string& prop) const { return nullptr; }

	const ItemClass* Class() const { return class_; }

	bool hasIntByHash(uint16_t propHash) const;
	bool hasInt(const char* prop) const;
	bool hasInt(const std::string& prop) const
	{
		return hasInt(prop.c_str());
	}

	LevelObjValue getIntByHash(uint16_t propHash) const;
	LevelObjValue getInt(const char* prop) const;
	LevelObjValue getInt(const std::string& prop) const
	{
		return getInt(prop.c_str());
	}

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getInt(const char* prop, LevelObjValue& value) const;
	bool getInt(const std::string& prop, LevelObjValue& value) const
	{
		return getInt(prop.c_str(), value);
	}
	void setIntByHash(uint16_t propHash, LevelObjValue value);
	void setInt(const char* prop, LevelObjValue value);
	void setInt(const std::string& prop, LevelObjValue value)
	{
		return setInt(prop.c_str(), value);
	}

	void applyDefaults();

	bool needsRecharge() const;
	bool needsRepair() const;
	bool isUsable() const;

	bool use(Player& player, const Level* level) const;

	const std::string& Name() const { return name; }
	const std::string& ShortName() const { return class_->ShortName(); }
	const std::string& SimpleName() const { return class_->Name(); }
	const std::string& ItemType() const { return class_->Type(); }
	const std::string& ItemSubType() const { return class_->SubType(); }
	bool Identified() const { return identified; }
};
