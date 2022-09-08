#include "LevelDrawableManager.h"
#include "Game/Drawables/Panel.h"
#include "Game/Player/Player.h"
#include "Level.h"

void LevelDrawableManager::setPosition(const Level& level, LevelDrawable& obj, Panel& panelObj)
{
	if (auto anchorTo = obj.anchorTo.lock())
	{
		auto& drawPos = panelObj.DrawPosition();
		auto drawSize = panelObj.Size();
		sf::Vector2f newPos;
		sf::FloatRect visibleRect;

		if (level.zoomDrawables == true)
		{
			newPos = anchorTo->getAnchorPosition() + obj.offset;
			if (level.surface.getZoom() > 1.f)
			{
				newPos.y += std::round((drawSize.y / level.surface.getZoom()) - drawSize.y);
			}
			visibleRect = level.surface.visibleRect;
		}
		else
		{
			newPos = level.surface.getDrawPosition(anchorTo->getAnchorPosition() + obj.offset);
			newPos.y += std::round((drawSize.y / level.surface.getZoom()) - drawSize.y);
			visibleRect = sf::FloatRect(level.surface.Position(), level.surface.Size());
		}

		if (obj.visibleRectOffset >= 0.f)
		{
			auto drawPosOffset = drawPos - panelObj.Position();
			auto newDrawPos = newPos + drawPosOffset;

			if (newDrawPos.x < visibleRect.left + obj.visibleRectOffset)
			{
				newPos.x = visibleRect.left + obj.visibleRectOffset - drawPosOffset.x;
			}
			if (newDrawPos.y < visibleRect.top + obj.visibleRectOffset)
			{
				newPos.y = visibleRect.top + obj.visibleRectOffset - drawPosOffset.y;
			}
			newDrawPos += drawSize;
			if (newDrawPos.x > visibleRect.left +
				visibleRect.width - obj.visibleRectOffset)
			{
				newPos.x = visibleRect.left + visibleRect.width
					- obj.visibleRectOffset - drawSize.x - drawPosOffset.x;
			}
			if (newDrawPos.y > visibleRect.top +
				visibleRect.height - obj.visibleRectOffset)
			{
				newPos.y = visibleRect.top + visibleRect.height
					- obj.visibleRectOffset - drawSize.y - drawPosOffset.y;
			}
		}
		panelObj.Position(newPos);
	}
}

void LevelDrawableManager::add(const Level& level, LevelDrawable obj)
{
	for (const auto& drawable : drawables)
	{
		if (drawable.id == obj.id)
		{
			return;
		}
	}
	if (auto drawableObj = obj.drawable.lock())
	{
		setPosition(level, obj, *drawableObj);
	}
	drawables.push_back(obj);
}

LevelDrawable* LevelDrawableManager::get(const std::string_view id)
{
	for (auto& obj : drawables)
	{
		if (obj.id == id)
		{
			return &obj;
		}
	}
	return nullptr;
}

std::shared_ptr<Panel> LevelDrawableManager::getPanel(size_t idx) const
{
	if (idx < drawables.size())
	{
		return drawables[idx].drawable.lock();
	}
	return nullptr;
}

void LevelDrawableManager::update(const Level& level, Game& game)
{
	for (auto it = drawables.rbegin(); it != drawables.rend();)
	{
		auto obj = it->drawable.lock();
		if (obj == nullptr)
		{
			++it;
			it = std::reverse_iterator(drawables.erase(it.base()));
		}
		else
		{
			setPosition(level , *it, *obj);
			obj->update(game);
			++it;
		}
	}
}
