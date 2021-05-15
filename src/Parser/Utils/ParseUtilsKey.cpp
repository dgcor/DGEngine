#include "ParseUtilsKey.h"
#include "Json/JsonUtils.h"
#include "GameUtils.h"

namespace Parser
{
	using namespace rapidjson;

	AnimationType getAnimationTypeKey(const rapidjson::Value& elem,
		const std::string_view key, AnimationType val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getAnimationType(keyElem.GetStringView(), val);
			}
		}
		return val;
	}

	Anchor getAnchorKey(const Value& elem, const std::string_view key, Anchor val)
	{
		if (elem.HasMember(key) == true)
		{
			return getAnchorVal(elem[key], val);
		}
		return val;
	}

	BindingFlags getBindingFlagsKey(const Value& elem, const std::string_view key, BindingFlags val)
	{
		if (elem.HasMember(key) == true)
		{
			return getBindingFlagsVal(elem[key], val);
		}
		return val;
	}

	BlendMode getBlendModeKey(const Value& elem, const std::string_view key, BlendMode val)
	{
		if (elem.HasMember(key) == true)
		{
			return getBlendModeVal(elem[key], val);
		}
		return val;
	}

	bool getBoolKey(const Value& elem, const std::string_view key, bool val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsBool() == true)
		{
			return elem[key].GetBool();
		}
		return val;
	}

	double getDoubleKey(const Value& elem, const std::string_view key, double val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsDouble() == true)
		{
			return elem[key].GetDouble();
		}
		return val;
	}

	float getFloatKey(const Value& elem, const std::string_view key, float val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsFloat() == true)
		{
			return elem[key].GetFloat();
		}
		return val;
	}

	int getIntKey(const Value& elem, const std::string_view key, int val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsInt() == true)
		{
			return elem[key].GetInt();
		}
		return val;
	}

	int64_t getInt64Key(const Value& elem, const std::string_view key, int64_t val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsInt64() == true)
		{
			return elem[key].GetInt64();
		}
		return val;
	}

	const char* getStringCharKey(const Value& elem,
		const std::string_view key, const char* val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsString() == true)
		{
			return elem[key].GetString();
		}
		return val;
	}

	std::string getStringKey(const Value& elem,
		const std::string_view key, const std::string_view val)
	{
		if (elem.HasMember(key) == true)
		{
			return getStringVal(elem[key], val);
		}
		return std::string(val);
	}

	std::string_view getStringViewKey(const Value& elem,
		const std::string_view key, const std::string_view val)
	{
		if (elem.HasMember(key) == true)
		{
			return getStringViewVal(elem[key], val);
		}
		return val;
	}

	unsigned getUIntKey(const Value& elem, const std::string_view key, unsigned val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsUint() == true)
		{
			return elem[key].GetUint();
		}
		return val;
	}

	uint64_t getUInt64Key(const Value& elem, const std::string_view key, uint64_t val)
	{
		if (elem.HasMember(key) == true
			&& elem[key].IsUint64() == true)
		{
			return elem[key].GetUint64();
		}
		return val;
	}

	std::pair<uint32_t, uint32_t> getFramesKey(const Value& elem,
		const std::string_view key, const std::pair<uint32_t, uint32_t>& val)
	{
		return getRange1Key<std::pair<uint32_t, uint32_t>, uint32_t>(elem, key, val);
	}

	sf::Vector2f getPositionKey(const Value& elem, const std::string_view key,
		const sf::Vector2f& size, const sf::Vector2u& refSize)
	{
		if (elem.HasMember(key) == true)
		{
			return getPositionVal(elem[key], size, refSize);
		}
		return {};
	}

	sf::IntRect getIntRectKey(const Value& elem,
		const std::string_view key, const sf::IntRect& val)
	{
		if (elem.HasMember(key) == true)
		{
			return getIntRectVal(elem[key], val);
		}
		return val;
	}

	sf::FloatRect getFloatRectKey(const Value& elem,
		const std::string_view key, const sf::FloatRect& val)
	{
		if (elem.HasMember(key) == true)
		{
			return getFloatRectVal(elem[key], val);
		}
		return val;
	}

	sf::Color getColorKey(const Value& elem, const std::string_view key, const sf::Color& val)
	{
		if (elem.HasMember(key) == true)
		{
			return getColorVal(elem[key], val);
		}
		return val;
	}

	sf::Time getTimeKey(const Value& elem, const std::string_view key, const sf::Time& val)
	{
		if (elem.HasMember(key) == true)
		{
			return getTimeVal(elem[key], val);
		}
		return val;
	}

	std::vector<std::string> getStringVectorKey(const Value& elem, const std::string_view key)
	{
		std::vector<std::string> vec;
		if (elem.HasMember(key) == true)
		{
			const auto& elemVal = elem[key];
			if (elemVal.IsArray() == true)
			{
				for (const auto& val : elemVal)
				{
					vec.push_back(JsonUtils::toString(val));
				}
			}
			else if (elemVal.IsString() == true)
			{
				vec.push_back(elemVal.GetStringStr());
			}
		}
		return vec;
	}

	IgnoreResource getIgnoreResourceKey(const Value& elem,
		const std::string_view key, IgnoreResource val)
	{
		if (elem.HasMember(key) == true)
		{
			return getIgnoreResourceVal(elem[key], val);
		}
		return val;
	}

	InputEventType getInputEventTypeKey(const Value& elem,
		const std::string_view key, InputEventType val)
	{
		if (elem.HasMember(key) == true)
		{
			return getInputEventTypeVal(elem[key], val);
		}
		return val;
	}

	sf::PrimitiveType getPrimitiveTypeKey(const Value& elem,
		const std::string_view key, sf::PrimitiveType val)
	{
		if (elem.HasMember(key) == true)
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString() == true)
			{
				return GameUtils::getPrimitiveType(keyElem.GetStringView(), val);
			}
		}
		return val;
	}

	static Value nullValue(Type::kNullType);

	const Value& getQueryKey(const Value* elem, const Value& query, const std::string_view key)
	{
		if (query.HasMember(key) == true)
		{
			if (elem != nullptr)
			{
				return getQueryVal(*elem, query[key]);
			}
			return query[key];
		}
		return nullValue;
	}

	const Value& getQueryKey(const Value& elem, const Value& query, const std::string_view key)
	{
		if (query.HasMember(key) == true)
		{
			return getQueryVal(elem, query[key]);
		}
		return nullValue;
	}

	ReplaceVars getReplaceVarsKey(const Value& elem, const std::string_view key, ReplaceVars val)
	{
		if (elem.HasMember(key) == true)
		{
			return getReplaceVarsVal(elem[key]);
		}
		return val;
	}

	Variable getVariableKey(const Value& elem, const std::string_view key)
	{
		if (elem.HasMember(key) == true)
		{
			return getVariableVal(elem[key]);
		}
		return Variable();
	}

	std::vector<std::pair<std::string, Variable>> getVariablesKey(
		const rapidjson::Value& elem, const std::string_view key)
	{
		if (elem.HasMember(key) == true &&
			elem[key].IsObject() == true)
		{
			return getVariables(elem[key]);
		}
		return {};
	}

	UnorderedStringMap<Variable> getVariablesMapKey(
		const rapidjson::Value& elem, const std::string_view key)
	{
		if (elem.HasMember(key) == true &&
			elem[key].IsObject() == true)
		{
			return getVariablesMap(elem[key]);
		}
		return {};
	}

	VarOrPredicate getVarOrPredicateKey(Game& game, const Value& elem, const std::string_view key)
	{
		if (elem.HasMember(key) == true)
		{
			return getVarOrPredicateVal(game, elem[key]);
		}
		return VarOrPredicate();
	}
}
