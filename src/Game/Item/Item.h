#pragma once

#include "Game/LevelObject/LevelObject.h"
#include "ItemClass.h"
#include "Utils/FixedMap.h"
#include "Utils/UnorderedStringMap.h"

class Player;

class Item : public LevelObject
{
private:
	friend class ItemLevelObject;
	friend class ItemSave;

	const Queryable* itemOwner{ nullptr };
	const SpellInstance* spell{ nullptr };

	bool wasHoverEnabledOnItemDrop{ false };

	mutable bool updateClassifierVals{ true };

	mutable std::string name;
	mutable std::array<std::string, 5> descriptions;

	FixedMap<uint16_t, LevelObjValue, 12> properties;

	mutable LevelObjValue pricePrefix1{ 0 };
	mutable LevelObjValue pricePrefix2{ 0 };
	mutable LevelObjValue priceSuffix1{ 0 };
	mutable LevelObjValue priceSuffix2{ 0 };

	bool identified{ false };

	void applyDefaults();

	bool useValue(uint16_t propHash, uint16_t useOpHash, LevelObjValue value, Player& player, const Level& level) const noexcept;

	bool getAndUseValue(uint16_t propHash, uint16_t useOpHash, uint16_t valueHash, Player& player, const Level& level) const noexcept;

	bool useItem(Player& player, const Level& level, uint16_t useOpHash, uint32_t& quantityLeft);
	bool useSpell(Player& player, const Level& level, uint16_t useOpHash, uint32_t& quantityLeft);

public:
	auto begin() noexcept { return properties.begin(); }
	auto end() noexcept { return properties.end(); }
	auto begin() const noexcept { return properties.begin(); }
	auto end() const noexcept { return properties.end(); }
	auto cbegin() const noexcept { return properties.cbegin(); }
	auto cend() const noexcept { return properties.cend(); }
	auto rbegin() noexcept { return properties.rbegin(); }
	auto rend() noexcept { return properties.rend(); }
	auto rbegin() const noexcept { return properties.rbegin(); }
	auto rend() const noexcept { return properties.rend(); }
	auto crbegin() const noexcept { return properties.crbegin(); }
	auto crend() const noexcept { return properties.crend(); }

	Item(const ItemClass* class__);

	void serialize(const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props) const;

	constexpr auto Class() const noexcept { return (const ItemClass*)class_; }

	bool Passable() const noexcept override { return true; }

	const std::string_view getType() const override { return "item"; }

	void clearMapPosition() noexcept { mapPosition = { -1, -1 }; }

	void resetDropAnimation(Level& level) noexcept;
	void resetDropAnimation(LevelMap& map) noexcept;

	bool getTexture(uint32_t textureNumber, TextureInfo& ti) const override;

	bool getNumber(const std::string_view prop, Number32& value) const override;

	void update(Game& game, Level& level, const std::shared_ptr<LevelObject>& thisPtr) override;

	bool getProperty(const std::string_view prop, Variable& var) const override;

	// doesn't check Identified, indestructible or unlimitedCharges
	bool hasPropertyByHash(uint16_t propHash) const noexcept;

	bool getIntByHash(uint16_t propHash, LevelObjValue& value) const;
	LevelObjValue getIntByHash(uint16_t propHash) const;

	bool setNumberByHash(uint16_t propHash, const Number32& value) override;
	bool setIntByHash(uint16_t propHash, LevelObjValue value);

	bool getNumberByHash(const Queryable& owner, uint16_t propHash, LevelObjValue& value) const override;
	bool getNumberByHash(const Queryable& owner, uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const;

	bool needsRecharge() const;
	bool needsRepair() const;
	bool isUsable() const noexcept;

	bool use(Player& player, const Level& level, uint32_t& quantityLeft);

	// returns the new item quantity. removes from the amount what was added/removed.
	LevelObjValue addQuantity(LevelObjValue& amount);

	bool hasBaseSpell() const noexcept { return Class()->hasSpell(); }
	auto getBaseSpell() const noexcept { return Class()->getSpell(); }

	bool hasSpell() const noexcept { return spell != nullptr; }
	auto getSpell() const noexcept { return spell; }

	void updateOwner(Queryable* obj);

	auto& Name() const noexcept { return name; }
	auto& ShortName() const noexcept { return Class()->ShortName(); }
	auto& SimpleName() const noexcept { return Class()->Name(); }
	auto& ItemType() const noexcept { return Class()->Type(); }
	auto& ItemSubType() const noexcept { return Class()->SubType(); }
	bool Identified() const noexcept { return identified; }
};
