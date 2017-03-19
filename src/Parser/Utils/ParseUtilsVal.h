#pragma once

#include "Game/GameProperties.h"
#include "Game/ItemLocation.h"
#include "Json/JsonParser.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Variable.h"

namespace Parser
{
	bool getBoolVal(const rapidjson::Value& elem, bool val = {});

	double getDoubleVal(const rapidjson::Value& elem, double val = {});

	float getFloatVal(const rapidjson::Value& elem, float val = {});

	int getIntVal(const rapidjson::Value& elem, int val = {});

	int64_t getInt64Val(const rapidjson::Value& elem, int64_t val = {});

	const char* getStringCharVal(const rapidjson::Value& elem, const char* val = "");

	std::string getStringVal(const rapidjson::Value& elem, const std::string& val = {});

	unsigned getUIntVal(const rapidjson::Value& elem, unsigned val = {});

	uint64_t getUInt64Val(const rapidjson::Value& elem, uint64_t val = {});

	template <class T>
	T getVector2fVal(const rapidjson::Value& elem, const T& val = {})
	{
		if (elem.IsArray() == true
			&& elem.Size() > 1
			&& elem[0].IsNumber() == true
			&& elem[1].IsNumber() == true)
		{
			return T(elem[0].GetFloat(), elem[1].GetFloat());
		}
		return val;
	}

	template <class T>
	T getVector2iVal(const rapidjson::Value& elem, const T& val = {})
	{
		if (elem.IsArray() == true
			&& elem.Size() > 1
			&& elem[0].IsNumber() == true
			&& elem[1].IsNumber() == true)
		{
			return T(elem[0].GetInt(), elem[1].GetInt());
		}
		return val;
	}

	template <class T>
	T getVector2uVal(const rapidjson::Value& elem, const T& val = {})
	{
		if (elem.IsArray() == true
			&& elem.Size() > 1
			&& elem[0].IsNumber() == true
			&& elem[1].IsNumber() == true)
		{
			return T(elem[0].GetUint(), elem[1].GetUint());
		}
		return val;
	}

	sf::IntRect getIntRectVal(const rapidjson::Value& elem,
		const sf::IntRect& val = {});

	sf::FloatRect getFloatRectVal(const rapidjson::Value& elem,
		const sf::FloatRect& val = {});

	sf::Keyboard::Key getKeyCodeVal(const rapidjson::Value& elem,
		sf::Keyboard::Key val = sf::Keyboard::Unknown);

	size_t getInventoryItemIndexVal(const rapidjson::Value& elem,
		PlayerInventory inv);

	InventoryPosition getInventoryPositionVal(const rapidjson::Value& elem,
		InventoryPosition val = InventoryPosition::TopLeft);

	template <class T>
	T getMinMaxIntVal(const rapidjson::Value& elem, T val = {})
	{
		if (elem.IsInt() == true)
		{
			return elem.GetInt();
		}
		else if (elem.IsBool() == true)
		{
			return (T)elem.GetBool();
		}
		else if (elem.IsString() == true)
		{
			if (elem.GetString() == std::string("min"))
			{
				return std::numeric_limits<T>::min();
			}
			else if (elem.GetString() == std::string("max"))
			{
				return std::numeric_limits<T>::max();
			}
		}
		return val;
	}

	ItemXY getItemXYVal(const rapidjson::Value& elem, const ItemXY& val = {});

	ItemCoordInventory getItemCoordInventoryVal(const rapidjson::Value& elem);

	ItemLocation getItemLocationVal(const rapidjson::Value& elem);

	PlayerInventory getPlayerInventoryVal(const rapidjson::Value& elem,
		PlayerInventory val = PlayerInventory::Body);

	Variable getVariableVal(const rapidjson::Value& elem);
}
