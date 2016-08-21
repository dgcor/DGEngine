#include "ParseUtils.h"
#include "SFMLUtils.h"
#include "Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace Utils;

	bool isValidArray(const rapidjson::Value& elem, const char* key)
	{
		return (elem.HasMember(key) && elem[key].IsArray() && elem[key].Size() > 0);
	}

	bool isValidString(const rapidjson::Value& elem, const char* key)
	{
		return (elem.HasMember(key) && elem[key].IsString() && elem[key].GetStringLength() > 0);
	}

	Anchor getAnchorVal(const char* anchor, Anchor val)
	{
		switch (str2int(toLower(anchor).c_str()))
		{
		case str2int("none"):
			return Anchor::None;
		case str2int("top"):
			return Anchor::Top;
		case str2int("bottom"):
			return Anchor::Bottom;
		case str2int("left"):
			return Anchor::Left;
		case str2int("right"):
			return Anchor::Right;
		case str2int("all"):
			return Anchor::Top | Anchor::Bottom | Anchor::Left | Anchor::Right;
		default:
			return val;
		}
	}

	Anchor getAnchorKey(const rapidjson::Value& elem, const char* key, Anchor val)
	{
		if (elem.HasMember(key))
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString())
			{
				return getAnchorVal(keyElem.GetString(), val);
			}
			else if (keyElem.IsArray())
			{
				Anchor ret = Anchor::None;
				for (const auto& arrElem : keyElem)
				{
					ret |= getAnchorVal(getStringVal(arrElem).c_str(), val);
				}
				return ret;
			}
		}
		return val;
	}

	bool getBoolVal(const rapidjson::Value& elem, bool val)
	{
		if (elem.IsBool()) {
			return elem.GetBool();
		}
		else {
			return val;
		}
	}

	bool getBoolKey(const rapidjson::Value& elem, const char* key, bool val)
	{
		if (elem.HasMember(key) && elem[key].IsBool()) {
			return elem[key].GetBool();
		}
		else {
			return val;
		}
	}

	bool getBoolIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, bool val)
	{
		if (idx < elem.Size() && elem[idx].IsBool()) {
			return elem[idx].GetBool();
		}
		else {
			return val;
		}
	}

	double getDoubleVal(const rapidjson::Value& elem, double val)
	{
		if (elem.IsDouble()) {
			return elem.GetDouble();
		}
		else {
			return val;
		}
	}

	double getDoubleKey(const rapidjson::Value& elem, const char* key, double val)
	{
		if (elem.HasMember(key) && elem[key].IsDouble()) {
			return elem[key].GetDouble();
		}
		else {
			return val;
		}
	}

	double getDoubleIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, double val)
	{
		if (idx < elem.Size() && elem[idx].IsDouble()) {
			return elem[idx].GetDouble();
		}
		else {
			return val;
		}
	}

	int getIntVal(const rapidjson::Value& elem, int val)
	{
		if (elem.IsInt()) {
			return elem.GetInt();
		}
		else {
			return val;
		}
	}

	int getIntKey(const rapidjson::Value& elem, const char* key, int val)
	{
		if (elem.HasMember(key) && elem[key].IsInt()) {
			return elem[key].GetInt();
		}
		else {
			return val;
		}
	}

	int getIntIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, int val)
	{
		if (idx < elem.Size() && elem[idx].IsInt()) {
			return elem[idx].GetInt();
		}
		else {
			return val;
		}
	}

	int64_t getInt64Val(const rapidjson::Value& elem, int64_t val)
	{
		if (elem.IsInt64()) {
			return elem.GetInt64();
		}
		else {
			return val;
		}
	}

	int64_t getInt64Key(const rapidjson::Value& elem, const char* key, int64_t val)
	{
		if (elem.HasMember(key) && elem[key].IsInt64()) {
			return elem[key].GetInt64();
		}
		else {
			return val;
		}
	}

	int64_t getInt64Idx(const rapidjson::Value& elem, rapidjson::SizeType idx, int64_t val)
	{
		if (idx < elem.Size() && elem[idx].IsInt64()) {
			return elem[idx].GetInt64();
		}
		else {
			return val;
		}
	}

	const char* getStringCharVal(const rapidjson::Value& elem, const char* val)
	{
		if (elem.IsString()) {
			return elem.GetString();
		}
		else {
			return val;
		}
	}

	std::string getStringVal(const rapidjson::Value& elem, const std::string& val)
	{
		return getStringCharVal(elem, val.c_str());
	}

	const char* getStringCharKey(const rapidjson::Value& elem,
		const char* key, const char* val)
	{
		if (elem.HasMember(key) && elem[key].IsString()) {
			return elem[key].GetString();
		}
		else {
			return val;
		}
	}

	std::string getStringKey(const rapidjson::Value& elem,
		const char* key, const std::string& val)
	{
		return getStringCharKey(elem, key, val.c_str());
	}

	const char* getStringCharIdx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, const char* val)
	{
		if (idx < elem.Size() && elem[idx].IsString()) {
			return elem[idx].GetString();
		}
		else {
			return val;
		}
	}

	std::string getStringIdx(const rapidjson::Value& elem,
		rapidjson::SizeType idx, const std::string& val)
	{
		return getStringCharIdx(elem, idx, val.c_str());
	}

	unsigned getUIntVal(const rapidjson::Value& elem, unsigned val)
	{
		if (elem.IsUint()) {
			return elem.GetUint();
		}
		else {
			return val;
		}
	}

	unsigned getUIntKey(const rapidjson::Value& elem, const char* key, unsigned val)
	{
		if (elem.HasMember(key) && elem[key].IsUint()) {
			return elem[key].GetUint();
		}
		else {
			return val;
		}
	}

	unsigned getUIntIdx(const rapidjson::Value& elem, rapidjson::SizeType idx, unsigned val)
	{
		if (idx < elem.Size() && elem[idx].IsUint()) {
			return elem[idx].GetUint();
		}
		else {
			return val;
		}
	}

	uint64_t getUInt64Val(const rapidjson::Value& elem, uint64_t val)
	{
		if (elem.IsUint64()) {
			return elem.GetUint64();
		}
		else {
			return val;
		}
	}

	uint64_t getUInt64Key(const rapidjson::Value& elem, const char* key, uint64_t val)
	{
		if (elem.HasMember(key) && elem[key].IsUint64()) {
			return elem[key].GetUint64();
		}
		else {
			return val;
		}
	}

	uint64_t getUInt64Idx(const rapidjson::Value& elem, rapidjson::SizeType idx, uint64_t val)
	{
		if (idx < elem.Size() && elem[idx].IsUint64()) {
			return elem[idx].GetUint64();
		}
		else {
			return val;
		}
	}

	sf::IntRect getIntRectKey(const rapidjson::Value& elem,
		const char* key, const sf::IntRect& val)
	{
		if (elem.HasMember(key))
		{
			return getIntRectVal(elem[key], val);
		}
		return val;
	}

	sf::IntRect getIntRectVal(const rapidjson::Value& elem, const sf::IntRect& val)
	{
		if (elem.IsArray() && elem.Size() >= 4)
		{
			return sf::IntRect(getInt64Val(elem[0]), getInt64Val(elem[1]),
				getInt64Val(elem[2]), getInt64Val(elem[3]));
		}
		else if (elem.IsArray() && elem.Size() >= 2)
		{
			return sf::IntRect(0, 0, getInt64Val(elem[0]), getInt64Val(elem[1]));
		}
		return val;
	}

	sf::FloatRect getFloatRectKey(const rapidjson::Value& elem,
		const char* key, const sf::FloatRect& val)
	{
		if (elem.HasMember(key))
		{
			return getFloatRectVal(elem[key], val);
		}
		return val;
	}

	sf::FloatRect getFloatRectVal(const rapidjson::Value& elem, const sf::FloatRect& val)
	{
		if (elem.IsArray() && elem.Size() >= 4)
		{
			return sf::FloatRect(getDoubleVal(elem[0]), getDoubleVal(elem[1]),
				getDoubleVal(elem[2]), getDoubleVal(elem[3]));
		}
		else if (elem.IsArray() && elem.Size() >= 2)
		{
			return sf::FloatRect(0, 0, getDoubleVal(elem[0]), getDoubleVal(elem[1]));
		}
		return val;
	}

	sf::Color getColorKey(const rapidjson::Value& elem, const char* key, const sf::Color& val)
	{
		if (elem.HasMember(key) && elem[key].IsString())
		{
			std::string str = elem[key].GetString();

			if (str.length() >= 10)
			{
				return sf::rgbaToColor(std::stoul(elem[key].GetString(), 0, 16));
			}
			else
			{
				return sf::rgbToColor(std::stoul(elem[key].GetString(), 0, 16));
			}
		}
		else
		{
			return val;
		}
	}

	std::vector<std::string> getStringVectorKey(const rapidjson::Value& elem, const char* key)
	{
		std::vector<std::string> vec;
		if (elem.HasMember(key) == true)
		{
			const auto& elemVal = elem[key];
			if (elemVal.IsArray() == true)
			{
				for (const auto& val : elemVal)
				{
					vec.push_back(val.GetString());
				}
			}
			else if (elemVal.IsString() == true)
			{
				vec.push_back(elemVal.GetString());
			}
		}
		return vec;
	}

	sf::Keyboard::Key getKeyCodeKey(const rapidjson::Value& elem, sf::Keyboard::Key val)
	{
		if (elem.IsInt())
		{
			auto num = elem.GetInt();
			if (num >= sf::Keyboard::Unknown && num < sf::Keyboard::KeyCount)
			{
				return static_cast<sf::Keyboard::Key>(num);
			}
		}
		else if (elem.IsString())
		{
			std::string str = elem.GetString();
			if (str.size() > 0)
			{
				if (str.size() == 1)
				{
					char ch = str[0];
					if (ch >= 'A' && ch <= 'Z')
					{
						return static_cast<sf::Keyboard::Key>(sf::Keyboard::A + ch - 'A');
					}
					if (ch >= 'a' && ch <= 'z')
					{
						return static_cast<sf::Keyboard::Key>(sf::Keyboard::A + ch - 'a');
					}
					if (ch >= '0' && ch <= '9')
					{
						return static_cast<sf::Keyboard::Key>(sf::Keyboard::Num0 + ch - '0');
					}
					if (ch == ' ')
					{
						return sf::Keyboard::Space;
					}
				}
				else
				{
					switch (str2int(toLower(str).c_str()))
					{
					case str2int("esc"):
					case str2int("escape"):
						return sf::Keyboard::Escape;
					case str2int("space"):
						return sf::Keyboard::Space;
					case str2int("enter"):
					case str2int("return"):
						return sf::Keyboard::Return;
					case str2int("backspace"):
						return sf::Keyboard::BackSpace;
					case str2int("tab"):
						return sf::Keyboard::Tab;
					case str2int("pageup"):
						return sf::Keyboard::PageUp;
					case str2int("pagedown"):
						return sf::Keyboard::PageDown;
					case str2int("end"):
						return sf::Keyboard::End;
					case str2int("home"):
						return sf::Keyboard::Home;
					case str2int("insert"):
						return sf::Keyboard::Insert;
					case str2int("delete"):
						return sf::Keyboard::Delete;
					case str2int("left"):
						return sf::Keyboard::Left;
					case str2int("right"):
						return sf::Keyboard::Right;
					case str2int("up"):
						return sf::Keyboard::Up;
					case str2int("down"):
						return sf::Keyboard::Down;
					case str2int("f1"):
						return sf::Keyboard::F1;
					case str2int("f2"):
						return sf::Keyboard::F2;
					case str2int("f3"):
						return sf::Keyboard::F3;
					case str2int("f4"):
						return sf::Keyboard::F4;
					case str2int("f5"):
						return sf::Keyboard::F5;
					case str2int("f6"):
						return sf::Keyboard::F6;
					case str2int("f7"):
						return sf::Keyboard::F7;
					case str2int("f8"):
						return sf::Keyboard::F8;
					case str2int("f9"):
						return sf::Keyboard::F9;
					case str2int("f10"):
						return sf::Keyboard::F10;
					case str2int("f11"):
						return sf::Keyboard::F11;
					case str2int("f12"):
						return sf::Keyboard::F12;
					case str2int("f13"):
						return sf::Keyboard::F13;
					case str2int("f14"):
						return sf::Keyboard::F14;
					case str2int("f15"):
						return sf::Keyboard::F15;
					case str2int("pause"):
						return sf::Keyboard::Pause;
					default:
						return val;
					}
				}
			}
		}
		return val;
	}

	IgnoreResource getIgnoreResourceKey(const rapidjson::Value& elem,
		const char* key, IgnoreResource val)
	{
		if (elem.HasMember(key))
		{
			const auto& ignoreElem = elem[key];

			if (ignoreElem.IsBool())
			{
				if (ignoreElem.GetBool() == true)
				{
					return IgnoreResource::DrawAndUpdate;
				}
				else
				{
					return IgnoreResource::None;
				}
			}
			else if (ignoreElem.IsString())
			{
				switch (str2int(toLower(elem[key].GetString()).c_str()))
				{
				case str2int("none"):
					return IgnoreResource::None;
				case str2int("drawandupdate"):
					return IgnoreResource::DrawAndUpdate;
				case str2int("update"):
					return IgnoreResource::Update;
				}
			}
		}
		return val;
	}

	PlayerDirection getPlayerDirectionKey(const rapidjson::Value& elem,
		const char* key, PlayerDirection val)
	{
		if (elem.HasMember(key) == false)
		{
			return val;
		}
		const auto& keyElem = elem[key];
		if (keyElem.IsString() == false)
		{
			return val;
		}
		switch (str2int(toLower(keyElem.GetString()).c_str()))
		{
		case str2int("all"):
			return PlayerDirection::All;
		case str2int("front"):
			return PlayerDirection::Front;
		case str2int("frontleft"):
			return PlayerDirection::FrontLeft;
		case str2int("left"):
			return PlayerDirection::Left;
		case str2int("backleft"):
			return PlayerDirection::BackLeft;
		case str2int("back"):
			return PlayerDirection::Back;
		case str2int("backright"):
			return PlayerDirection::BackRight;
		case str2int("right"):
			return PlayerDirection::Right;
		case str2int("frontright"):
			return PlayerDirection::FrontRight;
		default:
			return val;
		}
	}

	PlayerStatus getPlayerStatusKey(const rapidjson::Value& elem,
		const char* key, PlayerStatus val)
	{
		if (elem.HasMember(key) == false)
		{
			return val;
		}
		const auto& keyElem = elem[key];
		if (keyElem.IsString() == false)
		{
			return val;
		}
		switch (str2int(toLower(keyElem.GetString()).c_str()))
		{
		case str2int("stand1"):
			return PlayerStatus::Stand1;
		case str2int("stand2"):
			return PlayerStatus::Stand2;
		case str2int("walk1"):
			return PlayerStatus::Walk1;
		case str2int("walk2"):
			return PlayerStatus::Walk2;
		case str2int("attack1"):
			return PlayerStatus::Attack1;
		case str2int("attack2"):
			return PlayerStatus::Attack2;
		case str2int("attack3"):
			return PlayerStatus::Attack3;
		case str2int("attack4"):
			return PlayerStatus::Attack4;
		case str2int("defend1"):
			return PlayerStatus::Defend1;
		case str2int("defend2"):
			return PlayerStatus::Defend2;
		case str2int("defend3"):
			return PlayerStatus::Defend3;
		case str2int("defend4"):
			return PlayerStatus::Defend4;
		case str2int("hit1"):
			return PlayerStatus::Hit1;
		case str2int("hit2"):
			return PlayerStatus::Hit2;
		case str2int("die1"):
			return PlayerStatus::Die1;
		case str2int("die2"):
			return PlayerStatus::Die2;
		default:
			return val;
		}
	}

	Variable getVariableKey(const rapidjson::Value& elem, const char* key)
	{
		Variable var;
		if (elem.HasMember(key) == true)
		{
			const auto& value = elem[key];
			if (value.IsString())
			{
				var.set<std::string>(std::string(value.GetString()));
			}
			else if (value.IsInt64())
			{
				var.set<int64_t>(value.GetInt64());
			}
			else if (value.IsDouble())
			{
				var.set<double>(value.GetDouble());
			}
			else if (value.IsBool())
			{
				var.set<bool>(value.GetBool());
			}
		}
		return var;
	}
}
