#pragma once

#include "Actions/Action.h"
#include "BaseLevelObject.h"
#include "ItemClass.h"
#include "Save/SaveItem.h"
#include "Utils/FixedMap.h"

class Player;

class Item : public LevelObject
{
private:
	typedef FixedMap<uint16_t, LevelObjValue, 10> ItemProperties;

	const ItemClass* class_;

	BaseLevelObject base;

	bool wasHoverEnabledOnItemDrop{ false };

	mutable bool updateNameAndDescr{ true };

	mutable std::string name;
	mutable std::array<std::string, 5> descriptions;

	ItemProperties properties;

	bool identified{ false };

	void updateNameAndDescriptions() const;

	bool useHelper(uint16_t propHash, uint16_t useOpHash,
		LevelObjValue value, Player& player, const Level* level) const noexcept;

	bool useHelper(uint16_t propHash, uint16_t useOpHash, uint16_t valueHash,
		uint16_t valueMaxHash, Player& player, const Level* level) const;

	friend void Save::serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level, const Item& item);

public:
	using iterator = ItemProperties::iterator;
	using const_iterator = ItemProperties::const_iterator;
	using reverse_iterator = ItemProperties::reverse_iterator;
	using const_reverse_iterator = ItemProperties::const_reverse_iterator;

	iterator begin() noexcept { return properties.begin(); }
	iterator end() noexcept { return properties.end(); }
	const_iterator begin() const noexcept { return properties.begin(); }
	const_iterator end() const noexcept { return properties.end(); }
	const_iterator cbegin() const noexcept { return properties.cbegin(); }
	const_iterator cend() const noexcept { return properties.cend(); }
	reverse_iterator rbegin() noexcept { return properties.rbegin(); }
	reverse_iterator rend() noexcept { return properties.rend(); }
	const_reverse_iterator rbegin() const noexcept { return properties.rbegin(); }
	const_reverse_iterator rend() const noexcept { return properties.rend(); }
	const_reverse_iterator crbegin() const noexcept { return properties.crbegin(); }
	const_reverse_iterator crend() const noexcept { return properties.crend(); }

	Item(const ItemClass* class__);

	void resetDropAnimation(Level& level) noexcept;

	virtual const sf::Vector2f& Position() const { return base.sprite.getPosition(); }
	virtual sf::Vector2f Size() const { return base.getSize(); }

	virtual const MapCoord& MapPosition() const noexcept { return base.mapPosition; }
	virtual void MapPosition(const MapCoord& pos) noexcept { base.mapPosition = pos; }

	virtual bool getTexture(size_t textureNumber, TextureInfo& ti) const;

	virtual void executeAction(Game& game) const;
	virtual bool getNumberProp(const std::string_view prop, Number32& value) const
	{
		LevelObjValue val;
		bool ret = getInt(prop, val);
		if (ret == true)
		{
			value.setInt32(val);
		}
		return ret;
	}
	virtual bool Passable() const noexcept { return true; }
	virtual void setColor(const sf::Color& color) { base.sprite.setColor(color); }
	virtual void setOutline(const sf::Color& outline, const sf::Color& ignore) noexcept
	{
		base.sprite.setOutline(outline, ignore);
	}
	virtual void setOutlineOnHover(bool outlineOnHover_) noexcept { base.outlineOnHover = outlineOnHover_; }
	virtual void setPalette(const std::shared_ptr<Palette>& palette) noexcept { base.sprite.setPalette(palette); }
	virtual bool hasPalette() const noexcept { return base.sprite.hasPalette(); }

	virtual bool Hoverable() const noexcept { return base.enableHover; }
	virtual void Hoverable(bool hoverable) noexcept { base.enableHover = hoverable; }

	virtual void serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level) const
	{
		Save::serialize(serializeObj, props, game, level, *this);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(base.sprite, states);
	}
	virtual void update(Game& game, Level& level);

	void updateDrawPosition(const Level& level) { base.updateDrawPosition(level); }

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
	virtual void setProperty(const std::string_view prop, const Variable& val);

	virtual std::string_view getId() const { return {}; }
	virtual std::string_view getClassId() const { return class_->Id(); }

	const ItemClass* Class() const noexcept { return class_; }

	bool hasIntByHash(uint16_t propHash) const noexcept;
	bool hasInt(const std::string_view prop) const noexcept;

	LevelObjValue getIntByHash(uint16_t propHash) const;
	LevelObjValue getInt(const std::string_view prop) const;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getInt(const std::string_view prop, LevelObjValue& value) const;
	void setIntByHash(uint16_t propHash, LevelObjValue value);
	void setInt(const std::string_view prop, LevelObjValue value);

	void applyDefaults();

	bool needsRecharge() const;
	bool needsRepair() const;
	bool isUsable() const noexcept;

	bool use(Player& player, const Level* level, uint32_t& itemsLeft);

	// returns the new item quantity. removes from the amount what was added/removed.
	LevelObjValue addQuantity(LevelObjValue& amount);

	const std::string& Name() const noexcept { return name; }
	const std::string& ShortName() const noexcept { return class_->ShortName(); }
	const std::string& SimpleName() const noexcept { return class_->Name(); }
	const std::string& ItemType() const noexcept { return class_->Type(); }
	const std::string& ItemSubType() const noexcept { return class_->SubType(); }
	bool Identified() const noexcept { return identified; }
};
