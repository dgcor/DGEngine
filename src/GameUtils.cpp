#include "GameUtils.h"
#if ((_MSC_VER >= 1914) || (__GNUC__ >= 8))
#include <charconv>
#else
#include <cstdlib>
#endif
#include "Game.h"
#include "Utils/Utils.h"

namespace GameUtils
{
	AnimationType getAnimationType(const std::string_view str, AnimationType val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("looped"):
			return AnimationType::Looped;
		case str2int16("backandforth"):
			return AnimationType::BackAndForth;
		case str2int16("playonce"):
			return AnimationType::PlayOnce;
		default:
			return val;
		}
	}

	sf::Vector2f getAlignmentPosition(const sf::Vector2f& position,
		const sf::Vector2f& size,
		HorizontalAlign horizAlign,
		VerticalAlign vertAlign) noexcept
	{
		auto drawPos = position;
		switch (horizAlign)
		{
		default:
		case HorizontalAlign::Left:
			break;
		case HorizontalAlign::Center:
		{
			drawPos.x -= std::round(size.x / 2);
		}
		break;
		case HorizontalAlign::Right:
		{
			drawPos.x -= size.x;
		}
		break;
		}
		switch (vertAlign)
		{
		default:
		case VerticalAlign::Bottom:
			break;
		case VerticalAlign::Center:
		{
			drawPos.y -= std::round(size.y / 2);
		}
		break;
		case VerticalAlign::Top:
		{
			drawPos.y -= size.y;
		}
		break;
		}
		return drawPos;
	}

	sf::Vector2f getAnchorPosition(Anchor srcAnchor, const sf::Vector2f& srcSize,
		const sf::Vector2f& srcPos, const sf::Vector2f& anchorToSize) noexcept
	{
		auto pos = srcPos;
		if (srcAnchor == (Anchor::Bottom | Anchor::Left))
		{
			pos.x -= srcSize.x;
			pos.y += anchorToSize.y;
			return pos;
		}
		else if (srcAnchor == (Anchor::Bottom))
		{
			pos.x += std::round((anchorToSize.x / 2) - (srcSize.x / 2));
			pos.y += anchorToSize.y;
			return pos;
		}
		else if (srcAnchor == (Anchor::Bottom | Anchor::Right))
		{
			pos.x += anchorToSize.x;
			pos.y += anchorToSize.y;
			return pos;
		}
		else if (srcAnchor == (Anchor::Left))
		{
			pos.x -= srcSize.x;
			pos.y += std::round((anchorToSize.y / 2) - (srcSize.y / 2));
			return pos;
		}
		else if (srcAnchor == (Anchor::None))
		{
			pos.x += std::round((anchorToSize.x / 2) - (srcSize.x / 2));
			pos.y += std::round((anchorToSize.y / 2) - (srcSize.y / 2));
			return pos;
		}
		else if (srcAnchor == (Anchor::Right))
		{
			pos.x += anchorToSize.x;
			pos.y += std::round((anchorToSize.y / 2) - (srcSize.y / 2));
			return pos;
		}
		else if (srcAnchor == (Anchor::Top | Anchor::Left))
		{
			pos.x -= srcSize.x;
			pos.y -= srcSize.y;
			return pos;
		}
		else if (srcAnchor == (Anchor::Top))
		{
			pos.x += std::round((anchorToSize.x / 2) - (srcSize.x / 2));
			pos.y -= srcSize.y;
			return pos;
		}
		else if (srcAnchor == (Anchor::Top | Anchor::Right))
		{
			pos.x += anchorToSize.x;
			pos.y -= srcSize.y;
			return pos;
		}
		else
		{
			return pos;
		}
	}

	void setAnchorPosSize(Anchor anchor, sf::Vector2f& pos, sf::Vector2f& size,
		const sf::Vector2u& oldWindowSize, const sf::Vector2u& newWindowSize) noexcept
	{
		if (((int)anchor & (int)Anchor::Top) != 0)
		{
			if (((int)anchor & (int)Anchor::Bottom) != 0)
			{
				size.y = std::round(size.y + (float)newWindowSize.y - (float)oldWindowSize.y);
			}
		}
		else
		{
			if (((int)anchor & (int)Anchor::Bottom) != 0)
			{
				pos.y = std::round(pos.y + (float)newWindowSize.y - (float)oldWindowSize.y);
			}
			else
			{
				pos.y = std::round(pos.y + ((float)newWindowSize.y - (float)oldWindowSize.y) / 2.f);
			}
		}
		if (((int)anchor & (int)Anchor::Left) != 0)
		{
			if (((int)anchor & (int)Anchor::Right) != 0)
			{
				size.x = std::round(size.x + (float)newWindowSize.x - (float)oldWindowSize.x);
			}
		}
		else
		{
			if (((int)anchor & (int)Anchor::Right) != 0)
			{
				pos.x = std::round(pos.x + (float)newWindowSize.x - (float)oldWindowSize.x);
			}
			else
			{
				pos.x = std::round(pos.x + ((float)newWindowSize.x - (float)oldWindowSize.x) / 2.f);
			}
		}
	}

	Anchor getAnchor(const std::string_view str, Anchor val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("none"):
			return Anchor::None;
		case str2int16("top"):
			return Anchor::Top;
		case str2int16("topleft"):
			return Anchor::Top | Anchor::Left;
		case str2int16("topright"):
			return Anchor::Top | Anchor::Right;
		case str2int16("bottom"):
			return Anchor::Bottom;
		case str2int16("bottomleft"):
			return Anchor::Bottom | Anchor::Left;
		case str2int16("bottomright"):
			return Anchor::Bottom | Anchor::Right;
		case str2int16("left"):
			return Anchor::Left;
		case str2int16("right"):
			return Anchor::Right;
		case str2int16("all"):
			return Anchor::All;
		default:
			return val;
		}
	}

	BlendMode getBlendMode(const std::string_view str, BlendMode val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("alpha"):
			return BlendMode::Alpha;
		case str2int16("add"):
			return BlendMode::Add;
		case str2int16("multiply"):
			return BlendMode::Multiply;
		case str2int16("none"):
			return BlendMode::None;
		default:
			return val;
		}
	}

	Palette::ColorFormat getColorFormat(const std::string_view str)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("bgr"):
			return Palette::ColorFormat::BGR;
			break;
		case str2int16("rgba"):
			return Palette::ColorFormat::RGBA;
			break;
		case str2int16("bgra"):
			return Palette::ColorFormat::BGRA;
			break;
		case str2int16("argb"):
			return Palette::ColorFormat::ARGB;
			break;
		case str2int16("abgr"):
			return Palette::ColorFormat::ABGR;
			break;
		default:
			return Palette::ColorFormat::RGB;
		}
	}

	HorizontalAlign getHorizontalAlignment(const std::string_view str, HorizontalAlign val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("left"):
			return HorizontalAlign::Left;
		case str2int16("center"):
			return HorizontalAlign::Center;
		case str2int16("right"):
			return HorizontalAlign::Right;
		default:
			return val;
		}
	}

	VerticalAlign getVerticalAlignment(const std::string_view str, VerticalAlign val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("top"):
			return VerticalAlign::Top;
		case str2int16("center"):
			return VerticalAlign::Center;
		case str2int16("bottom"):
			return VerticalAlign::Bottom;
		default:
			return val;
		}
	}

	sf::Keyboard::Key getKeyCode(int num, sf::Keyboard::Key val) noexcept
	{
		if (num >= 0 && num <= 9)
		{
			return static_cast<sf::Keyboard::Key>(sf::Keyboard::Num0 + num);
		}
		return val;
	}

	sf::Keyboard::Key getKeyCode(const std::string_view str, sf::Keyboard::Key val)
	{
		if (str.empty() == false)
		{
			if (str.size() == 1)
			{
				char ch = str[0];
				if (ch >= 'A' && ch <= 'Z')
				{
					return static_cast<sf::Keyboard::Key>(sf::Keyboard::A + ch - 'A');
				}
				else if (ch >= 'a' && ch <= 'z')
				{
					return static_cast<sf::Keyboard::Key>(sf::Keyboard::A + ch - 'a');
				}
				else if (ch >= '0' && ch <= '9')
				{
					return static_cast<sf::Keyboard::Key>(sf::Keyboard::Num0 + ch - '0');
				}
				switch (ch)
				{
				case '[':
					return sf::Keyboard::LBracket;
				case ']':
					return sf::Keyboard::RBracket;
				case ';':
					return sf::Keyboard::Semicolon;
				case ',':
					return sf::Keyboard::Comma;
				case '.':
					return sf::Keyboard::Period;
				case '\'':
					return sf::Keyboard::Quote;
				case '\\':
					return sf::Keyboard::Backslash;
				case '~':
					return sf::Keyboard::Tilde;
				case '=':
					return sf::Keyboard::Equal;
				case ' ':
					return sf::Keyboard::Space;
				case '+':
					return sf::Keyboard::Add;
				case '-':
					return sf::Keyboard::Subtract;
				case '*':
					return sf::Keyboard::Multiply;
				case '/':
					return sf::Keyboard::Divide;
				default:
					return val;
				}
			}
			else
			{
				switch (str2int16(Utils::toLower(str)))
				{
				case str2int16("all"):
				case str2int16("any"):
					return sf::Keyboard::KeyCount;
				case str2int16("esc"):
				case str2int16("escape"):
					return sf::Keyboard::Escape;
				case str2int16("leftbracket"):
					return sf::Keyboard::LBracket;
				case str2int16("rightbracket"):
					return sf::Keyboard::RBracket;
				case str2int16("semicolon"):
					return sf::Keyboard::Semicolon;
				case str2int16("comma"):
					return sf::Keyboard::Comma;
				case str2int16("period"):
					return sf::Keyboard::Period;
				case str2int16("quote"):
					return sf::Keyboard::Quote;
				case str2int16("slash"):
					return sf::Keyboard::Slash;
				case str2int16("backslash"):
					return sf::Keyboard::Backslash;
				case str2int16("tilde"):
					return sf::Keyboard::Tilde;
				case str2int16("equal"):
					return sf::Keyboard::Equal;
				case str2int16("hyphen"):
					return sf::Keyboard::Hyphen;
				case str2int16("space"):
					return sf::Keyboard::Space;
				case str2int16("enter"):
				case str2int16("return"):
					return sf::Keyboard::Return;
				case str2int16("backspace"):
					return sf::Keyboard::BackSpace;
				case str2int16("tab"):
					return sf::Keyboard::Tab;
				case str2int16("pageup"):
					return sf::Keyboard::PageUp;
				case str2int16("pagedown"):
					return sf::Keyboard::PageDown;
				case str2int16("end"):
					return sf::Keyboard::End;
				case str2int16("home"):
					return sf::Keyboard::Home;
				case str2int16("insert"):
					return sf::Keyboard::Insert;
				case str2int16("delete"):
					return sf::Keyboard::Delete;
				case str2int16("add"):
					return sf::Keyboard::Add;
				case str2int16("subtract"):
					return sf::Keyboard::Subtract;
				case str2int16("multiply"):
					return sf::Keyboard::Multiply;
				case str2int16("divide"):
					return sf::Keyboard::Divide;
				case str2int16("left"):
					return sf::Keyboard::Left;
				case str2int16("right"):
					return sf::Keyboard::Right;
				case str2int16("up"):
					return sf::Keyboard::Up;
				case str2int16("down"):
					return sf::Keyboard::Down;
				case str2int16("f1"):
					return sf::Keyboard::F1;
				case str2int16("f2"):
					return sf::Keyboard::F2;
				case str2int16("f3"):
					return sf::Keyboard::F3;
				case str2int16("f4"):
					return sf::Keyboard::F4;
				case str2int16("f5"):
					return sf::Keyboard::F5;
				case str2int16("f6"):
					return sf::Keyboard::F6;
				case str2int16("f7"):
					return sf::Keyboard::F7;
				case str2int16("f8"):
					return sf::Keyboard::F8;
				case str2int16("f9"):
					return sf::Keyboard::F9;
				case str2int16("f10"):
					return sf::Keyboard::F10;
				case str2int16("f11"):
					return sf::Keyboard::F11;
				case str2int16("f12"):
					return sf::Keyboard::F12;
				case str2int16("f13"):
					return sf::Keyboard::F13;
				case str2int16("f14"):
					return sf::Keyboard::F14;
				case str2int16("f15"):
					return sf::Keyboard::F15;
				case str2int16("pause"):
					return sf::Keyboard::Pause;
				default:
					return val;
				}
			}
		}
		return val;
	}

	IgnoreResource getIgnoreResource(const std::string_view str, IgnoreResource val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("none"):
			return IgnoreResource::None;
		case str2int16("alldraws"):
			return IgnoreResource::Draw | IgnoreResource::All;
		case str2int16("alldrawsandupdates"):
			return IgnoreResource::Draw | IgnoreResource::Update | IgnoreResource::All;
		case str2int16("allupdates"):
			return IgnoreResource::Update | IgnoreResource::All;
		case str2int16("draw"):
			return IgnoreResource::Draw;
		case str2int16("drawandupdate"):
			return IgnoreResource::Draw | IgnoreResource::Update;
		case str2int16("update"):
			return IgnoreResource::Update;
		}
		return val;
	}

	InputEventType getInputEventType(const std::string_view str, InputEventType val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("none"):
			return InputEventType::None;
		case str2int16("leftclick"):
			return InputEventType::LeftClick;
		case str2int16("middleclick"):
			return InputEventType::MiddleClick;
		case str2int16("rightclick"):
			return InputEventType::RightClick;
		case str2int16("mousepress"):
			return InputEventType::MousePress;
		case str2int16("mousemove"):
			return InputEventType::MouseMove;
		case str2int16("mouserelease"):
			return InputEventType::MouseRelease;
		case str2int16("mousescroll"):
			return InputEventType::MouseScroll;
		case str2int16("keypress"):
			return InputEventType::KeyPress;
		case str2int16("textenter"):
			return InputEventType::TextEnter;
		case str2int16("touchbegin"):
			return InputEventType::TouchBegin;
		case str2int16("touchmove"):
			return InputEventType::TouchMove;
		case str2int16("touchend"):
			return InputEventType::TouchEnd;
		case str2int16("allleftclick"):
			return InputEventType::All ^ (InputEventType::MiddleClick | InputEventType::RightClick);
		case str2int16("allmiddleclick"):
			return InputEventType::All ^ (InputEventType::LeftClick | InputEventType::RightClick);
		case str2int16("allrightclick"):
			return InputEventType::All ^ (InputEventType::LeftClick | InputEventType::MiddleClick);
		case str2int16("all"):
			return InputEventType::All;
		}
		return val;
	}

	InventoryPosition getInventoryPosition(const std::string_view str, InventoryPosition val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("topleft"):
			return InventoryPosition::TopLeft;
		case str2int16("topright"):
			return InventoryPosition::TopRight;
		case str2int16("bottomleft"):
			return InventoryPosition::BottomLeft;
		case str2int16("bottomright"):
			return InventoryPosition::BottomRight;
		}
		return val;
	}

	PlayerDirection getPlayerDirection(const std::string_view str, PlayerDirection val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("all"):
			return PlayerDirection::All;
		case str2int16("front"):
			return PlayerDirection::Front;
		case str2int16("frontleft"):
			return PlayerDirection::FrontLeft;
		case str2int16("left"):
			return PlayerDirection::Left;
		case str2int16("backleft"):
			return PlayerDirection::BackLeft;
		case str2int16("back"):
			return PlayerDirection::Back;
		case str2int16("backright"):
			return PlayerDirection::BackRight;
		case str2int16("right"):
			return PlayerDirection::Right;
		case str2int16("frontright"):
			return PlayerDirection::FrontRight;
		default:
			return val;
		}
	}

	PlayerInventory getPlayerInventory(const std::string_view str, PlayerInventory val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("body"):
			return PlayerInventory::Body;
		case str2int16("belt"):
			return PlayerInventory::Belt;
		case str2int16("stash"):
			return PlayerInventory::Stash;
		default:
			return val;
		}
	}

	size_t getPlayerInventoryIndex(const std::string_view str, PlayerInventory val)
	{
#if ((_MSC_VER >= 1914) || (__GNUC__ >= 8))
		size_t num = (size_t)val;
		auto err = std::from_chars(str.data(), str.data() + str.size(), num);
		if (err.ec == std::errc() ||
			err.ec == std::errc::result_out_of_range)
		{
			return num;
		}
		return (size_t)getPlayerInventory(str, val);
#else
		int& errno_ref = errno;
		const char* sPtr = str.data();
		char* ePtr;
		errno_ref = 0;
		size_t val2 = (size_t)std::strtoul(sPtr, &ePtr, 10);

		if (errno_ref == ERANGE) // out of range
		{
			return (size_t)val;
		}
		else if (sPtr == ePtr) // invalid argument
		{
			return (size_t)getPlayerInventory(str, val);
		}
		else
		{
			return val2;
		}
#endif
	}

	PlayerItemMount getPlayerItemMount(const std::string_view str, PlayerItemMount val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("lefthand"):
			return PlayerItemMount::LeftHand;
		case str2int16("righthand"):
			return PlayerItemMount::RightHand;
		case str2int16("armor"):
			return PlayerItemMount::Armor;
		case str2int16("helmet"):
			return PlayerItemMount::Helmet;
		case str2int16("amulet"):
			return PlayerItemMount::Amulet;
		case str2int16("leftring"):
			return PlayerItemMount::LeftRing;
		case str2int16("rightring"):
			return PlayerItemMount::RightRing;
		default:
			return val;
		}
	}

	size_t getPlayerItemMountIndex(const std::string_view str, PlayerItemMount val)
	{
#if ((_MSC_VER >= 1914) || (__GNUC__ >= 8))
		size_t num = (size_t)val;
		auto err = std::from_chars(str.data(), str.data() + str.size(), num);
		if (err.ec == std::errc() ||
			err.ec == std::errc::result_out_of_range)
		{
			return num;
		}
		return (size_t)getPlayerItemMount(str, val);
#else
		int& errno_ref = errno;
		const char* sPtr = str.data();
		char* ePtr;
		errno_ref = 0;
		size_t val2 = (size_t)std::strtoul(sPtr, &ePtr, 10);

		if (errno_ref == ERANGE) // out of range
		{
			return (size_t)val;
		}
		else if (sPtr == ePtr) // invalid argument
		{
			return (size_t)getPlayerItemMount(str, val);
		}
		else
		{
			return val2;
		}
#endif
	}

	PlayerAnimation getPlayerAnimation(const std::string_view str, PlayerAnimation val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("stand1"):
			return PlayerAnimation::Stand1;
		case str2int16("stand2"):
			return PlayerAnimation::Stand2;
		case str2int16("walk1"):
			return PlayerAnimation::Walk1;
		case str2int16("walk2"):
			return PlayerAnimation::Walk2;
		case str2int16("attack1"):
			return PlayerAnimation::Attack1;
		case str2int16("attack2"):
			return PlayerAnimation::Attack2;
		case str2int16("attack3"):
			return PlayerAnimation::Attack3;
		case str2int16("attack4"):
			return PlayerAnimation::Attack4;
		case str2int16("defend1"):
			return PlayerAnimation::Defend1;
		case str2int16("defend2"):
			return PlayerAnimation::Defend2;
		case str2int16("defend3"):
			return PlayerAnimation::Defend3;
		case str2int16("defend4"):
			return PlayerAnimation::Defend4;
		case str2int16("hit1"):
			return PlayerAnimation::Hit1;
		case str2int16("hit2"):
			return PlayerAnimation::Hit2;
		case str2int16("die1"):
			return PlayerAnimation::Die1;
		case str2int16("die2"):
			return PlayerAnimation::Die2;
		default:
			return val;
		}
	}

	PlayerStatus getPlayerStatus(const std::string_view str, PlayerStatus val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("stand"):
			return PlayerStatus::Stand;
		case str2int16("walk"):
			return PlayerStatus::Walk;
		case str2int16("attack"):
			return PlayerStatus::Attack;
		case str2int16("dead"):
			return PlayerStatus::Dead;
		default:
			return val;
		}
	}

	sf::Time getTime(int fps)
	{
		fps = std::clamp(fps, 1, 1000);
		return sf::seconds(1.f / (float)fps);
	}

	std::string replaceStringWithQueryable(const std::string_view str,
		const Queryable& obj, char token)
	{
		std::string str2(str);
		size_t firstTokenStart = 0;
		while (true)
		{
			firstTokenStart = str.find(token, firstTokenStart);
			if (firstTokenStart == std::string_view::npos)
			{
				break;
			}
			size_t firstTokenStop = firstTokenStart + 1;
			size_t secondTokenStart = str.find_first_of(token, firstTokenStop);
			if (secondTokenStart == std::string_view::npos)
			{
				break;
			}
			size_t secondTokenStop = secondTokenStart + 1;

			std::string_view strProp(str.data() + firstTokenStop, secondTokenStart - firstTokenStop);
			Variable var;
			if (obj.getProperty(strProp, var) == true)
			{
				std::string_view strProp2(str.data() + firstTokenStart, secondTokenStop - firstTokenStart);
				Utils::replaceStringInPlace(str2, strProp2, VarUtils::toString(var));
			}
			firstTokenStart = secondTokenStart;
		}
		return str2;
	}

	std::string replaceStringWithVarOrProp(const std::string_view str,
		const Game& obj, char token)
	{
		std::string str2(str);
		size_t firstTokenStart = 0;
		while (true)
		{
			firstTokenStart = str.find(token, firstTokenStart);
			if (firstTokenStart == std::string_view::npos)
			{
				break;
			}
			size_t firstTokenStop = firstTokenStart + 1;
			size_t secondTokenStart = str.find_first_of(token, firstTokenStop);
			if (secondTokenStart == std::string_view::npos)
			{
				break;
			}
			size_t secondTokenStop = secondTokenStart + 1;

			std::string_view strProp(str.data() + firstTokenStop, secondTokenStart - firstTokenStop);
			Variable var;
			if (obj.getVarOrPropNoToken(strProp, var) == true)
			{
				std::string_view strProp2(str.data() + firstTokenStart, secondTokenStop - firstTokenStart);
				Utils::replaceStringInPlace(str2, strProp2, VarUtils::toString(var));
			}
			firstTokenStart = secondTokenStart;
		}
		return str2;
	}
}
