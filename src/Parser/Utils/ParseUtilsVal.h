#pragma once

#include "Game/GameProperties.h"
#include "Json/JsonParser.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Variable.h"

namespace Parser
{
	bool getBoolVal(const rapidjson::Value& elem, bool val = false);

	double getDoubleVal(const rapidjson::Value& elem, double val = 0.0);

	int getIntVal(const rapidjson::Value& elem, int val = 0);

	int64_t getInt64Val(const rapidjson::Value& elem, int64_t val = 0);

	const char* getStringCharVal(const rapidjson::Value& elem, const char* val = "");

	std::string getStringVal(const rapidjson::Value& elem, const std::string& val = "");

	unsigned getUIntVal(const rapidjson::Value& elem, unsigned val = 0);

	uint64_t getUInt64Val(const rapidjson::Value& elem, uint64_t val = 0);

	template <class T>
	T getVector2fVal(const rapidjson::Value& elem, const T& val = T())
	{
		if (elem.IsArray() == true
			&& elem.Size() > 1
			&& elem[0].IsNumber() == true
			&& elem[1].IsNumber() == true)
		{
			return T(elem[0].GetDouble(), elem[1].GetDouble());
		}
		return val;
	}

	template <class T>
	T getVector2iVal(const rapidjson::Value& elem, const T& val = T())
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
	T getVector2uVal(const rapidjson::Value& elem, const T& val = T())
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
		const sf::IntRect& val = sf::IntRect());

	sf::FloatRect getFloatRectVal(const rapidjson::Value& elem,
		const sf::FloatRect& val = sf::FloatRect());

	sf::Keyboard::Key getKeyCodeVal(const rapidjson::Value& elem,
		sf::Keyboard::Key val = sf::Keyboard::Unknown);

	size_t getInventoryItemIndexVal(const rapidjson::Value& elem,
		PlayerInventory inv);

	Variable getVariableVal(const rapidjson::Value& elem);
}
