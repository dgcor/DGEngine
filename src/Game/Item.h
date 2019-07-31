#pragma once

#include "Actions/Action.h"
#include "ItemClass.h"
#include "LevelObject.h"
#include "Save/SaveItem.h"
#include "Utils/FixedMap.h"

class Player;

class Item : public LevelObject
{
private:
	typedef FixedMap<uint16_t, LevelObjValue, 12> ItemProperties;

	const Queryable* itemOwner{ nullptr };
	const SpellInstance* spell{ nullptr };

	bool wasHoverEnabledOnItemDrop{ false };

	mutable bool updateClassifierVals{ true };

	mutable std::string name;
	mutable std::array<std::string, 5> descriptions;

	ItemProperties properties;

	mutable LevelObjValue pricePrefix1{ 0 };
	mutable LevelObjValue pricePrefix2{ 0 };
	mutable LevelObjValue priceSuffix1{ 0 };
	mutable LevelObjValue priceSuffix2{ 0 };

	bool identified{ false };

	void updateClassifierValues() const;

	bool useValue(uint16_t propHash, uint16_t useOpHash,
		LevelObjValue value, Player& player, const Level& level) const noexcept;

	bool getAndUseValue(uint16_t propHash, uint16_t useOpHash,
		uint16_t valueHash, Player& player, const Level& level) const noexcept;

	bool useItem(Player& player, const Level& level,
		uint16_t useOpHash, uint32_t& quantityLeft);
	bool useSpell(Player& player, const Level& level,
		uint16_t useOpHash, uint32_t& quantityLeft);

	// updates item's price based on whether it's identified or not.
	void updatePrice() const;

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

	constexpr const ItemClass* Class() const noexcept
	{
		return (ItemClass*)class_;
	}

	void clearMapPosition() noexcept { mapPosition = { -1, -1 }; }

	void resetDropAnimation(Level& level) noexcept;
	void resetDropAnimation(LevelMap& map) noexcept;

	virtual bool getTexture(uint32_t textureNumber, TextureInfo& ti) const;

	virtual bool getNumberProp(const std::string_view prop, Number32& value) const;
	virtual bool Passable() const noexcept { return true; }

	virtual void serialize(void* serializeObj, Save::Properties& props,
		const Game& game, const Level& level) const
	{
		Save::serialize(serializeObj, props, game, level, *this);
	}

	virtual void update(Game& game, Level& level, std::weak_ptr<LevelObject> thisPtr);

	virtual bool getProperty(const std::string_view prop, Variable& var) const;
	virtual void setProperty(const std::string_view prop, const Variable& val);

	virtual const std::string_view getType() const { return "item"; }

	bool hasIntByHash(uint16_t propHash) const noexcept;
	bool hasInt(const std::string_view prop) const noexcept;

	LevelObjValue getIntByHash(uint16_t propHash) const;
	LevelObjValue getInt(const std::string_view prop) const;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;
	bool getInt(const std::string_view prop, LevelObjValue& value) const;
	void setIntByHash(uint16_t propHash, LevelObjValue value);
	void setInt(const std::string_view prop, LevelObjValue value);

	bool getNumberPropByHash(const Queryable& owner,
		uint16_t propHash, LevelObjValue& value) const;
	bool getNumberPropByHash(const Queryable& owner, uint16_t propHash,
		const std::string_view minMaxNumber, LevelObjValue& value) const;

	void applyDefaults();

	bool needsRecharge() const;
	bool needsRepair() const;
	bool isUsable() const noexcept;

	bool use(Player& player, const Level& level, uint32_t& quantityLeft);

	// returns the new item quantity. removes from the amount what was added/removed.
	LevelObjValue addQuantity(LevelObjValue& amount);

	bool hasBaseSpell() const noexcept { return Class()->hasSpell(); }
	const SpellInstance* getBaseSpell() const noexcept { return Class()->getSpell(); }

	bool hasSpell() const noexcept { return spell != nullptr; }
	const SpellInstance* getSpell() const noexcept { return spell; }

	void updateOwner(Queryable* obj);

	const std::string& Name() const noexcept { return name; }
	const std::string& ShortName() const noexcept { return Class()->ShortName(); }
	const std::string& SimpleName() const noexcept { return Class()->Name(); }
	const std::string& ItemType() const noexcept { return Class()->Type(); }
	const std::string& ItemSubType() const noexcept { return Class()->SubType(); }
	bool Identified() const noexcept { return identified; }
};
