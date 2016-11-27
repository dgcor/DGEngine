#include "GameUtils.h"
#include "Game.h"
#include "Utils.h"

namespace GameUtils
{
	using namespace Utils;

	sf::Vector2f getAlignmentPosition(const sf::Vector2f& position,
		const sf::Vector2f& size,
		HorizontalAlign horizAlign,
		VerticalAlign vertAlign)
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
		const sf::Vector2f& srcPos, const sf::Vector2f& anchorToSize)
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
		const sf::Vector2u& oldWindowSize, const sf::Vector2u& newWindowSize)
	{
		if (((int)anchor & (int)Anchor::Top) != 0)
		{
			if (((int)anchor & (int)Anchor::Bottom) != 0)
			{
				size.y += std::round((float)newWindowSize.y - (float)oldWindowSize.y);
			}
		}
		else
		{
			if (((int)anchor & (int)Anchor::Bottom) != 0)
			{
				pos.y += std::round((float)newWindowSize.y - (float)oldWindowSize.y);
			}
			else
			{
				pos.y += std::round(((float)newWindowSize.y - (float)oldWindowSize.y) / 2.f);
			}
		}
		if (((int)anchor & (int)Anchor::Left) != 0)
		{
			if (((int)anchor & (int)Anchor::Right) != 0)
			{
				size.x += std::round((float)newWindowSize.x - (float)oldWindowSize.x);
			}
		}
		else
		{
			if (((int)anchor & (int)Anchor::Right) != 0)
			{
				pos.x += std::round((float)newWindowSize.x - (float)oldWindowSize.x);
			}
			else
			{
				pos.x += std::round(((float)newWindowSize.x - (float)oldWindowSize.x) / 2.f);
			}
		}
	}

	Anchor getAnchor(const std::string& str, Anchor val)
	{
		switch (str2int(toLower(str).c_str()))
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

	HorizontalAlign getHorizontalAlignment(const std::string& str)
	{
		switch (str2int(toLower(str).c_str()))
		{
		case str2int("left"):
		default:
			return HorizontalAlign::Left;
		case str2int("center"):
			return HorizontalAlign::Center;
		case str2int("right"):
			return HorizontalAlign::Right;
		}
	}

	VerticalAlign getVerticalAlignment(const std::string& str)
	{
		switch (str2int(toLower(str).c_str()))
		{
		case str2int("bottom"):
		default:
			return VerticalAlign::Bottom;
		case str2int("center"):
			return VerticalAlign::Center;
		case str2int("top"):
			return VerticalAlign::Top;
		}
	}

	sf::Keyboard::Key getKeyCode(int num, sf::Keyboard::Key val)
	{
		if (num >= sf::Keyboard::Unknown && num < sf::Keyboard::KeyCount)
		{
			return static_cast<sf::Keyboard::Key>(num);
		}
		return val;
	}

	sf::Keyboard::Key getKeyCode(const std::string& str, sf::Keyboard::Key val)
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
		return val;
	}

	IgnoreResource getIgnoreResource(const std::string& str, IgnoreResource val)
	{
		switch (str2int(toLower(str).c_str()))
		{
		case str2int("none"):
			return IgnoreResource::None;
		case str2int("drawandupdate"):
			return IgnoreResource::DrawAndUpdate;
		case str2int("update"):
			return IgnoreResource::Update;
		}
		return val;
	}

	PlayerDirection getPlayerDirection(const std::string& str, PlayerDirection val)
	{
		switch (str2int(toLower(str).c_str()))
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

	PlayerInventory getPlayerInventory(const std::string& str, PlayerInventory val)
	{
		switch (str2int(toLower(str).c_str()))
		{
		case str2int("body"):
			return PlayerInventory::Body;
		case str2int("belt"):
			return PlayerInventory::Belt;
		case str2int("stash"):
			return PlayerInventory::Stash;
		default:
			return val;
		}
	}

	size_t getPlayerInventoryIndex(const std::string& str, PlayerInventory val)
	{
		try
		{
			return std::stoul(str);
		}
		catch (std::exception ex) {}

		return (size_t)getPlayerInventory(str, val);
	}

	PlayerItemMount getPlayerItemMount(const std::string& str, PlayerItemMount val)
	{
		switch (str2int(toLower(str).c_str()))
		{
		case str2int("lefthand"):
			return PlayerItemMount::LeftHand;
		case str2int("righthand"):
			return PlayerItemMount::RightHand;
		case str2int("armor"):
			return PlayerItemMount::Armor;
		case str2int("helmet"):
			return PlayerItemMount::Helmet;
		case str2int("amulet"):
			return PlayerItemMount::Amulet;
		case str2int("leftring"):
			return PlayerItemMount::LeftRing;
		case str2int("rightring"):
			return PlayerItemMount::RightRing;
		default:
			return val;
		}
	}

	size_t getPlayerItemMountIndex(const std::string& str,
		PlayerItemMount val)
	{
		try
		{
			return std::stoul(str);
		}
		catch (std::exception ex) {}

		return (size_t)getPlayerItemMount(str, val);
	}

	PlayerStatus getPlayerStatus(const std::string& str, PlayerStatus val)
	{
		switch (str2int(toLower(str).c_str()))
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

	bool getUIObjProp(const UIObject& uiObject, const unsigned int propHash,
		const std::string& prop, Variable& var)
	{
		switch (propHash)
		{
		case str2int("position"):
		{
			auto movePos = uiObject.Position();
			if (prop == "x")
			{
				var = Variable((int64_t)movePos.x);
			}
			else
			{
				var = Variable((int64_t)movePos.y);
			}
		}
		break;
		case str2int("size"):
		{
			auto moveSize = uiObject.Size();
			if (prop == "x")
			{
				var = Variable((int64_t)moveSize.x);
			}
			else
			{
				var = Variable((int64_t)moveSize.y);
			}
		}
		break;
		default:
			return false;
		}
		return true;
	}

	bool getObjectProperty(const Game& game, const std::string& str, Variable& var)
	{
		if ((str.size() > 3)
			&& (str.front() == '|')
			&& (str.back() == '|'))
		{
			auto propStr = str.substr(1, str.size() - 2);
			auto props = splitStringIn2(propStr, '|');
			const auto& id = props.first;
			const auto& uiElemProps = props.second;

			if (id == "game")
			{
				return game.getProperty(uiElemProps, var);
			}
			const UIObject* uiObject = game.Resources().getDrawable(id);
			if (uiObject == nullptr)
			{
				if (id == "focus")
				{
					uiObject = game.Resources().getFocused();
				}
				else if (id == "currentLevel")
				{
					uiObject = game.Resources().getCurrentLevel();
				}
			}
			if (uiObject != nullptr)
			{
				return uiObject->getProperty(uiElemProps, var);
			}
		}
		return false;
	}
}
