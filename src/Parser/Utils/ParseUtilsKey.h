#pragma once

#include "Anchor.h"
#include "AnimationType.h"
#include "Game/GameProperties.h"
#include "Json/JsonParser.h"
#include "Parser/ParserProperties.h"
#include "ParseUtilsVal.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Variable.h"
#include <vector>

namespace Parser
{
	AnimationType getAnimationTypeKey(const rapidjson::Value& elem,
		const char* key, AnimationType val = AnimationType::Looped);

	Anchor getAnchorKey(const rapidjson::Value& elem,
		const char* key, Anchor val = Anchor::Top | Anchor::Left);

	bool getBoolKey(const rapidjson::Value& elem, const char* key, bool val = {});

	double getDoubleKey(const rapidjson::Value& elem, const char* key, double val = {});

	float getFloatKey(const rapidjson::Value& elem, const char* key, float val = {});

	int getIntKey(const rapidjson::Value& elem, const char* key, int val = {});

	int64_t getInt64Key(const rapidjson::Value& elem, const char* key, int64_t val = {});

	const char* getStringCharKey(const rapidjson::Value& elem,
		const char* key, const char* val = "");

	std::string getStringKey(const rapidjson::Value& elem,
		const char* key, const std::string& val = {});

	unsigned getUIntKey(const rapidjson::Value& elem,
		const char* key, unsigned val = {});

	uint64_t getUInt64Key(const rapidjson::Value& elem,
		const char* key, uint64_t val = {});

	template <class T>
	T getVector2fKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsArray() == true
			&& elem[key].Size() > 1
			&& elem[key][0].IsNumber() == true
			&& elem[key][1].IsNumber() == true)
		{
			return T(elem[key][0].GetFloat(), elem[key][1].GetFloat());
		}
		return val;
	}

	template <class T>
	T getVector2iKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsArray() == true
			&& elem[key].Size() > 1
			&& elem[key][0].IsNumber() == true
			&& elem[key][1].IsNumber() == true)
		{
			return T(elem[key][0].GetInt(), elem[key][1].GetInt());
		}
		return val;
	}

	template <class T>
	T getVector2uKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsArray() == true
			&& elem[key].Size() > 1
			&& elem[key][0].IsNumber() == true
			&& elem[key][1].IsNumber() == true)
		{
			return T(elem[key][0].GetUint(), elem[key][1].GetUint());
		}
		return val;
	}

	std::pair<size_t, size_t> getFramesKey(const rapidjson::Value& elem,
		const char* key, const std::pair<size_t, size_t>& val = {});

	std::pair<size_t, size_t> getIndexKey(const rapidjson::Value& elem,
		const char* key, const std::pair<size_t, size_t>& val = {});

	sf::Vector2f getPositionKey(const rapidjson::Value& elem,
		const char* key, const sf::Vector2f& size, const sf::Vector2u& refSize);

	sf::IntRect getIntRectKey(const rapidjson::Value& elem,
		const char* key, const sf::IntRect& val = {});

	sf::FloatRect getFloatRectKey(const rapidjson::Value& elem,
		const char* key, const sf::FloatRect& val = {});

	sf::Color getColorKey(const rapidjson::Value& elem, const char* key,
		const sf::Color& val = {});

	sf::Time getTimeKey(const rapidjson::Value& elem, const char* key,
		const sf::Time& val = {});

	std::vector<std::string> getStringVectorKey(const rapidjson::Value& elem, const char* key);

	IgnoreResource getIgnoreResourceKey(const rapidjson::Value& elem,
		const char* key, IgnoreResource val = IgnoreResource::None);

	size_t getInventoryItemIndexKey(const rapidjson::Value& elem,
		const char* key, PlayerInventory inv);

	InventoryPosition getInventoryPositionKey(const rapidjson::Value& elem,
		const char* key, InventoryPosition val = InventoryPosition::TopLeft);

	template <class T>
	T getMinMaxIntKey(const rapidjson::Value& elem, const char* key, T val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getMinMaxIntVal<T>(elem[key], val);
		}
		return val;
	}

	ItemCoordInventory getItemCoordInventoryKey(const rapidjson::Value& elem, const char* key);

	ItemLocation getItemLocationKey(const rapidjson::Value& elem, const char* key);

	ItemXY getItemXYKey(const rapidjson::Value& elem, const char* key, const ItemXY& val = {});

	PlayerDirection getPlayerDirectionKey(const rapidjson::Value& elem,
		const char* key, PlayerDirection val = PlayerDirection::All);

	PlayerInventory getPlayerInventoryKey(const rapidjson::Value& elem,
		const char* key, PlayerInventory val = PlayerInventory::Body);

	PlayerItemMount getPlayerItemMountKey(const rapidjson::Value& elem,
		const char* key, PlayerItemMount val = PlayerItemMount::LeftHand);

	PlayerAnimation getPlayerAnimationKey(const rapidjson::Value& elem,
		const char* key, PlayerAnimation val = PlayerAnimation::Stand1);

	ReplaceVars getReplaceVarsKey(const rapidjson::Value& elem,
		const char* key, ReplaceVars val = ReplaceVars::None);

	Variable getVariableKey(const rapidjson::Value& elem, const char* key);

	VarOrPredicate getVarOrPredicateKey(Game& game,
		const rapidjson::Value& elem, const char* key);
}
