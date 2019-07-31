#include "ParseUtilsVal.h"
#include "ParseUtilsKey.h"
#include "Parser/ParsePredicate.h"
#include "ParseUtils.h"
#include <cctype>
#include "FileUtils.h"
#include "GameUtils.h"
#include "Json/JsonUtils.h"
#include "SFML/SFMLUtils.h"
#include "Utils/Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace Utils;

	Anchor getAnchorVal(const rapidjson::Value& elem, Anchor val)
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
				ret |= GameUtils::getAnchor(getStringVal(arrElem).c_str(), val);
			}
			return ret;
		}
		return val;
	}

	BlendMode getBlendModeVal(const rapidjson::Value& elem, BlendMode val)
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

	std::string getStringVal(const Value& elem, const std::string& val)
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
		return val;
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
		if (elem.IsArray() == true)
		{
			return getVector2uVal(elem, val);
		}
		else if (elem.IsUint() == true)
		{
			return std::make_pair(1u, elem.GetUint());
		}
		return val;
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

	size_t getInventoryItemIndexVal(const Value& elem, PlayerInventory inv)
	{
		size_t itemIdx = 0;
		if (elem.IsUint() == true)
		{
			itemIdx = elem.GetUint();
		}
		else if (elem.IsString() == true)
		{
			if (inv == PlayerInventory::Body)
			{
				itemIdx = (size_t)GameUtils::getPlayerItemMount(elem.GetStringView());
			}
		}
		return itemIdx;
	}

	InventoryPosition getInventoryPositionVal(const Value& elem, InventoryPosition val)
	{
		if (elem.IsString() == true)
		{
			return GameUtils::getInventoryPosition(elem.GetStringView(), val);
		}
		return val;
	}

	LightSource getLightSourceVal(const Value& elem, LightSource val)
	{
		if (elem.IsUint() == true)
		{
			val.maxLight = elem.GetUint();
		}
		else if (elem.IsArray() == true)
		{
			switch (elem.Size())
			{
			case 4:
			{
				switch (str2int16(getStringViewVal(elem[3])))
				{
				case str2int16("linear"):
					val.easing = LightEasing::Linear;
					break;
				case str2int16("sine"):
					val.easing = LightEasing::Sine;
					break;
				case str2int16("quad"):
					val.easing = LightEasing::Quad;
					break;
				case str2int16("cubic"):
					val.easing = LightEasing::Cubic;
					break;
				case str2int16("quart"):
					val.easing = LightEasing::Quart;
					break;
				case str2int16("quint"):
					val.easing = LightEasing::Quint;
					break;
				case str2int16("expo"):
					val.easing = LightEasing::Expo;
					break;
				case str2int16("circ"):
					val.easing = LightEasing::Circ;
					break;
				default:
					break;
				}
			}
			case 3:
				val.radius = getUIntVal(elem[2], val.radius);
			case 2:
			{
				val.maxLight = getUIntVal(elem[1], val.maxLight);
				val.minLight = getUIntVal(elem[0], val.minLight);
				break;
			}
			case 1:
				val.maxLight = getUIntVal(elem[0], val.maxLight);
				break;
			default:
				break;
			}
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

	PairUInt8 getItemXYVal(const Value& elem, const PairUInt8& val)
	{
		if (elem.IsArray() == true
			&& elem.Size() > 1
			&& elem[0].IsUint() == true
			&& elem[1].IsUint() == true)
		{
			auto x = elem[0].GetUint();
			auto y = elem[1].GetUint();

			if (x <= 0xFF && y <= 0xFF)
			{
				return PairUInt8((uint8_t)x, (uint8_t)y);
			}
		}
		return val;
	}

	ItemCoordInventory getItemCoordInventoryVal(const Value& elem)
	{
		std::string_view playerId("");
		if (elem.HasMember("player") == true &&
			elem["player"].IsString() == true)
		{
			playerId = elem["player"].GetStringView();
		}
		PlayerInventory inv = PlayerInventory::Body;
		if (elem.HasMember("inventory") == true)
		{
			inv = getPlayerInventoryVal(elem["inventory"]);
			if (elem.HasMember("item") == true)
			{
				if (elem["item"].IsArray() == true)
				{
					auto itemPos = getItemXYVal(elem["item"]);
					return ItemCoordInventory(playerId, (size_t)inv, itemPos);
				}
				else
				{
					size_t itemIdx = getInventoryItemIndexVal(elem["item"], inv);
					return ItemCoordInventory(playerId, (size_t)inv, itemIdx);
				}
			}
			return ItemCoordInventory(playerId, (size_t)inv, 0);
		}
		return ItemCoordInventory(playerId);
	}

	AnimationSpeed getPlayerAnimationSpeedVal(const rapidjson::Value& elem)
	{
		AnimationSpeed speed;
		speed.animation = GameUtils::getTime(getIntKey(elem, "animation", 5));
		speed.walk = GameUtils::getTime(getIntKey(elem, "walk", 25));
		return speed;
	}

	ItemLocation getItemLocationVal(const Value& elem)
	{
		if (elem.HasMember("mapPosition") == true)
		{
			const auto& mapElem = elem["mapPosition"];
			if (mapElem.IsArray() == true
				&& mapElem.Size() > 1
				&& mapElem[0].IsNumber() == true
				&& mapElem[1].IsNumber() == true)
			{
				return {
					PairFloat(getNumberVal<float>(mapElem[0]),
					getNumberVal<float>(mapElem[1]))
				};
			}
		}
		return{ getItemCoordInventoryVal(elem) };
	}

	PlayerInventory getPlayerInventoryVal(const Value& elem, PlayerInventory val)
	{
		if (elem.IsUint() == true)
		{
			auto idx = elem.GetUint();
			if (idx < (size_t)PlayerInventory::Size)
			{
				return (PlayerInventory)idx;
			}
		}
		else if (elem.IsString() == true)
		{
			return GameUtils::getPlayerInventory(elem.GetStringView(), val);
		}
		return val;
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

	Variable getVariableVal(const Value& elem)
	{
		Variable var;
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
		return var;
	}

	VarOrPredicate getVarOrPredicateVal(Game& game, const rapidjson::Value& elem)
	{
		if (elem.IsObject() == true)
		{
			return VarOrPredicate(parsePredicateObj(game, elem));
		}
		return VarOrPredicate(getVariableVal(elem));
	}
}
