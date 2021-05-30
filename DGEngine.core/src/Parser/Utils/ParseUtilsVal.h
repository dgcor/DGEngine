#pragma once

#include "Anchor.h"
#include "BindingFlags.h"
#include "BlendMode.h"
#include "IgnoreResource.h"
#include "InputEvent.h"
#include "Json/JsonParser.h"
#include "Parser/ParserProperties.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>
#include "Utils/Number.h"
#include "Utils/PairXY.h"
#include "Utils/UnorderedStringMap.h"
#include "Variable.h"
#include "VarOrPredicate.h"

namespace Parser
{
	Anchor getAnchorVal(const rapidjson::Value& elem, Anchor val = Anchor::Top | Anchor::Left);

	BindingFlags getBindingFlagsVal(const rapidjson::Value& elem, BindingFlags val = BindingFlags::OnChange);

	BlendMode getBlendModeVal(const rapidjson::Value& elem, BlendMode val = BlendMode::Alpha);

	bool getBoolVal(const rapidjson::Value& elem, bool val = {});

	double getDoubleVal(const rapidjson::Value& elem, double val = {});

	float getFloatVal(const rapidjson::Value& elem, float val = {});

	int getIntVal(const rapidjson::Value& elem, int val = {});

	int64_t getInt64Val(const rapidjson::Value& elem, int64_t val = {});

	const char* getStringCharVal(const rapidjson::Value& elem, const char* val = "");

	std::string getStringVal(const rapidjson::Value& elem, const std::string_view val = {});

	std::string_view getStringViewVal(const rapidjson::Value& elem, const std::string_view val = "");

	unsigned getUIntVal(const rapidjson::Value& elem, unsigned val = {});

	uint64_t getUInt64Val(const rapidjson::Value& elem, uint64_t val = {});

	// gets a number based on its type
	template<class T>
	T getNumberVal(const rapidjson::Value& elem, T val = {})
	{
		if constexpr (std::is_integral<T>::value == true)
		{
			if constexpr (std::is_signed<T>::value == true)
			{
				// T = signed integer
				if constexpr (sizeof(T) <= 4)
				{
					if (elem.IsInt() == true)
					{
						return (T)elem.GetInt();
					}
					else if (elem.IsFloat() == true)
					{
						return (T)elem.GetFloat();
					}
					return val;
				}
				else
				{
					if (elem.IsInt64() == true)
					{
						return (T)elem.GetInt64();
					}
					else if (elem.IsDouble() == true)
					{
						return (T)elem.GetDouble();
					}
					return val;
				}
			}
			else
			{
				// T = unsigned integer
				if constexpr (sizeof(T) <= 4)
				{
					if (elem.IsUint() == true)
					{
						return (T)elem.GetUint();
					}
					else if (elem.IsFloat() == true)
					{
						auto num = elem.GetFloat();
						if (num >= 0.0)
						{
							return (T)num;
						}
					}
					return val;
				}
				else
				{
					if (elem.IsInt64() == true)
					{
						return (T)elem.GetInt64();
					}
					else if (elem.IsDouble() == true)
					{
						auto num = elem.GetDouble();
						if (num >= 0.0)
						{
							return (T)num;
						}
					}
					return val;
				}
			}
		}
		else if constexpr (std::is_floating_point<T>::value == true)
		{
			// T = floating point
			if constexpr (sizeof(T) <= 4)
			{
				if (elem.IsFloat() == true)
				{
					return (T)elem.GetFloat();
				}
				else if (elem.IsInt() == true)
				{
					return (T)elem.GetInt();
				}
				return val;
			}
			else
			{
				if (elem.IsDouble() == true)
				{
					return (T)elem.GetDouble();
				}
				else if (elem.IsInt64() == true)
				{
					return (T)elem.GetInt64();
				}
				return val;
			}
		}
	}

