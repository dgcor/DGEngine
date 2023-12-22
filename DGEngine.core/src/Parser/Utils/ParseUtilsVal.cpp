#include "ParseUtilsVal.h"
#include "Game/Utils/GameUtils.h"
#include "Json/JsonUtils.h"
#include "Parser/ParsePredicate.h"
#include "ParseUtils.h"
#include "SFML/SFMLUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;

	Anchor getAnchorVal(const Value& elem, Anchor val)
	{
		if (elem.IsString() == true)
		{
			return GameUtils::getAnchor(elem.GetStringView(), val);
		}
		else if (elem.IsArray() == true)
		{
			Anchor ret = Anchor::None;
			for (const auto& arrElem : elem)
			{
				ret |= GameUtils::getAnchor(getStringViewVal(arrElem), val);
			}
			return ret;
		}
		return val;
	}

	AnimationType getAnimationTypeVal(const Value& elem, AnimationType val)
	{
		if (elem.IsString() == true)
		{
			return GameUtils::getAnimationType(elem.GetStringView(), val);
		}
		return val;
	}

	BindingFlags getBindingFlagsVal(const Value& elem, BindingFlags val)
	{
		if (elem.IsString() == true)
		{
			return GameUtils::getBindingFlags(elem.GetStringView(), val);
		}
		else if (elem.IsArray() == true)
		{
			BindingFlags ret = BindingFlags::OnChange;
			for (const auto& arrElem : elem)
			{
				ret |= GameUtils::getBindingFlags(getStringViewVal(arrElem), val);
			}
			return ret;
		}
		return val;
	}

	BlendMode getBlendModeVal(const Value& elem, BlendMode val)
	{
		if (elem.IsString() == true)
		{
			return GameUtils::getBlendMode(elem.GetStringView(), val);
		}
		return val;
	}

	Direction getDirectionVal(const Value& elem, Direction val)
	{
		if (elem.IsString() == true)
		{
			return GameUtils::getDirection(elem.GetStringView(), val);
		}
		return val;
	}

	bool getBoolVal(const Value& elem, bool val)
	{
		if (elem.IsBool() == true)
		{
			return elem.GetBool();
		}
		return val;
	}

	double getDoubleVal(const Value& elem, double val)
	{
		if (elem.IsDouble() == true)
		{
			return elem.GetDouble();
		}
		return val;
	}

	float getFloatVal(const Value& elem, float val)
	{
		if (elem.IsDouble() == true)
		{
			auto d = elem.GetDouble();
			auto f = (float)d;
			if ((int64_t)d == (int64_t)f)
			{
				return f;
			}
		}
		return val;
	}

	int getIntVal(const Value& elem, int val)
	{
		if (elem.IsInt() == true)
		{
			return elem.GetInt();
		}
		return val;
	}

	int64_t getInt64Val(const Value& elem, int64_t val)
	{
		if (elem.IsInt64() == true)
		{
			return elem.GetInt64();
		}
		return val;
	}

	const char* getStringCharVal(const Value& elem, const char* val)
	{
		if (elem.IsString() == true)
		{
			return elem.GetString();
		}
		return val;
	}

	std::string getStringVal(const Value& elem, const std::string_view val)
	{
		if (elem.IsString() == true)
		{
			return elem.GetStringStr();
		}
		else if (elem.IsInt64() == true)
		{
			return Utils::toString(elem.GetInt64());
		}
		else if (elem.IsDouble() == true)
		{
			return Utils::toString(elem.GetDouble());
		}
		return std::string(val);
	}

	std::string_view getStringViewVal(const Value& elem, const std::string_view val)
	{
		if (elem.IsString() == true)
		{
			return elem.GetStringView();
		}
		return val;
	}

	unsigned getUIntVal(const Value& elem, unsigned val)
	{
		if (elem.IsUint() == true)
		{
			return elem.GetUint();
		}
		return val;
	}

	uint64_t getUInt64Val(const Value& elem, uint64_t val)
	{
		if (elem.IsUint64() == true)
		{
			return elem.GetUint64();
		}
		return val;
	}

	sf::Vector2f getPositionVal(const Value& elem, const sf::Vector2f& size, const sf::Vector2u& refSize)
	{
		float x = 0.f;
		float y = 0.f;

		if (elem.IsArray() == true
			&& elem.Size() > 1)
		{
			if (elem[0].IsNumber() == true)
			{
				x = std::round(elem[0].GetFloat());
			}
			else if (elem[0].IsString() == true)
			{
				switch (GameUtils::getHorizontalAlignment(elem[0].GetStringView()))
				{
				case HorizontalAlign::Left:
				default:
					break;
				case HorizontalAlign::Center:
					x = std::round((((float)refSize.x) / 2.f) - (size.x / 2.f));
					break;
				case HorizontalAlign::Right:
					x = ((float)refSize.x) - size.x;
					break;
				}
			}
			if (elem[1].IsNumber() == true)
			{
				y = std::round(elem[1].GetFloat());
			}
			else if (elem[1].IsString() == true)
			{
				switch (GameUtils::getVerticalAlignment(elem[1].GetStringView()))
				{
				case VerticalAlign::Top:
				default:
					break;
				case VerticalAlign::Center:
					y = std::round((((float)refSize.y) / 2.f) - (size.y / 2.f));
					break;
				case VerticalAlign::Bottom:
					y = ((float)refSize.y) - size.y;
					break;
				}
			}
		}
		return { x, y };
	}

	sf::Vector2f getSizeVal(const Value& elem, const sf::Vector2f& val)
	{
		auto size = getVector2fVal(elem, val);
		size.x = std::round(size.x);
		size.y = std::round(size.y);
		return size;
	}

	sf::IntRect getIntRectVal(const Value& elem, const sf::IntRect& val)
	{
		if (elem.IsArray() == true && elem.Size() >= 4)
		{
			return sf::IntRect(
				getNumberVal<int>(elem[0]),
				getNumberVal<int>(elem[1]),
				getNumberVal<int>(elem[2]),
				getNumberVal<int>(elem[3])
			);
		}
		else if (elem.IsArray() == true && elem.Size() >= 2)
		{
			return sf::IntRect(
				0,
				0,
				getNumberVal<int>(elem[0]),
				getNumberVal<int>(elem[1])
			);
		}
		return val;
	}

	sf::FloatRect getFloatRectVal(const Value& elem, const sf::FloatRect& val)
	{
		if (elem.IsArray() == true && elem.Size() >= 4)
		{
			return sf::FloatRect(
				getNumberVal<float>(elem[0]),
				getNumberVal<float>(elem[1]),
				getNumberVal<float>(elem[2]),
				getNumberVal<float>(elem[3])
			);
		}
		else if (elem.IsArray() == true && elem.Size() >= 2)
		{
			return sf::FloatRect(
				0.f,
				0.f,
				getNumberVal<float>(elem[0]),
				getNumberVal<float>(elem[1])
			);
		}
		return val;
	}

	sf::Color getColorVal(const Value& elem, const sf::Color& val)
	{
		if (elem.IsString() == true)
		{
			auto color = SFMLUtils::stringToColor(elem.GetStringView());
			if (color)
			{
				return *color;
			}
		}
		else if (elem.IsUint() == true)
		{
			return SFMLUtils::rgbaToColor(elem.GetUint());
		}
		else if (elem.IsArray() == true)
		{
			if (elem.Size() == 3)
			{
				return sf::Color(
					getNumberVal<uint8_t>(elem[0]),
					getNumberVal<uint8_t>(elem[1]),
					getNumberVal<uint8_t>(elem[2])
				);
			}
			else if (elem.Size() > 3)
			{
				return sf::Color(
					getNumberVal<uint8_t>(elem[0]),
					getNumberVal<uint8_t>(elem[1]),
					getNumberVal<uint8_t>(elem[2]),
					getNumberVal<uint8_t>(elem[3])
				);
			}
		}
		return val;
	}

	sf::Keyboard::Key getKeyCodeVal(const Value& elem, sf::Keyboard::Key val)
	{
		if (elem.IsInt() == true)
		{
			return GameUtils::getKeyCode(elem.GetInt(), val);
		}
		else if (elem.IsString() == true)
		{
			return GameUtils::getKeyCode(elem.GetStringView(), val);
		}
		return val;
	}

	sf::Keyboard::Scancode getScanCodeVal(const Value& elem, sf::Keyboard::Scancode val)
	{
		if (elem.IsInt() == true)
		{
			return GameUtils::getScanCode(elem.GetInt(), val);
		}
		else if (elem.IsString() == true)
		{
			return GameUtils::getScanCode(elem.GetStringView(), val);
		}
		return val;
	}

	sf::Time getTimeVal(const Value& elem, const sf::Time& val)
	{
		if (elem.IsInt() == true)
		{
			return sf::milliseconds(elem.GetInt());
		}
		else if (elem.IsFloat() == true)
		{
			return sf::seconds(elem.GetFloat());
		}
		else if (elem.IsString() == true)
		{
			int32_t h = 0, m = 0, s = 0, ms = 0;
			auto sv = Utils::splitStringIn2(elem.GetStringView(), '.');
			bool negativeTime = sv.first.starts_with('-');
			if (sv.second.empty() == false)
			{
				ms = std::abs(Utils::strtoi(sv.second));
			}
			sv = Utils::splitStringIn2(sv.first, ':');
			if (sv.second.empty() == false)
			{
				auto sv2 = Utils::splitStringIn2(sv.second, ':');
				if (sv2.second.empty() == false)
				{
					h = std::abs(Utils::strtoi(sv.first));
					m = std::abs(Utils::strtoi(sv2.first));
					s = std::abs(Utils::strtoi(sv2.second));
				}
				else
				{
					m = std::abs(Utils::strtoi(sv.first));
					s = std::abs(Utils::strtoi(sv2.first));
				}
			}
			else
			{
				auto seconds = Utils::strToNumberOpt<int32_t>(sv.first);
				if (seconds.has_value() == false)
				{
					return val;
				}
				s = std::abs(*seconds);
			}
			int32_t totalms = (((h * 3600) + (m * 60) + s) * 1000) + (ms * 100);
			if (negativeTime == true)
			{
				totalms = -totalms;
			}
			return sf::milliseconds(totalms);
		}
		return val;
	}

	sf::Time getTimeUVal(const Value& elem, const sf::Time& val)
	{
		auto time = getTimeVal(elem, val);
		if (time.asMicroseconds() < 0)
		{
			return val;
		}
		return time;
	}

	std::vector<std::string> getStringVectorVal(const Value& elem)
	{
		std::vector<std::string> vec;
		if (elem.IsArray() == true)
		{
			for (const auto& val : elem)
			{
				vec.push_back(JsonUtils::toString(val));
			}
		}
		else if (elem.IsString() == true)
		{
			vec.push_back(elem.GetStringStr());
		}
		return vec;
	}

	IgnoreResource getIgnoreResourceVal(const Value& elem, IgnoreResource val)
	{
		if (elem.IsBool() == true)
		{
			if (elem.GetBool() == true)
			{
				return IgnoreResource::Draw | IgnoreResource::Update;
			}
			else
			{
				return IgnoreResource::None;
			}
		}
		else if (elem.IsString() == true)
		{
			return GameUtils::getIgnoreResource(elem.GetStringView(), val);
		}
		else if (elem.IsArray() == true)
		{
			IgnoreResource ret = IgnoreResource::None;
			for (const auto& arrElem : elem)
			{
				ret |= GameUtils::getIgnoreResource(arrElem.GetStringView(), val);
			}
			return ret;
		}
		return val;
	}

	InputEventType getInputEventTypeVal(const Value& elem, InputEventType val)
	{
		if (elem.IsBool() == true)
		{
			if (elem.GetBool() == true)
			{
				return InputEventType::All;
			}
			else
			{
				return InputEventType::None;
			}
		}
		else if (elem.IsString() == true)
		{
			return GameUtils::getInputEventType(elem.GetStringView(), val);
		}
		else if (elem.IsArray() == true)
		{
			InputEventType ret = InputEventType::None;
			for (const auto& arrElem : elem)
			{
				ret |= GameUtils::getInputEventType(arrElem.GetStringView(), val);
			}
			return ret;
		}
		return val;
	}

	sf::PrimitiveType getPrimitiveTypeVal(const Value& elem, sf::PrimitiveType val)
	{
		if (elem.IsString() == true)
		{
			return GameUtils::getPrimitiveType(elem.GetStringView(), val);
		}
		return val;
	}

	Number32 getMinMaxNumber32Val(const Value& elem)
	{
		return getMinMaxIntVal<Number32>(elem);
	}

	const Value& getQueryVal(const Value* elem, const Value& query)
	{
		if (elem != nullptr)
		{
			return JsonUtils::query(*elem, query);
		}
		return query;
	}

	const Value& getQueryVal(const Value& elem, const Value& query)
	{
		return JsonUtils::query(elem, query);
	}

	ReplaceVars getReplaceVarsVal(const Value& elem, ReplaceVars val)
	{
		if (elem.IsBool() == true)
		{
			if (elem.GetBool() == false)
			{
				return ReplaceVars::None;
			}
			else
			{
				return ReplaceVars::String;
			}
		}
		else if (elem.IsString() == true)
		{
			return getReplaceVars(elem.GetStringView(), val);
		}
		return val;
	}

	bool getVariableVal(const Value& elem, Variable& var)
	{
		if (elem.IsString() == true)
		{
			var.emplace<std::string>(elem.GetStringStr());
		}
		else if (elem.IsInt64() == true)
		{
			var.emplace<int64_t>(elem.GetInt64());
		}
		else if (elem.IsDouble() == true)
		{
			var.emplace<double>(elem.GetDouble());
		}
		else if (elem.IsBool() == true)
		{
			var.emplace<bool>(elem.GetBool());
		}
		else if (elem.IsArray() == true)
		{
			if (elem.Size() == 2)
			{
				if (elem[0].IsInt64() == true &&
					elem[1].IsInt64() == true)
				{
					var.emplace<Int64Tuple2>(std::make_tuple(
						elem[0].GetInt64(),
						elem[1].GetInt64()
					));
				}
				else if (elem[0].IsDouble() == true &&
					elem[1].IsDouble() == true)
				{
					var.emplace<DoubleTuple2>(std::make_tuple(
						elem[0].GetDouble(),
						elem[1].GetDouble()
					));
				}
				else
				{
					return false;
				}
			}
			else if (elem.Size() == 3)
			{
				if (elem[0].IsInt64() == true &&
					elem[1].IsInt64() == true &&
					elem[2].IsInt64() == true)
				{
					var.emplace<Int64Tuple3>(std::make_tuple(
						elem[0].GetInt64(),
						elem[1].GetInt64(),
						elem[2].GetInt64()
					));
				}
				else if (elem[0].IsDouble() == true &&
					elem[1].IsDouble() == true &&
					elem[2].IsDouble() == true)
				{
					var.emplace<DoubleTuple3>(std::make_tuple(
						elem[0].GetDouble(),
						elem[1].GetDouble(),
						elem[2].GetDouble()
					));
				}
				else
				{
					return false;
				}
			}
			else if (elem.Size() > 3)
			{
				if (elem[0].IsInt64() == true &&
					elem[1].IsInt64() == true &&
					elem[2].IsInt64() == true &&
					elem[3].IsInt64() == true)
				{
					var.emplace<Int64Tuple4>(std::make_tuple(
						elem[0].GetInt64(),
						elem[1].GetInt64(),
						elem[2].GetInt64(),
						elem[3].GetInt64()
					));
				}
				else if (elem[0].IsDouble() == true &&
					elem[1].IsDouble() == true &&
					elem[2].IsDouble() == true &&
					elem[3].IsDouble() == true)
				{
					var.emplace<DoubleTuple4>(std::make_tuple(
						elem[0].GetDouble(),
						elem[1].GetDouble(),
						elem[2].GetDouble(),
						elem[3].GetDouble()
					));
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	Variable getVariableVal(const Value& elem)
	{
		Variable var;
		getVariableVal(elem, var);
		return var;
	}

	std::vector<std::pair<std::string, Variable>> getVariables(const Value& elem)
	{
		std::vector<std::pair<std::string, Variable>> vars;
		if (elem.IsObject() == true)
		{
			for (const auto& it : std::ranges::subrange(elem.MemberBegin(), elem.MemberEnd()))
			{
				auto key = it.name.GetStringView();
				if (key.empty() == false)
				{
					Variable var;
					if (getVariableVal(it.value, var) == true)
					{
						vars.push_back({ std::string(key), var });
					}
				}
			}
		}
		return vars;
	}

	UnorderedStringMap<Variable> getVariablesMap(const Value& elem)
	{
		UnorderedStringMap<Variable> vars;
		if (elem.IsObject() == true)
		{
			for (const auto& it : std::ranges::subrange(elem.MemberBegin(), elem.MemberEnd()))
			{
				auto key = it.name.GetStringView();
				if (key.empty() == false)
				{
					Variable var;
					if (getVariableVal(it.value, var) == true)
					{
						vars.insert(std::make_pair(key, var));
					}
				}
			}
		}
		return vars;
	}

	VarOrPredicate getVarOrPredicateVal(Game& game, const Value& elem)
	{
		if (elem.IsObject() == true)
		{
			return VarOrPredicate(getPredicateObj(game, elem));
		}
		return VarOrPredicate(getVariableVal(elem));
	}
}
