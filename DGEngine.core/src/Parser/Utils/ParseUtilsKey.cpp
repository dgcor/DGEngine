#include "ParseUtilsKey.h"

namespace Parser
{
	using namespace rapidjson;

	AnimationType getAnimationTypeKey(const Value& elem, const std::string_view key, AnimationType val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getAnimationTypeVal(elem[key], val);
		}
		return val;
	}

	Anchor getAnchorKey(const Value& elem, const std::string_view key, Anchor val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getAnchorVal(elem[key], val);
		}
		return val;
	}

	BindingFlags getBindingFlagsKey(const Value& elem, const std::string_view key, BindingFlags val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getBindingFlagsVal(elem[key], val);
		}
		return val;
	}

	BlendMode getBlendModeKey(const Value& elem, const std::string_view key, BlendMode val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getBlendModeVal(elem[key], val);
		}
		return val;
	}

	Direction getDirectionKey(const Value& elem, const std::string_view key, Direction val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getDirectionVal(elem[key], val);
		}
		return val;
	}

	bool getBoolKey(const Value& elem, const std::string_view key, bool val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getBoolVal(elem[key], val);
		}
		return val;
	}

	double getDoubleKey(const Value& elem, const std::string_view key, double val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getDoubleVal(elem[key], val);
		}
		return val;
	}

	float getFloatKey(const Value& elem, const std::string_view key, float val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getFloatVal(elem[key], val);
		}
		return val;
	}

	int getIntKey(const Value& elem, const std::string_view key, int val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getIntVal(elem[key], val);
		}
		return val;
	}

	int64_t getInt64Key(const Value& elem, const std::string_view key, int64_t val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getInt64Val(elem[key], val);
		}
		return val;
	}

	const char* getStringCharKey(const Value& elem, const std::string_view key, const char* val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getStringCharVal(elem[key], val);
		}
		return val;
	}

	std::string getStringKey(const Value& elem, const std::string_view key, const std::string_view val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getStringVal(elem[key], val);
		}
		return std::string(val);
	}

	std::string_view getStringViewKey(const Value& elem, const std::string_view key, const std::string_view val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getStringViewVal(elem[key], val);
		}
		return val;
	}

	unsigned getUIntKey(const Value& elem, const std::string_view key, unsigned val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getUIntVal(elem[key], val);
		}
		return val;
	}

	uint64_t getUInt64Key(const Value& elem, const std::string_view key, uint64_t val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getUInt64Val(elem[key], val);
		}
		return val;
	}

	sf::Vector2f getPositionKey(const Value& elem, const std::string_view key,
		const sf::Vector2f& size, const sf::Vector2u& refSize)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getPositionVal(elem[key], size, refSize);
		}
		return {};
	}

	sf::Vector2f getSizeKey(const Value& elem, const std::string_view key, const sf::Vector2f& val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getSizeVal(elem[key], val);
		}
		return val;
	}

	sf::IntRect getIntRectKey(const Value& elem, const std::string_view key, const sf::IntRect& val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getIntRectVal(elem[key], val);
		}
		return val;
	}

	sf::FloatRect getFloatRectKey(const Value& elem, const std::string_view key, const sf::FloatRect& val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getFloatRectVal(elem[key], val);
		}
		return val;
	}

	sf::Color getColorKey(const Value& elem, const std::string_view key, const sf::Color& val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getColorVal(elem[key], val);
		}
		return val;
	}

	sf::Time getTimeKey(const Value& elem, const std::string_view key, const sf::Time& val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getTimeVal(elem[key], val);
		}
		return val;
	}

	sf::Time getTimeUKey(const Value& elem, const std::string_view key, const sf::Time& val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getTimeUVal(elem[key], val);
		}
		return val;
	}

	std::vector<std::string> getStringVectorKey(const Value& elem, const std::string_view key)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getStringVectorVal(elem[key]);
		}
		return {};
	}

	IgnoreResource getIgnoreResourceKey(const Value& elem, const std::string_view key, IgnoreResource val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getIgnoreResourceVal(elem[key], val);
		}
		return val;
	}

	InputEventType getInputEventTypeKey(const Value& elem, const std::string_view key, InputEventType val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getInputEventTypeVal(elem[key], val);
		}
		return val;
	}

	sf::PrimitiveType getPrimitiveTypeKey(const Value& elem, const std::string_view key, sf::PrimitiveType val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getPrimitiveTypeVal(elem[key], val);
		}
		return val;
	}

	static Value nullValue(Type::kNullType);

	const Value& getQueryKey(const Value* elem, const Value& query, const std::string_view key)
	{
		if (query.IsObject() == true &&
			query.HasMember(key) == true)
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
		if (query.IsObject() == true &&
			query.HasMember(key) == true)
		{
			return getQueryVal(elem, query[key]);
		}
		return nullValue;
	}

	ReplaceVars getReplaceVarsKey(const Value& elem, const std::string_view key, ReplaceVars val)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getReplaceVarsVal(elem[key]);
		}
		return val;
	}

	Variable getVariableKey(const Value& elem, const std::string_view key)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getVariableVal(elem[key]);
		}
		return {};
	}

	std::vector<std::pair<std::string, Variable>> getVariablesKey(const Value& elem, const std::string_view key)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getVariables(elem[key]);
		}
		return {};
	}

	UnorderedStringMap<Variable> getVariablesMapKey(const Value& elem, const std::string_view key)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getVariablesMap(elem[key]);
		}
		return {};
	}

	VarOrPredicate getVarOrPredicateKey(Game& game, const Value& elem, const std::string_view key)
	{
		if (elem.IsObject() == true &&
			elem.HasMember(key) == true)
		{
			return getVarOrPredicateVal(game, elem[key]);
		}
		return {};
	}
}
