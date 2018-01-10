#include "BaseLevelObject.h"
#include "Game.h"
#include "Game/Level.h"

void BaseLevelObject::checkAndUpdateTextureIndex()
{
	if (currentTextureIdx < textureStartIdx || currentTextureIdx >= textureEndIdx)
	{
		currentTextureIdx = textureStartIdx;
	}
}

bool BaseLevelObject::hasValidState() const
{
	return (texturePack == nullptr || textureStartIdx > textureEndIdx) == false;
}

sf::Vector2f BaseLevelObject::getBasePosition(const Level& level) const
{
	return sf::Vector2f(
		std::round(sprite.getPosition().x + (float)(sprite.getTextureRect().width / 2)),
		std::round(sprite.getPosition().y + (float)(sprite.getTextureRect().height - (level.TileHeight() / 2)))
	);
}

void BaseLevelObject::updateDrawPosition(const Level& level)
{
	updateDrawPosition(level, level.Map().getCoord(mapPosition));
}

void BaseLevelObject::updateDrawPosition(const Level& level, sf::Vector2f drawPos)
{
	const auto& texSize = sprite.getTextureRect();
	drawPos.x += std::round((float)(-((int)texSize.width / 2)) + (level.TileWidth() / 2));
	drawPos.y += std::round((float)(level.PillarHeight() - ((int)texSize.height)));
	sprite.setPosition(drawPos);
}

void BaseLevelObject::updateHover(Game& game, Level& level, LevelObject* levelObj)
{
	if (enableHover == false)
	{
		return;
	}
	bool objHovered = level.HasMouseInside() == true;
	if (hoverCellSize == 0)
	{
		objHovered = objHovered &&
			(sprite.getGlobalBounds().contains(level.MousePositionf()) == true);
	}
	else if (hoverCellSize == 1)
	{
		objHovered = objHovered &&
			(mapPosition == level.getMapCoordOverMouse());
	}
	else
	{
		auto minMapPosition = mapPosition;
		minMapPosition.x = (uint16_t)std::max(0, (int32_t)mapPosition.x - ((int32_t)hoverCellSize - 1));
		minMapPosition.y = (uint16_t)std::max(0, (int32_t)mapPosition.y - ((int32_t)hoverCellSize - 1));

		auto mouseMapCoord = level.getMapCoordOverMouse();

		objHovered = objHovered &&
			(mouseMapCoord.x <= mapPosition.x &&
				mouseMapCoord.x >= minMapPosition.x &&
				mouseMapCoord.y <= mapPosition.y &&
				mouseMapCoord.y >= minMapPosition.y);
	}
	if (objHovered == true)
	{
		if (level.getClickedObject() == nullptr)
		{
			level.setClickedObject(levelObj);
		}
		if (hovered == false)
		{
			hovered = true;
			if (outlineOnHover == true)
			{
				sprite.setOutlineEnabled(true);
			}
			level.setHoverObject(levelObj);
			level.executeHoverEnterAction(game);
		}
	}
	else
	{
		if (hovered == true)
		{
			hovered = false;
			if (outlineOnHover == true)
			{
				sprite.setOutlineEnabled(false);
			}
			if (level.getHoverObject() == levelObj)
			{
				level.setHoverObject(nullptr);
				level.executeHoverLeaveAction(game);
			}
		}
	}
}

std::shared_ptr<LevelObject> BaseLevelObject::updateMapPosition(
	Level& level, const MapCoord& pos, LevelObject* levelObj)
{
	auto oldObj = level.Map()[mapPosition].getObject(levelObj);
	level.Map()[mapPosition].deleteObject(levelObj);
	mapPosition = pos;
	return oldObj;
}

void BaseLevelObject::updateMapPositionBack(Level& level, const MapCoord& pos, LevelObject* levelObj)
{
	auto obj = updateMapPosition(level, pos, levelObj);
	level.Map()[mapPosition].addBack(obj);
}

void BaseLevelObject::updateMapPositionFront(Level& level, const MapCoord& pos, LevelObject* levelObj)
{
	auto obj = updateMapPosition(level, pos, levelObj);
	level.Map()[mapPosition].addFront(obj);
}

bool BaseLevelObject::updateTexture()
{
	const sf::Texture* texture;
	sf::IntRect rect;
	if (texturePack->get(currentTextureIdx, texturePackIdx, &texture, rect) == true)
	{
		sprite.setTexture(*texture, false);
		sprite.setTextureRect(rect);
		return true;
	}
	return false;
}

void BaseLevelObject::queueAction(const std::shared_ptr<Action>& action)
{
	if (action != nullptr)
	{
		queuedActions.push_back(action);
	}
}

void BaseLevelObject::queueAction(const BaseClass& class_, uint16_t nameHash16)
{
	auto action = class_.getAction(nameHash16);
	queueAction(action);
}

void BaseLevelObject::processQueuedActions(Game& game)
{
	for (auto it = queuedActions.begin(); it != queuedActions.end();)
	{
		game.Events().addBack(*it);
		it = queuedActions.erase(it);
	}
}