	// gets a number based on its type
	// if the number type is an integer, returns it unchanged
	// if the number is a float and below 0, val is returned
	template<class T>
	T getUnsignedNumberVal(const rapidjson::Value& elem, T val = {})
	{
		if constexpr (std::is_integral<T>::value == true)
		{
			if constexpr (std::is_signed<T>::value == true)
			{
				// T = signed integer
				if constexpr (sizeof(T) <= 4)
				{
					if (elem.IsInt() == true)
					{
						return (T)elem.GetInt();
					}
					else if (elem.IsFloat() == true)
					{
						return (T)elem.GetFloat();
					}
					return val;
				}
				else
				{
					if (elem.IsInt64() == true)
					{
						return (T)elem.GetInt64();
					}
					else if (elem.IsDouble() == true)
					{
						return (T)elem.GetDouble();
					}
					return val;
				}
			}
			else
			{
				// T = unsigned integer
				if constexpr (sizeof(T) <= 4)
				{
					if (elem.IsUint() == true)
					{
						return (T)elem.GetUint();
					}
					else if (elem.IsFloat() == true)
					{
						auto num = elem.GetFloat();
						if (num >= 0.0)
						{
							return (T)num;
						}
					}
					return val;
				}
				else
				{
					if (elem.IsInt64() == true)
					{
						return (T)elem.GetInt64();
					}
					else if (elem.IsDouble() == true)
					{
						auto num = elem.GetDouble();
						if (num >= 0.0)
						{
							return (T)num;
						}
					}
					return val;
				}
			}
		}
		else if constexpr (std::is_floating_point<T>::value == true)
		{
			// T = floating point
			if constexpr (sizeof(T) <= 4)
			{
				if (elem.IsFloat() == true)
				{
					auto num = elem.GetFloat();
					if (num >= 0.0)
					{
						return (T)num;
					}
				}
				else if (elem.IsUint() == true)
				{
					return (T)elem.GetUint();
				}
				return val;
			}
			else
			{
				if (elem.IsDouble() == true)
				{
					auto num = elem.GetDouble();
					if (num >= 0.0)
					{
						return (T)num;
					}
				}
				else if (elem.IsUint64() == true)
				{
					return (T)elem.GetUint64();
				}
				return val;
			}
		}
	}

	template <class T, class NumType>
	T getVector2NumberVal(const rapidjson::Value & elem, const T & val = {})
	{
		if (elem.IsArray() == true
			&& elem.Size() > 1
			&& elem[0].IsNumber() == true
			&& elem[1].IsNumber() == true)
		{
			return T(
				getNumberVal<NumType>(elem[0]),
				getNumberVal<NumType>(elem[1])
			);
		}
		return val;
	}

	template <class T, class NumType>
	T getVector2UnsignedNumberVal(const rapidjson::Value& elem, const T& val = {})
	{
		if (elem.IsArray() == true
			&& elem.Size() > 1
			&& elem[0].IsNumber() == true
			&& elem[1].IsNumber() == true)
		{
			return T(
				getUnsignedNumberVal<NumType>(elem[0]),
				getUnsignedNumberVal<NumType>(elem[1])
			);
		}
		return val;
	}

	template <class T>
	T getVector2fVal(const rapidjson::Value& elem, const T& val = {})
	{
		return getVector2NumberVal<T, float>(elem, val);
	}

	template <class T>
	T getVector2iVal(const rapidjson::Value& elem, const T& val = {})
	{
		return getVector2NumberVal<T, int32_t>(elem, val);
	}

	template <class T>
	T getVector2uVal(const rapidjson::Value& elem, const T& val = {})
	{
		return getVector2NumberVal<T, uint32_t>(elem, val);
	}

	template <class T, class NumType>
	T getVector3NumberVal(const rapidjson::Value& elem, const T& val = {})
	{
		if (elem.IsArray() == true
			&& elem.Size() > 2
			&& elem[0].IsNumber() == true
			&& elem[1].IsNumber() == true
			&& elem[2].IsNumber() == true)
		{
			return T(
				getNumberVal<NumType>(elem[0]),
				getNumberVal<NumType>(elem[1]),
				getNumberVal<NumType>(elem[2])
			);
		}
		return val;
	}

