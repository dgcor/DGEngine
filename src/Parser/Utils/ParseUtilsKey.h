#pragma once

#include "Game/PlayerProperties.h"
#include "Json/JsonParser.h"
#include "ParseUtilsVal.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Variable.h"
#include <vector>

namespace Parser
{
	Anchor getAnchorKey(const rapidjson::Value& elem,
		const char* key, Anchor val = Anchor::Top | Anchor::Left);

	bool getBoolKey(const rapidjson::Value& elem, const char* key, bool val = false);

	double getDoubleKey(const rapidjson::Value& elem, const char* key, double val = 0.0);

	int getIntKey(const rapidjson::Value& elem, const char* key, int val = 0);

	int64_t getInt64Key(const rapidjson::Value& elem, const char* key, int64_t val = 0);

	const char* getStringCharKey(const rapidjson::Value& elem,
		const char* key, const char* val = "");

	std::string getStringKey(const rapidjson::Value& elem,
		const char* key, const std::string& val = "");

	unsigned getUIntKey(const rapidjson::Value& elem,
		const char* key, unsigned val = 0);

	uint64_t getUInt64Key(const rapidjson::Value& elem,
		const char* key, uint64_t val = 0);

	template <class T>
	T getVector2fKey(const rapidjson::Value& elem, const char* key, const T& val = T())
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsArray() == true
			&& elem[key].Size() > 1
			&& elem[key][0].IsNumber() == true
			&& elem[key][1].IsNumber() == true)
		{
			return T(elem[key][0].GetDouble(), elem[key][1].GetDouble());
		}
		return val;
	}

	template <class T>
	T getVector2iKey(const rapidjson::Value& elem, const char* key, const T& val = T())
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
	T getVector2uKey(const rapidjson::Value& elem, const char* key, const T& val = T())
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

	sf::IntRect getIntRectKey(const rapidjson::Value& elem,
		const char* key, const sf::IntRect& val = sf::IntRect());

	sf::FloatRect getFloatRectKey(const rapidjson::Value& elem,
		const char* key, const sf::FloatRect& val = sf::FloatRect());

	sf::Color getColorKey(const rapidjson::Value& elem, const char* key,
		const sf::Color& val = sf::Color());

	std::vector<std::string> getStringVectorKey(const rapidjson::Value& elem, const char* key);

	IgnoreResource getIgnoreResourceKey(const rapidjson::Value& elem,
		const char* key, IgnoreResource val = IgnoreResource::None);

	size_t getInventoryItemIndexKey(const rapidjson::Value& elem,
		const char* key, PlayerInventory inv);

	PlayerDirection getPlayerDirectionKey(const rapidjson::Value& elem,
		const char* key, PlayerDirection val = PlayerDirection::All);

	PlayerInventory getPlayerInventoryKey(const rapidjson::Value& elem,
		const char* key, PlayerInventory val = PlayerInventory::Body);

	PlayerItemMount getPlayerItemMountKey(const rapidjson::Value& elem,
		const char* key, PlayerItemMount val = PlayerItemMount::LeftHand);

	PlayerStatus getPlayerStatusKey(const rapidjson::Value& elem,
		const char* key, PlayerStatus val = PlayerStatus::Stand1);

	Variable getVariableKey(const rapidjson::Value& elem, const char* key);
}
