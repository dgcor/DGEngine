#pragma once

#include "Game/GameProperties.h"
#include "Game/ItemLocation.h"
#include "Game/Number.h"
#include "IgnoreResource.h"
#include "Json/JsonParser.h"
#include "Parser/ParserProperties.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Variable.h"
#include "VarOrPredicate.h"

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

	std::pair<size_t, size_t> getFramesVal(const rapidjson::Value& elem,
		const std::pair<size_t, size_t>& val = {});

	std::pair<size_t, size_t> getIndexVal(const rapidjson::Value& elem,
		const std::pair<size_t, size_t>& val = {});

	sf::IntRect getIntRectVal(const rapidjson::Value& elem,
		const sf::IntRect& val = {});

	sf::FloatRect getFloatRectVal(const rapidjson::Value& elem,
		const sf::FloatRect& val = {});

	sf::Color getColorVal(const rapidjson::Value& elem,
		const sf::Color& val = {});

	sf::Keyboard::Key getKeyCodeVal(const rapidjson::Value& elem,
		sf::Keyboard::Key val = sf::Keyboard::Unknown);

	sf::Time getTimeVal(const rapidjson::Value& elem,
		const sf::Time& val = {});

	IgnoreResource getIgnoreResourceVal(const rapidjson::Value& elem,
		IgnoreResource val = IgnoreResource::None);

	size_t getInventoryItemIndexVal(const rapidjson::Value& elem,
		PlayerInventory inv);

	InventoryPosition getInventoryPositionVal(const rapidjson::Value& elem,
		InventoryPosition val = InventoryPosition::TopLeft);

	template <class T>
	T getMinMaxIntVal(const rapidjson::Value& elem, T val = {})
	{
		if (elem.IsUint() == true)
		{
			return (T)elem.GetUint();
		}
		if (elem.IsInt() == true)
		{
			return (T)elem.GetInt();
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

	Number32 getMinMaxNumber32Val(const rapidjson::Value& elem);

	ItemXY getItemXYVal(const rapidjson::Value& elem, const ItemXY& val = {});

	ItemCoordInventory getItemCoordInventoryVal(const rapidjson::Value& elem);

	ItemLocation getItemLocationVal(const rapidjson::Value& elem);

	AnimationSpeed getPlayerAnimationSpeedVal(const rapidjson::Value& elem);

	PlayerInventory getPlayerInventoryVal(const rapidjson::Value& elem,
		PlayerInventory val = PlayerInventory::Body);

	ReplaceVars getReplaceVarsVal(const rapidjson::Value& elem,
		ReplaceVars val = ReplaceVars::None);

	Variable getVariableVal(const rapidjson::Value& elem);

	VarOrPredicate getVarOrPredicateVal(Game& game, const rapidjson::Value& elem);
}