	template <class T, class NumType>
	T getVector4NumberVal(const rapidjson::Value& elem, const T& val = {})
	{
		if (elem.IsArray() == true
			&& elem.Size() > 3
			&& elem[0].IsNumber() == true
			&& elem[1].IsNumber() == true
			&& elem[2].IsNumber() == true
			&& elem[3].IsNumber() == true)
		{
			return T(
				getNumberVal<NumType>(elem[0]),
				getNumberVal<NumType>(elem[1]),
				getNumberVal<NumType>(elem[2]),
				getNumberVal<NumType>(elem[3])
			);
		}
		return val;
	}

	template <class T, class NumType>
	T getRangeOrValueVal(const rapidjson::Value& elem, const T& val = {})
	{
		if (elem.IsArray() == true
			&& elem.Size() > 1
			&& elem[0].IsNumber() == true
			&& elem[1].IsNumber() == true)
		{
			return T(getNumberVal<NumType>(elem[0]), getNumberVal<NumType>(elem[1]));
		}
		else if (elem.IsNumber() == true)
		{
			auto v = getNumberVal<NumType>(elem);
			return T(v, v);
		}
		return val;
	}

	template <class T, class NumType, NumType minVal>
	T getRangeNVal(const rapidjson::Value& elem, const T& val = {})
	{
		if (elem.IsArray() == true
			&& elem.Size() > 1
			&& elem[0].IsNumber() == true
			&& elem[1].IsNumber() == true)
		{
			return T(getNumberVal<NumType>(elem[0]), getNumberVal<NumType>(elem[1]));
		}
		else if (elem.IsNumber() == true)
		{
			return T((NumType)minVal, getNumberVal<NumType>(elem));
		}
		return val;
	}

	template <class T, class NumType>
	T getRange0Val(const rapidjson::Value& elem, const T& val = {})
	{
		return getRangeNVal<T, NumType, 0>(elem, val);
	}

	template <class T, class NumType>
	T getRange1Val(const rapidjson::Value& elem, const T& val = {})
	{
		return getRangeNVal<T, NumType, 1>(elem, val);
	}

	std::pair<uint32_t, uint32_t> getFramesVal(const rapidjson::Value& elem,
		const std::pair<uint32_t, uint32_t>& val = {});

	sf::Vector2f getPositionVal(const rapidjson::Value& elem,
		const sf::Vector2f& size, const sf::Vector2u& refSize);

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

	InputEventType getInputEventTypeVal(const rapidjson::Value& elem,
		InputEventType val = InputEventType::None);

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
			if (elem.GetString() == std::string_view("min"))
			{
				return std::numeric_limits<T>::min();
			}
			else if (elem.GetString() == std::string_view("max"))
			{
				return std::numeric_limits<T>::max();
			}
		}
		return val;
	}

	Number32 getMinMaxNumber32Val(const rapidjson::Value& elem);

	const rapidjson::Value& getQueryVal(const rapidjson::Value* elem,
		const rapidjson::Value& query);

	const rapidjson::Value& getQueryVal(const rapidjson::Value& elem,
		const rapidjson::Value& query);

	ReplaceVars getReplaceVarsVal(const rapidjson::Value& elem,
		ReplaceVars val = ReplaceVars::None);

	bool getVariableVal(const rapidjson::Value& elem, Variable& var);

	Variable getVariableVal(const rapidjson::Value& elem);

	std::vector<std::pair<std::string, Variable>> getVariables(const rapidjson::Value& elem);

	UnorderedStringMap<Variable> getVariablesMap(const rapidjson::Value& elem);

	VarOrPredicate getVarOrPredicateVal(Game& game, const rapidjson::Value& elem);
}
