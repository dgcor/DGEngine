#include "ParseUtils.h"
#include "SFMLUtils.h"
#include "Utils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace Utils;

	bool isValidString(const rapidjson::Value& elem, const char* key)
	{
		return (elem.HasMember(key) && elem[key].IsString() && elem[key].GetStringLength() > 0);
	}

	Anchor getAnchor(const char* anchor, Anchor val)
	{
		switch (str2int(anchor))
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

	Anchor getAnchor(const rapidjson::Value& elem, const char* key, Anchor val)
	{
		if (elem.HasMember(key))
		{
			const auto& keyElem = elem[key];
			if (keyElem.IsString())
			{
				return getAnchor(keyElem.GetString(), val);
			}
			else if (keyElem.IsArray())
			{
				Anchor ret = Anchor::None;
				for (const auto& arrElem : keyElem)
				{
					ret |= getAnchor(getString(arrElem).c_str(), val);
				}
				return ret;
			}
		}
		return val;
	}

	bool getBool(const rapidjson::Value& elem, bool val)
	{
		if (elem.IsBool()) {
			return elem.GetBool();
		}
		else {
			return val;
		}
	}

	bool getBool(const rapidjson::Value& elem, const char* key, bool val)
	{
		if (elem.HasMember(key) && elem[key].IsBool()) {
			return elem[key].GetBool();
		}
		else {
			return val;
		}
	}

	bool getBool(const rapidjson::Value& elem, rapidjson::SizeType idx, bool val)
	{
		if (idx < elem.Size() && elem[idx].IsBool()) {
			return elem[idx].GetBool();
		}
		else {
			return val;
		}
	}

	double getDouble(const rapidjson::Value& elem, double val)
	{
		if (elem.IsDouble()) {
			return elem.GetDouble();
		}
		else {
			return val;
		}
	}

	double getDouble(const rapidjson::Value& elem, const char* key, double val)
	{
		if (elem.HasMember(key) && elem[key].IsDouble()) {
			return elem[key].GetDouble();
		}
		else {
			return val;
		}
	}

	double getDouble(const rapidjson::Value& elem, rapidjson::SizeType idx, double val)
	{
		if (idx < elem.Size() && elem[idx].IsDouble()) {
			return elem[idx].GetDouble();
		}
		else {
			return val;
		}
	}

	int getInt(const rapidjson::Value& elem, int val)
	{
		if (elem.IsInt()) {
			return elem.GetInt();
		}
		else {
			return val;
		}
	}

	int getInt(const rapidjson::Value& elem, const char* key, int val)
	{
		if (elem.HasMember(key) && elem[key].IsInt()) {
			return elem[key].GetInt();
		}
		else {
			return val;
		}
	}

	int getInt(const rapidjson::Value& elem, rapidjson::SizeType idx, int val)
	{
		if (idx < elem.Size() && elem[idx].IsInt()) {
			return elem[idx].GetInt();
		}
		else {
			return val;
		}
	}

	int64_t getInt64(const rapidjson::Value& elem, int64_t val)
	{
		if (elem.IsInt64()) {
			return elem.GetInt64();
		}
		else {
			return val;
		}
	}

	int64_t getInt64(const rapidjson::Value& elem, const char* key, int64_t val)
	{
		if (elem.HasMember(key) && elem[key].IsInt64()) {
			return elem[key].GetInt64();
		}
		else {
			return val;
		}
	}

	int64_t getInt64(const rapidjson::Value& elem, rapidjson::SizeType idx, int64_t val)
	{
		if (idx < elem.Size() && elem[idx].IsInt64()) {
			return elem[idx].GetInt64();
		}
		else {
			return val;
		}
	}

	const char* getStringChar_(const rapidjson::Value& elem, const char* val)
	{
		if (elem.IsString()) {
			return elem.GetString();
		}
		else {
			return val;
		}
	}

	std::string getString(const rapidjson::Value& elem, const std::string& val)
	{
		return getStringChar_(elem, val.c_str());
	}

	const char* getStringChar(const rapidjson::Value& elem, const char* key, const char* val)
	{
		if (elem.HasMember(key) && elem[key].IsString()) {
			return elem[key].GetString();
		}
		else {
			return val;
		}
	}

	std::string getString(const rapidjson::Value& elem, const char* key, const std::string& val)
	{
		return getStringChar(elem, key, val.c_str());
	}

	const char* getStringChar_(const rapidjson::Value& elem, rapidjson::SizeType idx, const char* val)
	{
		if (idx < elem.Size() && elem[idx].IsString()) {
			return elem[idx].GetString();
		}
		else {
			return val;
		}
	}

	std::string getString_(const rapidjson::Value& elem, rapidjson::SizeType idx, const std::string& val)
	{
		return getStringChar_(elem, idx, val.c_str());
	}

	unsigned getUInt_(const rapidjson::Value& elem, unsigned val)
	{
		if (elem.IsUint()) {
			return elem.GetUint();
		}
		else {
			return val;
		}
	}

	unsigned getUInt(const rapidjson::Value& elem, const char* key, unsigned val)
	{
		if (elem.HasMember(key) && elem[key].IsUint()) {
			return elem[key].GetUint();
		}
		else {
			return val;
		}
	}

	unsigned getUInt(const rapidjson::Value& elem, rapidjson::SizeType idx, unsigned val)
	{
		if (idx < elem.Size() && elem[idx].IsUint()) {
			return elem[idx].GetUint();
		}
		else {
			return val;
		}
	}

	uint64_t getUInt64(const rapidjson::Value& elem, uint64_t val)
	{
		if (elem.IsUint64()) {
			return elem.GetUint64();
		}
		else {
			return val;
		}
	}

	uint64_t getUInt64(const rapidjson::Value& elem, const char* key, uint64_t val)
	{
		if (elem.HasMember(key) && elem[key].IsUint64()) {
			return elem[key].GetUint64();
		}
		else {
			return val;
		}
	}

	uint64_t getUInt64(const rapidjson::Value& elem, rapidjson::SizeType idx, uint64_t val)
	{
		if (idx < elem.Size() && elem[idx].IsUint64()) {
			return elem[idx].GetUint64();
		}
		else {
			return val;
		}
	}

	sf::IntRect getIntRect(const rapidjson::Value& elem, const char* key, const sf::IntRect& val)
	{
		if (elem.HasMember(key))
		{
			return getIntRect(elem[key], val);
		}
		return val;
	}

	sf::IntRect getIntRect(const rapidjson::Value& elem, const sf::IntRect& val)
	{
		if (elem.IsArray() && elem.Size() >= 4)
		{
			return sf::IntRect(getInt(elem[0]), getInt(elem[1]), getInt(elem[2]), getInt(elem[3]));
		}
		else if (elem.IsArray() && elem.Size() >= 2)
		{
			return sf::IntRect(0, 0, getInt(elem[0]), getInt(elem[1]));
		}
		return val;
	}

	sf::FloatRect getFloatRect(const rapidjson::Value& elem, const char* key, const sf::FloatRect& val)
	{
		if (elem.HasMember(key))
		{
			return getFloatRect(elem[key], val);
		}
		return val;
	}

	sf::FloatRect getFloatRect(const rapidjson::Value& elem, const sf::FloatRect& val)
	{
		if (elem.IsArray() && elem.Size() >= 4)
		{
			return sf::FloatRect(getDouble(elem[0]), getDouble(elem[1]), getDouble(elem[2]), getDouble(elem[3]));
		}
		else if (elem.IsArray() && elem.Size() >= 2)
		{
			return sf::FloatRect(0, 0, getDouble(elem[0]), getDouble(elem[1]));
		}
		return val;
	}

	sf::Color getColor(const rapidjson::Value& elem, const char* key, const sf::Color& val)
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

	std::vector<std::string> getStringVector(const rapidjson::Value& elem, const char* key)
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

	sf::Keyboard::Key getKeyCode(const rapidjson::Value& elem, sf::Keyboard::Key val)
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

	IgnoreResource getIgnoreResource(const rapidjson::Value& elem, const char* key, IgnoreResource val)
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

	Variable getVariable(const rapidjson::Value& elem, const char* key)
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
