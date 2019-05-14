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

	std::string_view getStringViewKey(const rapidjson::Value& elem,
		const char* key, const std::string_view val = "");

	unsigned getUIntKey(const rapidjson::Value& elem,
		const char* key, unsigned val = {});

	uint64_t getUInt64Key(const rapidjson::Value& elem,
		const char* key, uint64_t val = {});

	template<class T>
	T getNumberKey(const rapidjson::Value& elem, const char* key, T val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getNumberVal<T>(elem[key], val);
		}
		return val;
	}

	template<class T>
	T getUnsignedNumberKey(const rapidjson::Value& elem, const char* key, T val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getUnsignedNumberVal<T>(elem[key], val);
		}
		return val;
	}

	template <class T, class NumType>
	T getVector2NumberKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getVector2NumberVal<T, NumType>(elem[key], val);
		}
		return val;
	}

	template <class T, class NumType>
	T getVector2UnsignedNumberKey(const rapidjson::Value& elem,
		const char* key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getVector2UnsignedNumberVal<T, NumType>(elem[key], val);
		}
		return val;
	}

	template <class T>
	T getVector2fKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		return getVector2NumberKey<T, float>(elem, key, val);
	}

	template <class T>
	T getVector2iKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		return getVector2NumberKey<T, int32_t>(elem, key, val);
	}

	template <class T>
	T getVector2uKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		return getVector2NumberKey<T, uint32_t>(elem, key, val);
	}

	template <class T, class NumType>
	T getVector3NumberKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getVector3NumberVal<T, NumType>(elem[key], val);
		}
		return val;
	}

	template <class T>
	T getVector3iKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		return getVector3NumberKey<T, float>(elem, key, val);
	}

	template <class T>
	T getVector3fKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		return getVector3NumberKey<T, float>(elem, key, val);
	}

	template <class T, class NumType>
	T getVector4NumberKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		if (elem.HasMember(key) == true)
		{
			return getVector4NumberVal<T, NumType>(elem[key], val);
		}
		return val;
	}

	template <class T>
	T getVector4iKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		return getVector4NumberKey<T, float>(elem, key, val);
	}

	template <class T>
	T getVector4fKey(const rapidjson::Value& elem, const char* key, const T& val = {})
	{
		return getVector4NumberKey<T, float>(elem, key, val);
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

	InputEvent getInputEventKey(const rapidjson::Value& elem,
		const char* key, InputEvent val = InputEvent::None);

	size_t getInventoryItemIndexKey(const rapidjson::Value& elem,
		const char* key, PlayerInventory inv);

	InventoryPosition getInventoryPositionKey(const rapidjson::Value& elem,
		const char* key, InventoryPosition val = InventoryPosition::TopLeft);

	LightSource getLightSourceKey(const rapidjson::Value& elem,
		const char* key, LightSource val = {});

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

	PairUInt8 getItemXYKey(const rapidjson::Value& elem, const char* key, const PairUInt8& val = {});

	PlayerDirection getPlayerDirectionKey(const rapidjson::Value& elem,
		const char* key, PlayerDirection val = PlayerDirection::All);

	PlayerInventory getPlayerInventoryKey(const rapidjson::Value& elem,
		const char* key, PlayerInventory val = PlayerInventory::Body);

	PlayerItemMount getPlayerItemMountKey(const rapidjson::Value& elem,
		const char* key, PlayerItemMount val = PlayerItemMount::LeftHand);

	PlayerAnimation getPlayerAnimationKey(const rapidjson::Value& elem,
		const char* key, PlayerAnimation val = PlayerAnimation::Stand1);

	PlayerStatus getPlayerStatusKey(const rapidjson::Value& elem,
		const char* key, PlayerStatus val = PlayerStatus::Stand);

	const rapidjson::Value& getQueryKey(const rapidjson::Value* elem,
		const rapidjson::Value& query, const char* key);

	const rapidjson::Value& getQueryKey(const rapidjson::Value& elem,
		const rapidjson::Value& query, const char* key);

	ReplaceVars getReplaceVarsKey(const rapidjson::Value& elem,
		const char* key, ReplaceVars val = ReplaceVars::None);

	Variable getVariableKey(const rapidjson::Value& elem, const char* key);

	VarOrPredicate getVarOrPredicateKey(Game& game,
		const rapidjson::Value& elem, const char* key);
}
