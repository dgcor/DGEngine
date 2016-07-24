#include "GameUtils.h"
#include "Game.h"
#include "Utils.h"

namespace GameUtils
{
	using Utils::str2int;

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

	HorizontalAlign getHorizontalAlignment(const std::string& align)
	{
		if (align == "center")
		{
			return HorizontalAlign::Center;
		}
		else if (align == "right")
		{
			return HorizontalAlign::Right;
		}
		else
		{
			return HorizontalAlign::Left;
		}
	}

	VerticalAlign getVerticalAlignment(const std::string& align)
	{
		if (align == "center")
		{
			return VerticalAlign::Center;
		}
		else if (align == "top")
		{
			return VerticalAlign::Top;
		}
		else
		{
			return VerticalAlign::Bottom;
		}
	}

	bool getUIObjProp(const UIObject& uiObject, const unsigned int propHash,
		const std::vector<std::string>& props, Variable& var)
	{
		switch (propHash)
		{
		case str2int("position"):
		{
			if (props.size() > 1)
			{
				auto movePos = uiObject.Position();
				if (props[1] == "x")
				{
					var = Variable((int64_t)movePos.x);
				}
				else
				{
					var = Variable((int64_t)movePos.y);
				}
				return true;
			}
		}
		break;
		case str2int("size"):
		{
			if (props.size() > 1)
			{
				auto moveSize = uiObject.Size();
				if (props[1] == "x")
				{
					var = Variable((int64_t)moveSize.x);
				}
				else
				{
					var = Variable((int64_t)moveSize.y);
				}
				return true;
			}
		}
		break;
		}
		return false;
	}

	bool getObjectProperty(const Game& game, const std::string& str, Variable& var)
	{
		if ((str.size() > 2) && (str.front() == '|') && (str.back() == '|'))
		{
			auto propStr = str.substr(1, str.size() - 2);

			if (propStr.size() > 1)
			{
				auto props = Utils::splitString(propStr, '|');
				if (props.size() > 1)
				{
					const auto& id = props[0];
					const auto& uiElemProps = props[1];

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
			}
		}
		return false;
	}
}
