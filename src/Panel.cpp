#include "Panel.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils/Utils.h"

void Panel::addDrawable(const std::shared_ptr<UIObject>& obj)
{
	drawables.push_back(obj);
	if (relativePositions == true)
	{
		obj->Position(obj->Position() + position);
	}
}

UIObject* Panel::getDrawable(size_t idx) const
{
	if (idx < drawables.size())
	{
		if (auto obj = drawables[idx].lock())
		{
			return obj.get();
		}
	}
	return nullptr;
}

void Panel::updateSize(const Game& game)
{
	auto pos = position;
	sf::Vector2f size;
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldDrawRegionSize(), game.DrawRegionSize());
	position = pos;
	for (const auto& drawable : drawables)
	{
		if (auto obj = drawable.lock())
		{
			obj->updateSize(game);
		}
	}
}

void Panel::Position(const sf::Vector2f& newPosition)
{
	for (const auto& drawable : drawables)
	{
		if (auto obj = drawable.lock())
		{
			obj->Position(obj->Position() - position + newPosition);
		}
	}
	position = newPosition;
}

void Panel::getDrawPositionAndSize(sf::Vector2f& drawPos, sf::Vector2f& drawSize) const
{
	drawPos = { std::numeric_limits<float>::max() , std::numeric_limits<float>::max() };
	drawSize = position;
	for (const auto& drawable : drawables)
	{
		if (auto obj = drawable.lock())
		{
			const auto& objDrawPos = obj->DrawPosition();
			if (objDrawPos.x < drawPos.x)
			{
				drawPos.x = objDrawPos.x;
			}
			if (objDrawPos.y < drawPos.y)
			{
				drawPos.y = objDrawPos.y;
			}
			auto posAndSize = objDrawPos + obj->Size();
			if (posAndSize.x > drawSize.x)
			{
				drawSize.x = posAndSize.x;
			}
			if (posAndSize.y > drawSize.y)
			{
				drawSize.y = posAndSize.y;
			}
		}
	}
	if (drawPos.x == std::numeric_limits<float>::max())
	{
		drawPos = position;
	}
	drawSize -= drawPos;
}

void Panel::draw(const Game& game, sf::RenderTarget& target,
	const sf::FloatRect& visibleRect) const
{
	if (visible == false)
	{
		return;
	}
	for (const auto& drawable : drawables)
	{
		if (auto obj = drawable.lock())
		{
			sf::FloatRect itemRect(obj->DrawPosition(), obj->Size());
			if (visibleRect.intersects(itemRect) == true)
			{
				obj->draw(game, target);
			}
		}
	}
}

void Panel::draw(const Game& game, sf::RenderTarget& target) const
{
	draw(game, target, { 0.f, 0.f, game.DrawRegionSizef().x, game.DrawRegionSizef().y });
}

void Panel::update(Game& game)
{
	if (visible == false)
	{
		return;
	}
	for (auto it = drawables.rbegin(); it != drawables.rend();)
	{
		auto obj = it->lock();
		if (obj == nullptr)
		{
			++it;
			it = std::reverse_iterator(drawables.erase(it.base()));
		}
		else
		{
			obj->update(game);
			++it;
		}
	}
}

bool Panel::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	switch (propHash)
	{
	case str2int16("itemCount"):
		var = Variable((int64_t)getItemCount());
		break;
	case str2int16("item"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto itemIdx = Utils::strtou(props2.first);
		if (itemIdx < drawables.size())
		{
			if (auto obj = drawables[itemIdx].lock())
			{
				return obj->getProperty(props2.second, var);
			}
		}
		return false;
	}
	default:
		return getUIObjProp(propHash, props.second, var);
	}
	return true;
}
