#pragma once

#include "Game/PlayerProperties.h"
#include "Json/JsonParser.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Variable.h"
#include <vector>

namespace Parser
{
	bool isValidArray(const rapidjson::Value& elem, const char* key);
	bool isValidString(const rapidjson::Value& elem, const char* key);

	Anchor getAnchorKey(const rapidjson::Value& elem,
		const char* key, Anchor val = Anchor::Top | Anchor::Left);

	bool getBoolVal(const rapidjson::Value& elem, bool val = false);
	bool getBoolKey(const rapidjson::Value& elem, const char* key, bool val = false);
	bool getBoolIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, bool val = false);
	double getDoubleVal(const rapidjson::Value& elem, double val = 0.0);
	double getDoubleKey(const rapidjson::Value& elem, const char* key, double val = 0.0);
	double getDoubleIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, double val = 0.0);
	int getIntVal(const rapidjson::Value& elem, int val = 0);
	int getIntKey(const rapidjson::Value& elem, const char* key, int val = 0);
	int getIntIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, int val = 0);
	int64_t getInt64Val(const rapidjson::Value& elem, int64_t val = 0);
	int64_t getInt64Key(const rapidjson::Value& elem, const char* key, int64_t val = 0);
	int64_t getInt64Idx(const rapidjson::Value& elem, rapidjson::SizeType idx, int64_t val = 0);
	const char* getStringCharVal(const rapidjson::Value& elem, const char* val = "");
	std::string getStringVal(const rapidjson::Value& elem, const std::string& val = "");
	const char* getStringCharKey(const rapidjson::Value& elem,
		const char* key, const char* val = "");
	std::string getStringKey(const rapidjson::Value& elem,
		const char* key, const std::string& val = "");
	const char* getStringCharIdx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, const char* val = "");
	std::string getStringIdx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, const std::string& val = "");
	unsigned getUIntVal(const rapidjson::Value& elem, unsigned val = 0);
	unsigned getUIntKey(const rapidjson::Value& elem,
		const char* key, unsigned val = 0);
	unsigned getUIntIdx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, unsigned val = 0);
	uint64_t getUInt64Val(const rapidjson::Value& elem, uint64_t val = 0);
	uint64_t getUInt64Key(const rapidjson::Value& elem,
		const char* key, uint64_t val = 0);
	uint64_t getUInt64Idx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, uint64_t val = 0);

	template <class T>
	T getVector2fVal(const rapidjson::Value& elem, const T& val = T())
	{
		if (elem.IsArray() && elem.Size() > 1)
		{
			return T(elem[0].GetDouble(), elem[1].GetDouble());
		}
		else
		{
			return val;
		}
	}

	template <class T>
	T getVector2fKey(const rapidjson::Value& elem, const char* key, const T& val = T())
	{
		if (elem.HasMember(key) && elem[key].IsArray())
		{
			return T(elem[key][0].GetDouble(), elem[key][1].GetDouble());
		}
		else
		{
			return val;
		}
	}

	template <class T>
	T getVector2iVal(const rapidjson::Value& elem, const T& val = T())
	{
		if (elem.IsArray() && elem.Size() > 1)
		{
			return T(elem[0].GetInt(), elem[1].GetInt());
		}
		else
		{
			return val;
		}
	}

	template <class T>
	T getVector2iKey(const rapidjson::Value& elem, const char* key, const T& val = T())
	{
		if (elem.HasMember(key) && elem[key].IsArray())
		{
			return T(elem[key][0].GetInt(), elem[key][1].GetInt());
		}
		else
		{
			return val;
		}
	}

	template <class T>
	T getVector2uVal(const rapidjson::Value& elem, const T& val = T())
	{
		if (elem.IsArray() && elem.Size() > 1)
		{
			return T(elem[0].GetUint(), elem[1].GetUint());
		}
		else
		{
			return val;
		}
	}

	template <class T>
	T getVector2uKey(const rapidjson::Value& elem, const char* key, const T& val = T())
	{
		if (elem.HasMember(key) && elem[key].IsArray())
		{
			return T(elem[key][0].GetUint(), elem[key][1].GetUint());
		}
		else
		{
			return val;
		}
	}

	sf::IntRect getIntRectKey(const rapidjson::Value& elem,
		const char* key, const sf::IntRect& val = sf::IntRect());
	sf::IntRect getIntRectVal(const rapidjson::Value& elem,
		const sf::IntRect& val = sf::IntRect());
	sf::FloatRect getFloatRectKey(const rapidjson::Value& elem,
		const char* key, const sf::FloatRect& val = sf::FloatRect());
	sf::FloatRect getFloatRectVal(const rapidjson::Value& elem,
		const sf::FloatRect& val = sf::FloatRect());

	sf::Color getColorKey(const rapidjson::Value& elem, const char* key,
		const sf::Color& val = sf::Color());

	std::vector<std::string> getStringVectorKey(const rapidjson::Value& elem, const char* key);

	sf::Keyboard::Key getKeyCodeKey(const rapidjson::Value& elem,
		sf::Keyboard::Key val = sf::Keyboard::Unknown);

	IgnoreResource getIgnoreResourceKey(const rapidjson::Value& elem,
		const char* key, IgnoreResource val = IgnoreResource::None);

	PlayerDirection getPlayerDirectionKey(const rapidjson::Value& elem,
		const char* key, PlayerDirection val = PlayerDirection::All);

	PlayerStatus getPlayerStatusKey(const rapidjson::Value& elem,
		const char* key, PlayerStatus val = PlayerStatus::Stand1);

	Variable getVariableKey(const rapidjson::Value& elem, const char* key);
}
