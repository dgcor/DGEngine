#pragma once

#include "AnimationType.h"
#include "Json/JsonParser.h"
#include "ParseUtilsVal.h"
#include <SFML/Graphics/PrimitiveType.hpp>

namespace Parser
{
	AnimationType getAnimationTypeKey(const rapidjson::Value& elem,
		const std::string_view key, AnimationType val = AnimationType::Looped);

	Anchor getAnchorKey(const rapidjson::Value& elem,
		const std::string_view key, Anchor val = Anchor::Top | Anchor::Left);

	BindingFlags getBindingFlagsKey(const rapidjson::Value& elem,
		const std::string_view key, BindingFlags val = BindingFlags::OnChange);

	BlendMode getBlendModeKey(const rapidjson::Value& elem,
		const std::string_view key, BlendMode val = BlendMode::Alpha);

	bool getBoolKey(const rapidjson::Value& elem, const std::string_view key, bool val = {});

	double getDoubleKey(const rapidjson::Value& elem, const std::string_view key, double val = {});

	float getFloatKey(const rapidjson::Value& elem, const std::string_view key, float val = {});

	int getIntKey(const rapidjson::Value& elem, const std::string_view key, int val = {});

	int64_t getInt64Key(const rapidjson::Value& elem, const std::string_view key, int64_t val = {});

	const char* getStringCharKey(const rapidjson::Value& elem,
		const std::string_view key, const char* val = "");

	std::string getStringKey(const rapidjson::Value& elem,
		const std::string_view key, const std::string_view val = {});

	std::string_view getStringViewKey(const rapidjson::Value& elem,
		const std::string_view key, const std::string_view val = "");

	unsigned getUIntKey(const rapidjson::Value& elem,
		const std::string_view key, unsigned val = {});

	uint64_t getUInt64Key(const rapidjson::Value& elem,
		const std::string_view key, uint64_t val = {});

	template<class T>
	T getNumberKey(const rapidjson::Value& elem, const std::string_view key, T val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getNumberVal<T>(elem[key], val);
		}
		return val;
	}

	template<class T>
	T getUnsignedNumberKey(const rapidjson::Value& elem, const std::string_view key, T val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getUnsignedNumberVal<T>(elem[key], val);
		}
		return val;
	}

	template <class T, class NumType>
	T getVector2NumberKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getVector2NumberVal<T, NumType>(elem[key], val);
		}
		return val;
	}

	template <class T, class NumType>
	T getVector2UnsignedNumberKey(const rapidjson::Value& elem,
		const std::string_view key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getVector2UnsignedNumberVal<T, NumType>(elem[key], val);
		}
		return val;
	}

	template <class T>
	T getVector2fKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		return getVector2NumberKey<T, float>(elem, key, val);
	}

	template <class T>
	T getVector2iKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		return getVector2NumberKey<T, int32_t>(elem, key, val);
	}

	template <class T>
	T getVector2uKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		return getVector2NumberKey<T, uint32_t>(elem, key, val);
	}

	template <class T, class NumType>
	T getVector3NumberKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getVector3NumberVal<T, NumType>(elem[key], val);
		}
		return val;
	}

	template <class T>
	T getVector3iKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		return getVector3NumberKey<T, float>(elem, key, val);
	}

	template <class T>
	T getVector3fKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		return getVector3NumberKey<T, float>(elem, key, val);
	}

	template <class T, class NumType>
	T getVector4NumberKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getVector4NumberVal<T, NumType>(elem[key], val);
		}
		return val;
	}

	template <class T>
	T getVector4iKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		return getVector4NumberKey<T, float>(elem, key, val);
	}

	template <class T>
	T getVector4fKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		return getVector4NumberKey<T, float>(elem, key, val);
	}

	template <class T, class NumType>
	T getRangeOrValueKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getRangeOrValueVal<T, NumType>(elem[key], val);
		}
		return val;
	}

	template <class T, class NumType, NumType minVal>
	T getRangeNKey(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getRangeNVal<T, NumType, minVal>(elem[key], val);
		}
		return val;
	}

	template <class T, class NumType>
	T getRange0Key(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getRange0Val<T, NumType>(elem[key], val);
		}
		return val;
	}

	template <class T, class NumType>
	T getRange1Key(const rapidjson::Value& elem, const std::string_view key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getRange1Val<T, NumType>(elem[key], val);
		}
		return val;
	}

	std::pair<uint32_t, uint32_t> getFramesKey(const rapidjson::Value& elem,
		const std::string_view key, const std::pair<uint32_t, uint32_t>& val = {});

	sf::Vector2f getPositionKey(const rapidjson::Value& elem,
		const std::string_view key, const sf::Vector2f& size, const sf::Vector2u& refSize);

	sf::IntRect getIntRectKey(const rapidjson::Value& elem,
		const std::string_view key, const sf::IntRect& val = {});

	sf::FloatRect getFloatRectKey(const rapidjson::Value& elem,
		const std::string_view key, const sf::FloatRect& val = {});

	sf::Color getColorKey(const rapidjson::Value& elem, const std::string_view key,
		const sf::Color& val = {});

	sf::Time getTimeKey(const rapidjson::Value& elem, const std::string_view key,
		const sf::Time& val = {});

	std::vector<std::string> getStringVectorKey(const rapidjson::Value& elem, const std::string_view key);

	IgnoreResource getIgnoreResourceKey(const rapidjson::Value& elem,
		const std::string_view key, IgnoreResource val = IgnoreResource::None);

	InputEventType getInputEventTypeKey(const rapidjson::Value& elem,
		const std::string_view key, InputEventType val = InputEventType::None);

	template <class T>
	T getMinMaxIntKey(const rapidjson::Value& elem, const std::string_view key, T val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getMinMaxIntVal<T>(elem[key], val);
		}
		return val;
	}

	sf::PrimitiveType getPrimitiveTypeKey(const rapidjson::Value& elem,
		const std::string_view key, sf::PrimitiveType val = sf::PrimitiveType::TrianglesStrip);

	const rapidjson::Value& getQueryKey(const rapidjson::Value* elem,
		const rapidjson::Value& query, const std::string_view key);

	const rapidjson::Value& getQueryKey(const rapidjson::Value& elem,
		const rapidjson::Value& query, const std::string_view key);

	ReplaceVars getReplaceVarsKey(const rapidjson::Value& elem,
		const std::string_view key, ReplaceVars val = ReplaceVars::None);

	Variable getVariableKey(const rapidjson::Value& elem, const std::string_view key);

	std::vector<std::pair<std::string, Variable>> getVariablesKey(
		const rapidjson::Value& elem, const std::string_view key);

	UnorderedStringMap<Variable> getVariablesMapKey(
		const rapidjson::Value& elem, const std::string_view key);

	VarOrPredicate getVarOrPredicateKey(Game& game,
		const rapidjson::Value& elem, const std::string_view key);
}
