#include "GameUtils.h"
#include <cmath>
#include <version>
#ifdef __cpp_lib_format
#include <format>
#else
#include <cstdio>
#endif
#include "Game/Game.h"
#include "Utils/StringHash.h"
#include "Utils/Utils.h"
#include "VarUtils.h"

namespace GameUtils
{
	sf::Time FPSToTime(int fps)
	{
		fps = std::clamp(fps, 1, 1000);
		return sf::seconds(1.f / (float)fps);
	}

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
			drawPos.x -= std::round(size.x / 2.f);
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
			drawPos.y -= std::round(size.y / 2.f);
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
			pos.x += std::round((anchorToSize.x / 2.f) - (srcSize.x / 2.f));
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
			pos.y += std::round((anchorToSize.y / 2.f) - (srcSize.y / 2.f));
			return pos;
		}
		else if (srcAnchor == (Anchor::None))
		{
			pos.x += std::round((anchorToSize.x / 2.f) - (srcSize.x / 2.f));
			pos.y += std::round((anchorToSize.y / 2.f) - (srcSize.y / 2.f));
			return pos;
		}
		else if (srcAnchor == (Anchor::Right))
		{
			pos.x += anchorToSize.x;
			pos.y += std::round((anchorToSize.y / 2.f) - (srcSize.y / 2.f));
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
			pos.x += std::round((anchorToSize.x / 2.f) - (srcSize.x / 2.f));
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
		if ((int)(anchor & Anchor::Top) != 0)
		{
			if ((int)(anchor & Anchor::Bottom) != 0)
			{
				size.y = std::round(size.y + (float)newWindowSize.y - (float)oldWindowSize.y);
			}
		}
		else
		{
			if ((int)(anchor & Anchor::Bottom) != 0)
			{
				pos.y = std::round(pos.y + (float)newWindowSize.y - (float)oldWindowSize.y);
			}
			else
			{
				pos.y = std::round(pos.y + ((float)newWindowSize.y - (float)oldWindowSize.y) / 2.f);
			}
		}
		if ((int)(anchor & Anchor::Left) != 0)
		{
			if ((int)(anchor & Anchor::Right) != 0)
			{
				size.x = std::round(size.x + (float)newWindowSize.x - (float)oldWindowSize.x);
			}
		}
		else
		{
			if ((int)(anchor & Anchor::Right) != 0)
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

	BindingFlags getBindingFlags(const std::string_view str, BindingFlags val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("onchange"):
			return BindingFlags::OnChange;
		case str2int16("once"):
			return BindingFlags::Once;
		case str2int16("always"):
			return BindingFlags::Always;
		case str2int16("whenhidden"):
			return BindingFlags::WhenHidden;
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
		case str2int16("min"):
			return BlendMode::Min;
		case str2int16("max"):
			return BlendMode::Max;
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

	Direction getDirection(const std::string_view str, Direction val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("up"):
			return Direction::Up;
		case str2int16("down"):
			return Direction::Down;
		case str2int16("left"):
			return Direction::Left;
		case str2int16("right"):
			return Direction::Right;
		default:
			return val;
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

	sf::PrimitiveType getPrimitiveType(const std::string_view str, sf::PrimitiveType val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("points"):
			return sf::PrimitiveType::Points;
		case str2int16("lines"):
			return sf::PrimitiveType::Lines;
		case str2int16("linestrip"):
			return sf::PrimitiveType::LineStrip;
		case str2int16("triangles"):
			return sf::PrimitiveType::Triangles;
		case str2int16("trianglestrip"):
			return sf::PrimitiveType::TriangleStrip;
		case str2int16("trianglefan"):
			return sf::PrimitiveType::TriangleFan;
		default:
			return val;
		}
	}

	sf::Keyboard::Key getKeyCode(int num, sf::Keyboard::Key val) noexcept
	{
		if (num >= 0 && num <= 9)
		{
			return static_cast<sf::Keyboard::Key>(sf::Keyboard::Key::Num0 + num);
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
					return static_cast<sf::Keyboard::Key>(sf::Keyboard::Key::A + ch - 'A');
				}
				else if (ch >= 'a' && ch <= 'z')
				{
					return static_cast<sf::Keyboard::Key>(sf::Keyboard::Key::A + ch - 'a');
				}
				else if (ch >= '0' && ch <= '9')
				{
					return static_cast<sf::Keyboard::Key>(sf::Keyboard::Key::Num0 + ch - '0');
				}
				switch (ch)
				{
				case '[':
					return sf::Keyboard::Key::LBracket;
				case ']':
					return sf::Keyboard::Key::RBracket;
				case ';':
					return sf::Keyboard::Key::Semicolon;
				case ',':
					return sf::Keyboard::Key::Comma;
				case '.':
					return sf::Keyboard::Key::Period;
				case '\'':
					return sf::Keyboard::Key::Apostrophe;
				case '\\':
					return sf::Keyboard::Key::Backslash;
				case '~':
					return sf::Keyboard::Key::Tilde;
				case '=':
					return sf::Keyboard::Key::Equal;
				case ' ':
					return sf::Keyboard::Key::Space;
				case '+':
					return sf::Keyboard::Key::Add;
				case '-':
					return sf::Keyboard::Key::Subtract;
				case '*':
					return sf::Keyboard::Key::Multiply;
				case '/':
					return sf::Keyboard::Key::Divide;
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
					return sf::Keyboard::Key::Escape;
				case str2int16("lcontrol"):
					return sf::Keyboard::Key::LControl;
				case str2int16("lshift"):
					return sf::Keyboard::Key::LShift;
				case str2int16("lalt"):
					return sf::Keyboard::Key::LAlt;
				case str2int16("lsystem"):
					return sf::Keyboard::Key::LSystem;
				case str2int16("rcontrol"):
					return sf::Keyboard::Key::RControl;
				case str2int16("rshift"):
					return sf::Keyboard::Key::RShift;
				case str2int16("ralt"):
					return sf::Keyboard::Key::RAlt;
				case str2int16("rsystem"):
					return sf::Keyboard::Key::RSystem;
				case str2int16("menu"):
					return sf::Keyboard::Key::Menu;
				case str2int16("leftbracket"):
					return sf::Keyboard::Key::LBracket;
				case str2int16("rightbracket"):
					return sf::Keyboard::Key::RBracket;
				case str2int16("semicolon"):
					return sf::Keyboard::Key::Semicolon;
				case str2int16("comma"):
					return sf::Keyboard::Key::Comma;
				case str2int16("period"):
					return sf::Keyboard::Key::Period;
				case str2int16("apostrophe"):
				case str2int16("quote"):
					return sf::Keyboard::Key::Apostrophe;
				case str2int16("slash"):
					return sf::Keyboard::Key::Slash;
				case str2int16("backslash"):
					return sf::Keyboard::Key::Backslash;
				case str2int16("tilde"):
					return sf::Keyboard::Key::Tilde;
				case str2int16("equal"):
					return sf::Keyboard::Key::Equal;
				case str2int16("hyphen"):
					return sf::Keyboard::Key::Hyphen;
				case str2int16("space"):
					return sf::Keyboard::Key::Space;
				case str2int16("enter"):
				case str2int16("return"):
					return sf::Keyboard::Key::Return;
				case str2int16("backspace"):
					return sf::Keyboard::Key::BackSpace;
				case str2int16("tab"):
					return sf::Keyboard::Key::Tab;
				case str2int16("pageup"):
					return sf::Keyboard::Key::PageUp;
				case str2int16("pagedown"):
					return sf::Keyboard::Key::PageDown;
				case str2int16("end"):
					return sf::Keyboard::Key::End;
				case str2int16("home"):
					return sf::Keyboard::Key::Home;
				case str2int16("insert"):
					return sf::Keyboard::Key::Insert;
				case str2int16("delete"):
					return sf::Keyboard::Key::Delete;
				case str2int16("add"):
					return sf::Keyboard::Key::Add;
				case str2int16("subtract"):
					return sf::Keyboard::Key::Subtract;
				case str2int16("multiply"):
					return sf::Keyboard::Key::Multiply;
				case str2int16("divide"):
					return sf::Keyboard::Key::Divide;
				case str2int16("left"):
					return sf::Keyboard::Key::Left;
				case str2int16("right"):
					return sf::Keyboard::Key::Right;
				case str2int16("up"):
					return sf::Keyboard::Key::Up;
				case str2int16("down"):
					return sf::Keyboard::Key::Down;
				case str2int16("numpad0"):
					return sf::Keyboard::Key::Numpad0;
				case str2int16("numpad1"):
					return sf::Keyboard::Key::Numpad1;
				case str2int16("numpad2"):
					return sf::Keyboard::Key::Numpad2;
				case str2int16("numpad3"):
					return sf::Keyboard::Key::Numpad3;
				case str2int16("numpad4"):
					return sf::Keyboard::Key::Numpad4;
				case str2int16("numpad5"):
					return sf::Keyboard::Key::Numpad5;
				case str2int16("numpad6"):
					return sf::Keyboard::Key::Numpad6;
				case str2int16("numpad7"):
					return sf::Keyboard::Key::Numpad7;
				case str2int16("numpad8"):
					return sf::Keyboard::Key::Numpad8;
				case str2int16("numpad9"):
					return sf::Keyboard::Key::Numpad9;
				case str2int16("f1"):
					return sf::Keyboard::Key::F1;
				case str2int16("f2"):
					return sf::Keyboard::Key::F2;
				case str2int16("f3"):
					return sf::Keyboard::Key::F3;
				case str2int16("f4"):
					return sf::Keyboard::Key::F4;
				case str2int16("f5"):
					return sf::Keyboard::Key::F5;
				case str2int16("f6"):
					return sf::Keyboard::Key::F6;
				case str2int16("f7"):
					return sf::Keyboard::Key::F7;
				case str2int16("f8"):
					return sf::Keyboard::Key::F8;
				case str2int16("f9"):
					return sf::Keyboard::Key::F9;
				case str2int16("f10"):
					return sf::Keyboard::Key::F10;
				case str2int16("f11"):
					return sf::Keyboard::Key::F11;
				case str2int16("f12"):
					return sf::Keyboard::Key::F12;
				case str2int16("f13"):
					return sf::Keyboard::Key::F13;
				case str2int16("f14"):
					return sf::Keyboard::Key::F14;
				case str2int16("f15"):
					return sf::Keyboard::Key::F15;
				case str2int16("pause"):
					return sf::Keyboard::Key::Pause;
				default:
					return val;
				}
			}
		}
		return val;
	}

	sf::Keyboard::Scancode getScanCode(int num, sf::Keyboard::Scancode val) noexcept
	{
		if (num == 0)
		{
			return sf::Keyboard::Scancode::Num0;
		}
		else if (num >= 1 && num <= 9)
		{
			return static_cast<sf::Keyboard::Scancode>(sf::Keyboard::Scancode::Num1 + num - 1);
		}
		return val;
	}

	sf::Keyboard::Scancode getScanCode(const std::string_view str, sf::Keyboard::Scancode val)
	{
		if (str.empty() == false)
		{
			if (str.size() == 1)
			{
				char ch = str[0];
				if (ch >= 'A' && ch <= 'Z')
				{
					return static_cast<sf::Keyboard::Scancode>(sf::Keyboard::Scancode::A + ch - 'A');
				}
				else if (ch >= 'a' && ch <= 'z')
				{
					return static_cast<sf::Keyboard::Scancode>(sf::Keyboard::Scancode::A + ch - 'a');
				}
				else if (ch >= '1' && ch <= '9')
				{
					return static_cast<sf::Keyboard::Scancode>(sf::Keyboard::Scancode::Num1 + ch - '1');
				}
				switch (ch)
				{
				case '0':
					return sf::Keyboard::Scancode::Num0;
				case '[':
					return sf::Keyboard::Scancode::LBracket;
				case ']':
					return sf::Keyboard::Scancode::RBracket;
				case ';':
					return sf::Keyboard::Scancode::Semicolon;
				case ',':
					return sf::Keyboard::Scancode::Comma;
				case '.':
					return sf::Keyboard::Scancode::Period;
				case '\'':
					return sf::Keyboard::Scancode::Apostrophe;
				case '\\':
					return sf::Keyboard::Scancode::Backslash;
				case '`':
				case '~':
					return sf::Keyboard::Scancode::Grave;
				case '=':
					return sf::Keyboard::Scancode::Equal;
				case ' ':
					return sf::Keyboard::Scancode::Space;
				case '+':
					return sf::Keyboard::Scancode::Equal;
				case '-':
					return sf::Keyboard::Scancode::Hyphen;
				case '/':
					return sf::Keyboard::Scancode::Slash;
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
					return sf::Keyboard::Scancode::ScancodeCount;
				case str2int16("esc"):
				case str2int16("escape"):
					return sf::Keyboard::Scancode::Escape;
				case str2int16("lcontrol"):
					return sf::Keyboard::Scancode::LControl;
				case str2int16("lshift"):
					return sf::Keyboard::Scancode::LShift;
				case str2int16("lalt"):
					return sf::Keyboard::Scancode::LAlt;
				case str2int16("lsystem"):
					return sf::Keyboard::Scancode::LSystem;
				case str2int16("rcontrol"):
					return sf::Keyboard::Scancode::RControl;
				case str2int16("rshift"):
					return sf::Keyboard::Scancode::RShift;
				case str2int16("ralt"):
					return sf::Keyboard::Scancode::RAlt;
				case str2int16("rsystem"):
					return sf::Keyboard::Scancode::RSystem;
				case str2int16("menu"):
					return sf::Keyboard::Scancode::Menu;
				case str2int16("leftbracket"):
					return sf::Keyboard::Scancode::LBracket;
				case str2int16("rightbracket"):
					return sf::Keyboard::Scancode::RBracket;
				case str2int16("semicolon"):
					return sf::Keyboard::Scancode::Semicolon;
				case str2int16("comma"):
					return sf::Keyboard::Scancode::Comma;
				case str2int16("period"):
					return sf::Keyboard::Scancode::Period;
				case str2int16("apostrophe"):
				case str2int16("quote"):
					return sf::Keyboard::Scancode::Apostrophe;
				case str2int16("slash"):
					return sf::Keyboard::Scancode::Slash;
				case str2int16("backslash"):
					return sf::Keyboard::Scancode::Backslash;
				case str2int16("tilde"):
					return sf::Keyboard::Scancode::Grave;
				case str2int16("equal"):
					return sf::Keyboard::Scancode::Equal;
				case str2int16("hyphen"):
					return sf::Keyboard::Scancode::Hyphen;
				case str2int16("space"):
					return sf::Keyboard::Scancode::Space;
				case str2int16("enter"):
				case str2int16("return"):
					return sf::Keyboard::Scancode::Enter;
				case str2int16("backspace"):
					return sf::Keyboard::Scancode::Backspace;
				case str2int16("tab"):
					return sf::Keyboard::Scancode::Tab;
				case str2int16("pageup"):
					return sf::Keyboard::Scancode::PageUp;
				case str2int16("pagedown"):
					return sf::Keyboard::Scancode::PageDown;
				case str2int16("end"):
					return sf::Keyboard::Scancode::End;
				case str2int16("home"):
					return sf::Keyboard::Scancode::Home;
				case str2int16("insert"):
					return sf::Keyboard::Scancode::Insert;
				case str2int16("delete"):
					return sf::Keyboard::Scancode::Delete;
				case str2int16("add"):
					return sf::Keyboard::Scancode::Equal;
				case str2int16("subtract"):
					return sf::Keyboard::Scancode::Hyphen;
				case str2int16("divide"):
					return sf::Keyboard::Scancode::Slash;
				case str2int16("left"):
					return sf::Keyboard::Scancode::Left;
				case str2int16("right"):
					return sf::Keyboard::Scancode::Right;
				case str2int16("up"):
					return sf::Keyboard::Scancode::Up;
				case str2int16("down"):
					return sf::Keyboard::Scancode::Down;
				case str2int16("numpad0"):
					return sf::Keyboard::Scancode::Numpad0;
				case str2int16("numpad1"):
					return sf::Keyboard::Scancode::Numpad1;
				case str2int16("numpad2"):
					return sf::Keyboard::Scancode::Numpad2;
				case str2int16("numpad3"):
					return sf::Keyboard::Scancode::Numpad3;
				case str2int16("numpad4"):
					return sf::Keyboard::Scancode::Numpad4;
				case str2int16("numpad5"):
					return sf::Keyboard::Scancode::Numpad5;
				case str2int16("numpad6"):
					return sf::Keyboard::Scancode::Numpad6;
				case str2int16("numpad7"):
					return sf::Keyboard::Scancode::Numpad7;
				case str2int16("numpad8"):
					return sf::Keyboard::Scancode::Numpad8;
				case str2int16("numpad9"):
					return sf::Keyboard::Scancode::Numpad9;
				case str2int16("f1"):
					return sf::Keyboard::Scancode::F1;
				case str2int16("f2"):
					return sf::Keyboard::Scancode::F2;
				case str2int16("f3"):
					return sf::Keyboard::Scancode::F3;
				case str2int16("f4"):
					return sf::Keyboard::Scancode::F4;
				case str2int16("f5"):
					return sf::Keyboard::Scancode::F5;
				case str2int16("f6"):
					return sf::Keyboard::Scancode::F6;
				case str2int16("f7"):
					return sf::Keyboard::Scancode::F7;
				case str2int16("f8"):
					return sf::Keyboard::Scancode::F8;
				case str2int16("f9"):
					return sf::Keyboard::Scancode::F9;
				case str2int16("f10"):
					return sf::Keyboard::Scancode::F10;
				case str2int16("f11"):
					return sf::Keyboard::Scancode::F11;
				case str2int16("f12"):
					return sf::Keyboard::Scancode::F12;
				case str2int16("f13"):
					return sf::Keyboard::Scancode::F13;
				case str2int16("f14"):
					return sf::Keyboard::Scancode::F14;
				case str2int16("f15"):
					return sf::Keyboard::Scancode::F15;
				case str2int16("pause"):
					return sf::Keyboard::Scancode::Pause;
				default:
					return val;
				}
			}
		}
		return val;
	}

	std::string getScanCodeDescription(const std::string_view str)
	{
		auto scancode = getScanCode(str, sf::Keyboard::Scancode::Unknown);
		auto scancodeStr = sf::Keyboard::getDescription(scancode).toUtf8();
		return std::string((const char*)scancodeStr.data(), scancodeStr.size());
	}

	// total time to seconds
	float getTimeS(sf::Time time, bool roundUp)
	{
		if (roundUp == true)
		{
			return std::ceil(time.asSeconds());
		}
		else
		{
			return std::floor(time.asSeconds());
		}
	}

	// total time to seconds and milliseconds
	float getTimeSf(sf::Time time, float& f)
	{
		float seconds;
		f = std::abs(std::modf(time.asSeconds(), &seconds));
		return seconds;
	}

	// total time to minutes and seconds
	void getTimeMS(int32_t seconds, float& m, float& s)
	{
		auto pSeconds = std::abs(seconds);
		bool negative = seconds != pSeconds;
		seconds = pSeconds;
		if (seconds < 60)
		{
			s = (float)seconds;
		}
		else if (seconds < 3600)
		{
			seconds = seconds % 3600;
			m = (float)(seconds / 60);
			s = (float)(seconds % 60);
		}
		else
		{
			int32_t h = seconds / 3600;
			seconds = seconds % 3600;
			m = (float)((seconds / 60) + (h * 60));
			s = (float)(seconds % 60);
		}
		if (negative == true)
		{
			m = -m;
		}
	}

	// total time to minutes and seconds
	void getTimeMS(sf::Time time, bool roundUp, float& m, float& s)
	{
		auto seconds = getTimeS(time, roundUp);
		getTimeMS((int32_t)seconds, m, s);
	}

	// total time to minutes, seconds and milliseconds
	void getTimeMSf(sf::Time time, float& m, float& s, float& f)
	{
		auto seconds = getTimeSf(time, f);
		getTimeMS((int32_t)seconds, m, s);
	}

	// total time to hours, minutes and seconds
	void getTimeHMS(int32_t seconds, float& h, float& m, float& s)
	{
		auto pSeconds = std::abs(seconds);
		bool negative = seconds != pSeconds;
		seconds = pSeconds;
		if (seconds < 60)
		{
			s = (float)seconds;
		}
		else if (seconds < 3600)
		{
			seconds = seconds % 3600;
			m = (float)(seconds / 60);
			s = (float)(seconds % 60);
		}
		else
		{
			h = (float)(seconds / 3600);
			seconds = seconds % 3600;
			m = (float)(seconds / 60);
			s = (float)(seconds % 60);
		}
		if (negative == true)
		{
			h = -h;
		}
	}

	// total time to hours, minutes and seconds
	void getTimeHMS(sf::Time time, bool roundUp, float& h, float& m, float& s)
	{
		auto seconds = getTimeS(time, roundUp);
		getTimeHMS((int32_t)seconds, h, m, s);
	}

	// total time to hours, minutes, seconds and milliseconds
	void getTimeHMSf(sf::Time time, float& h, float& m, float& s, float& f)
	{
		auto seconds = getTimeSf(time, f);
		getTimeHMS((int32_t)seconds, h, m, s);
	}

#ifndef false
	template<typename... Args>
	std::string stringFormat(const char* fmt, Args... args)
	{
		static constexpr size_t buffSize = 24;
		char buffer[buffSize];
		snprintf(buffer, buffSize, fmt, args...);
		return Utils::trimStart(buffer);
	}
#endif

	std::string getTimeString(std::optional<float> hours, std::optional<float> minutes,
		std::optional<float> seconds, std::optional<float> milliseconds,
		std::optional<int16_t> leadingZeroes, std::optional<int16_t> millisecondDecimals)
	{
		float h = hours ? *hours : 0.f;
		float m = minutes ? *minutes : 0.f;
		float s = seconds ? *seconds : 0.f;
		float ms = 0.f;
		int zeroes = 0;
		int decimals = 0;

		if (milliseconds)
		{
			decimals = std::clamp(millisecondDecimals ? *millisecondDecimals : 0, 0, 3);
			ms = *milliseconds;
			ms = std::round(ms * std::pow(10.f, (float)decimals));
		}
		if (leadingZeroes)
		{
			zeroes = std::clamp(leadingZeroes ? *leadingZeroes : 0, 0, 2);

			// add extra space for minus sign
			if (std::signbit(h) || std::signbit(m) || std::signbit(s) || std::signbit(ms))
			{
				zeroes++;
			}
		}

		if (hours)
		{
#ifdef __cpp_lib_format
			if (decimals)
			{
				return std::format("{:0{}g}:{:02g}:{:02g}.{:0{}g}", h, zeroes, m, s, ms, decimals);
			}
			else
			{
				return std::format("{:0{}g}:{:02g}:{:02g}", h, zeroes, m, s);
			}
#else
			if (decimals)
			{
				return stringFormat("%0*g:%02g:%02g.%0*g", zeroes, h, m, s, decimals, ms);
			}
			else
			{
				return stringFormat("%0*g:%02g:%02g", zeroes, h, m, s);
			}
#endif
		}
		else if (minutes)
		{
#ifdef __cpp_lib_format
			if (decimals)
			{
				return std::format("{:0{}g}:{:02g}.{:0{}g}", m, zeroes, s, ms, decimals);
			}
			else
			{
				return std::format("{:0{}g}:{:02g}", m, zeroes, s);
			}
#else
			if (decimals)
			{
				return stringFormat("%0*g:%02g.%0*g", zeroes, m, s, decimals, ms);
			}
			else
			{
				return stringFormat("%0*g:%02g", zeroes, m, s);
			}
#endif
		}
		else if (seconds)
		{
#ifdef __cpp_lib_format
			if (decimals)
			{
				return std::format("{:0{}g}.{:0{}g}", s, zeroes, ms, decimals);
			}
			else
			{
				return std::format("{:0{}g}", s, zeroes);
			}
#else
			if (decimals)
			{
				return stringFormat("%0*g.%0*g", zeroes, s, decimals, ms);
			}
			else
			{
				return stringFormat("%0*g", zeroes, s);
			}
#endif
		}
		return {};
	}

	Variable getTime(sf::Time time, std::string_view format, bool roundUp)
	{
		switch (str2int16(format))
		{
		case str2int16("ms"):
			return (int64_t)(time.asMicroseconds() / 1000);
		case str2int16("s"):
		default:
			return (int64_t)getTimeS(time, roundUp);
		case str2int16("S"):
			return getTimeString({}, {}, getTimeS(time, roundUp), {}, {}, {});
		case str2int16("Sf"):
		{
			float s = {}, f = {};
			s = getTimeSf(time, f);
			return getTimeString({}, {}, s, f, {}, 1);
		}
		case str2int16("Sff"):
		{
			float s = {}, f = {};
			s = getTimeSf(time, f);
			return getTimeString({}, {}, s, f, {}, 2);
		}
		case str2int16("Sfff"):
		{
			float s = {}, f = {};
			s = getTimeSf(time, f);
			return getTimeString({}, {}, s, f, {}, 3);
		}
		case str2int16("SS"):
			return getTimeString({}, {}, getTimeS(time, roundUp), {}, 2, {});
		case str2int16("SSf"):
		{
			float s = {}, f = {};
			s = getTimeSf(time, f);
			return getTimeString({}, {}, s, f, 2, 1);
		}
		case str2int16("SSff"):
		{
			float s = {}, f = {};
			s = getTimeSf(time, f);
			return getTimeString({}, {}, s, f, 2, 2);
		}
		case str2int16("SSfff"):
		{
			float s = {}, f = {};
			s = getTimeSf(time, f);
			return getTimeString({}, {}, s, f, 2, 3);
		}
		case str2int16("MSS"):
		{
			float m = {}, s = {};
			getTimeMS(time, roundUp, m, s);
			return getTimeString({}, m, s, {}, {}, {});
		}
		case str2int16("MSSf"):
		{
			float m = {}, s = {}, f = {};
			getTimeMSf(time, m, s, f);
			return getTimeString({}, m, s, f, {}, 1);
		}
		case str2int16("MSSff"):
		{
			float m = {}, s = {}, f = {};
			getTimeMSf(time, m, s, f);
			return getTimeString({}, m, s, f, {}, 2);
		}
		case str2int16("MSSfff"):
		{
			float m = {}, s = {}, f = {};
			getTimeMSf(time, m, s, f);
			return getTimeString({}, m, s, f, {}, 3);
		}
		case str2int16("MMSS"):
		{
			float m = {}, s = {};
			getTimeMS(time, roundUp, m, s);
			return getTimeString({}, m, s, {}, 2, {});
		}
		case str2int16("MMSSf"):
		{
			float m = {}, s = {}, f = {};
			getTimeMSf(time, m, s, f);
			return getTimeString({}, m, s, f, 2, 1);
		}
		case str2int16("MMSSff"):
		{
			float m = {}, s = {}, f = {};
			getTimeMSf(time, m, s, f);
			return getTimeString({}, m, s, f, 2, 2);
		}
		case str2int16("MMSSfff"):
		{
			float m = {}, s = {}, f = {};
			getTimeMSf(time, m, s, f);
			return getTimeString({}, m, s, f, 2, 3);
		}
		case str2int16("HMMSS"):
		{
			float h = {}, m = {}, s = {};
			getTimeHMS(time, roundUp, h, m, s);
			return getTimeString(h, m, s, {}, {}, {});
		}
		case str2int16("HMMSSf"):
		{
			float h = {}, m = {}, s = {}, f = {};
			getTimeHMSf(time, h, m, s, f);
			return getTimeString(h, m, s, f, {}, 1);
		}
		case str2int16("HMMSSff"):
		{
			float h = {}, m = {}, s = {}, f = {};
			getTimeHMSf(time, h, m, s, f);
			return getTimeString(h, m, s, f, {}, 2);
		}
		case str2int16("HMMSSfff"):
		{
			float h = {}, m = {}, s = {}, f = {};
			getTimeHMSf(time, h, m, s, f);
			return getTimeString(h, m, s, f, {}, 3);
		}
		case str2int16("HHMMSS"):
		{
			float h = {}, m = {}, s = {};
			getTimeHMS(time, roundUp, h, m, s);
			return getTimeString(h, m, s, {}, 2, {});
		}
		case str2int16("HHMMSSf"):
		{
			float h = {}, m = {}, s = {}, f = {};
			getTimeHMSf(time, h, m, s, f);
			return getTimeString(h, m, s, f, 2, 1);
		}
		case str2int16("HHMMSSff"):
		{
			float h = {}, m = {}, s = {}, f = {};
			getTimeHMSf(time, h, m, s, f);
			return getTimeString(h, m, s, f, 2, 2);
		}
		case str2int16("HHMMSSfff"):
		{
			float h = {}, m = {}, s = {}, f = {};
			getTimeHMSf(time, h, m, s, f);
			return getTimeString(h, m, s, f, 2, 3);
		}
		}
	}

	std::string replaceStringWithFunction(const std::string_view str, char token,
		const std::function<void(const std::string_view&, std::string&)> stringReplaceFunc)
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

			// %str%
			std::string_view strProp(str.data() + firstTokenStart, secondTokenStop - firstTokenStart);
			stringReplaceFunc(strProp, str2);

			firstTokenStart = secondTokenStart;
		}
		return str2;
	}

	std::string replaceStringWithQueryable(const std::string_view str,
		const Queryable& obj, char token)
	{
		return replaceStringWithFunction(str, token,
			[&str, &obj](const std::string_view& strProp, std::string& str2)
			{
				if (strProp.size() <= 2)
				{
					return;
				}
				Variable var;
				if (obj.getProperty(strProp.substr(1, strProp.size() - 2), var) == true)
				{
					Utils::replaceStringInPlace(str2, strProp, VarUtils::toString(var));
				}
			});
	}

	std::string replaceStringWithVarOrProp(const std::string_view str,
		const Game& obj, char token)
	{
		return replaceStringWithFunction(str, token,
			[&str, &obj](const std::string_view& strProp, std::string& str2)
			{
				if (strProp.size() <= 2)
				{
					return;
				}
				Variable var;
				if (obj.getVarOrPropNoToken(strProp.substr(1, strProp.size() - 2), var) == true)
				{
					Utils::replaceStringInPlace(str2, strProp, VarUtils::toString(var));
				}
			});
	}
}
