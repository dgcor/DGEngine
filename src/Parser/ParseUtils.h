#pragma once

#include "Json/JsonParser.h"
#include "ResourceManager.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Variable.h"
#include <vector>

namespace Parser
{
	bool isValidString(const rapidjson::Value& elem, const char* key);

	Anchor getAnchor(const rapidjson::Value& elem, const char* key, Anchor val = Anchor::Top | Anchor::Left);

	bool getBool(const rapidjson::Value& elem, bool val = false);
	bool getBool(const rapidjson::Value& elem, const char* key, bool val = false);
	bool getBool(const rapidjson::Value& elem, rapidjson::SizeType idx, bool val = false);
	double getDouble(const rapidjson::Value& elem, double val = 0.0);
	double getDouble(const rapidjson::Value& elem, const char* key, double val = 0.0);
	double getDouble(const rapidjson::Value& elem, rapidjson::SizeType idx, double val = 0.0);
	int getInt(const rapidjson::Value& elem, int val = 0);
	int getInt(const rapidjson::Value& elem, const char* key, int val = 0);
	int getInt(const rapidjson::Value& elem, rapidjson::SizeType idx, int val = 0);
	int64_t getInt64(const rapidjson::Value& elem, int64_t val = 0);
	int64_t getInt64(const rapidjson::Value& elem, const char* key, int64_t val = 0);
	int64_t getInt64(const rapidjson::Value& elem, rapidjson::SizeType idx, int64_t val = 0);
	const char* getStringChar_(const rapidjson::Value& elem, const char* val = "");
	std::string getString(const rapidjson::Value& elem, const std::string& val = "");
	const char* getStringChar(const rapidjson::Value& elem, const char* key, const char* val = "");
	std::string getString(const rapidjson::Value& elem, const char* key, const std::string& val = "");
	const char* getStringChar_(const rapidjson::Value& elem, rapidjson::SizeType idx, const char* val = "");
	std::string getString_(const rapidjson::Value& elem, rapidjson::SizeType idx, const std::string& val = "");
	unsigned getUInt_(const rapidjson::Value& elem, unsigned val = 0);
	unsigned getUInt(const rapidjson::Value& elem, const char* key, unsigned val = 0);
	unsigned getUInt(const rapidjson::Value& elem, rapidjson::SizeType idx, unsigned val = 0);
	uint64_t getUInt64(const rapidjson::Value& elem, uint64_t val = 0);
	uint64_t getUInt64(const rapidjson::Value& elem, const char* key, uint64_t val = 0);
	uint64_t getUInt64(const rapidjson::Value& elem, rapidjson::SizeType idx, uint64_t val = 0);

	template <class T>
	T getVector2f(const rapidjson::Value& elem, const T& val = T())
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
	T getVector2f(const rapidjson::Value& elem, const char* key, const T& val = T())
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
	T getVector2i(const rapidjson::Value& elem, const T& val = T())
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
	T getVector2i(const rapidjson::Value& elem, const char* key, const T& val = T())
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
	T getVector2u(const rapidjson::Value& elem, const T& val = T())
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
	T getVector2u(const rapidjson::Value& elem, const char* key, const T& val = T())
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

	sf::IntRect getIntRect(const rapidjson::Value& elem, const char* key, const sf::IntRect& val = sf::IntRect());
	sf::IntRect getIntRect(const rapidjson::Value& elem, const sf::IntRect& val = sf::IntRect());
	sf::FloatRect getFloatRect(const rapidjson::Value& elem, const char* key, const sf::FloatRect& val = sf::FloatRect());
	sf::FloatRect getFloatRect(const rapidjson::Value& elem, const sf::FloatRect& val = sf::FloatRect());

	sf::Color getColor(const rapidjson::Value& elem, const char* key, const sf::Color& val = sf::Color());

	std::vector<std::string> getStringVector(const rapidjson::Value& elem, const char* key);

	sf::Keyboard::Key getKeyCode(const rapidjson::Value& elem, sf::Keyboard::Key val = sf::Keyboard::Unknown);

	IgnoreResource getIgnoreResource(const rapidjson::Value& elem, const char* key, IgnoreResource val = IgnoreResource::None);

	Variable getVariable(const rapidjson::Value& elem, const char* key);
}
