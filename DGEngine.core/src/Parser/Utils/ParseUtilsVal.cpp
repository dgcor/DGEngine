#include "ParseUtilsVal.h"
#include "GameUtils.h"
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
		if (elem.IsFloat() == true)
		{
			return elem.GetFloat();
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

	std::pair<uint32_t, uint32_t> getFramesVal(const Value& elem,
		const std::pair<uint32_t, uint32_t>& val)
	{
		return getRange1Val<std::pair<uint32_t, uint32_t>, uint32_t>(elem, val);
	}

	sf::Vector2f getPositionVal(const Value& elem,
		const sf::Vector2f& size, const sf::Vector2u& refSize)
	{
		float x = 0.f;
		float y = 0.f;

		if (elem.IsArray() == true
			&& elem.Size() > 1)
		{
			if (elem[0].IsNumber() == true)
			{
				x = elem[0].GetFloat();
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
				y = elem[1].GetFloat();
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
			return SFMLUtils::stringToColor(elem.GetStringView());
		}
		else if (elem.IsUint() == true)
		{
			return SFMLUtils::rgbaToColor(elem.GetUint());
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

	sf::Time getTimeVal(const Value& elem, const sf::Time& val)
	{
		if (elem.IsUint() == true)
		{
			return sf::milliseconds(elem.GetUint());
		}
		else if (elem.IsFloat() == true)
		{
			return sf::seconds(elem.GetFloat());
		}
		else if (elem.IsString() == true)
		{
			int h = 0, m = 0, s = 0, ms = 0;
			auto sv = Utils::splitStringIn2(elem.GetStringView(), '.');
			if (sv.second.empty() == false)
			{
				ms = Utils::strtoi(sv.second);
			}
			sv = Utils::splitStringIn2(sv.first, ':');
			if (sv.second.empty() == false)
			{
				auto sv2 = Utils::splitStringIn2(sv.second, ':');
				if (sv2.second.empty() == false)
				{
					h = Utils::strtoi(sv.first);
					m = Utils::strtoi(sv2.first);
					s = Utils::strtoi(sv2.second);
				}
				else
				{
					m = Utils::strtoi(sv.first);
					s = Utils::strtoi(sv2.first);
				}
			}
			else
			{
				s = Utils::strtoi(sv.first);
			}
			auto totalms = (((h * 3600) + (m * 60) + s) * 1000) + (ms * 100);
			return sf::milliseconds(totalms);
		}
		return val;
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

	Number32 getMinMaxNumber32Val(const Value& elem)
	{
		Number32 num;
		if (elem.IsInt() == true)
		{
			num.setInt32(elem.GetInt());
		}
		if (elem.IsUint() == true)
		{
			num.setUInt32(elem.GetUint());
		}
		if (elem.IsFloat() == true)
		{
			num.setFloat(elem.GetFloat());
		}
		else if (elem.IsBool() == true)
		{
			num.setInt32((int32_t)elem.GetBool());
		}
		else if (elem.IsString() == true)
		{
			if (elem.GetStringView() == "min")
			{
				num.setInt32(std::numeric_limits<int32_t>::min());
			}
			else if (elem.GetStringView() == "max")
			{
				num.setInt32(std::numeric_limits<int32_t>::max());
			}
		}
		return num;
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
		for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
		{
			auto key = it->name.GetStringView();
			if (key.empty() == false)
			{
				Variable var;
				if (getVariableVal(it->value, var) == true)
				{
					vars.push_back(std::make_pair(std::string(key), var));
				}
			}
		}
		return vars;
	}

	UnorderedStringMap<Variable> getVariablesMap(const Value& elem)
	{
		UnorderedStringMap<Variable> vars;
		for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
		{
			auto key = it->name.GetStringView();
			if (key.empty() == false)
			{
				Variable var;
				if (getVariableVal(it->value, var) == true)
				{
					vars.insert(std::make_pair(key, var));
				}
			}
		}
		return vars;
	}

	VarOrPredicate getVarOrPredicateVal(Game& game, const rapidjson::Value& elem)
	{
		if (elem.IsObject() == true)
		{
			return VarOrPredicate(getPredicateObj(game, elem));
		}
		return VarOrPredicate(getVariableVal(elem));
	}
}
