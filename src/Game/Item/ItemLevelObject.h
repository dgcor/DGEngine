#pragma once

#include "Game/Properties/LevelObjValue.h"
#include "Game/Variable.h"
#include <memory>
#include "Utils/Number.h"
#include "Utils/UnorderedStringMap.h"

class Game;
class Item;
class Level;
class LevelObject;
class Queryable;
struct TextureInfo;

class ItemLevelObject
{
private:
	// updates item's price based on whether it's identified or not.
	static void updatePrice(const Item& item);

	static void updateClassifierValues(const Item& item);

public:
	static bool getTexture(const Item& item, uint32_t textureNumber, TextureInfo& ti);

	static bool getIntByHash(const Item& item, uint16_t propHash, LevelObjValue& value);

	static LevelObjValue getIntByHash(const Item& item, uint16_t propHash);

	static bool setIntByHash(Item& item, uint16_t propHash, LevelObjValue value);

	static bool getNumber(const Item& item, const std::string_view prop, Number32& value);

	static bool getNumberByHash(const Item& item, const Queryable& owner, uint16_t propHash, LevelObjValue& value);

	static bool getNumberByHash(const Item& item, const Queryable& owner, uint16_t propHash,
		const std::string_view minMaxNumber, LevelObjValue& value);

	static bool getProperty(const Item& item, const std::string_view prop, Variable& var);

	static void update(Item& item, Game& game, Level& level, const std::shared_ptr<LevelObject>& thisPtr);
};
