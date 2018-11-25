#include "BaseLevelObject.h"
#include "Game.h"
#include "Game/Level.h"

bool BaseLevelObject::hasValidState() const noexcept
{
	return (texturePack != nullptr);
}

bool BaseLevelObject::getTexture(TextureInfo& ti) const
{
	if (sprite.getTexture() != nullptr)
	{
		ti.texture = sprite.getTexture();
		ti.textureRect = sprite.getTextureRect();
		ti.offset = {};
		ti.palette = sprite.getPalette();
		return true;
	}
	return false;
}

void BaseLevelObject::updateDrawPosition(const Level& level)
{
	updateDrawPosition(level, level.Map().getCoord(mapPosition));
}

void BaseLevelObject::updateDrawPosition(const Level& level, const sf::Vector2f& drawPos)
{
	basePosition.x = drawPos.x + (level.TileWidth() / 2);
	basePosition.y = drawPos.y + (level.TileHeight() / 2);

	const auto& texSize = sprite.getTextureRect();

	originalPosition.x = basePosition.x - (texSize.width / 2);
	originalPosition.y = drawPos.y - texSize.height + level.TileHeight();

	sprite.setPosition(originalPosition + offset);
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

void BaseLevelObject::updateMapPositionBack(Level& level, const MapCoord pos,
	LevelObject* levelObj)
{
	if (level.Map().isMapCoordValid(mapPosition) == true)
	{
		level.Map()[mapPosition].removeObject(levelObj);
	}
	if (level.Map().isMapCoordValid(pos) == true)
	{
		level.Map()[pos].addBack(levelObj);
	}
	mapPosition = pos;
}

void BaseLevelObject::updateMapPositionFront(Level& level, const MapCoord pos,
	LevelObject* levelObj)
{
	if (level.Map().isMapCoordValid(mapPosition) == true)
	{
		level.Map()[mapPosition].removeObject(levelObj);
	}
	if (level.Map().isMapCoordValid(pos) == true)
	{
		level.Map()[pos].addFront(levelObj);
	}
	mapPosition = pos;
}

bool BaseLevelObject::updateTexture()
{
	TextureInfo ti;
	if (texturePack->get(animation.currentTextureIdx, ti) == true)
	{
		sprite.setTexture(ti, true);
		offset = ti.offset;
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

void BaseLevelObject::queueAction(const BaseClassActions& class_, uint16_t nameHash16)
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
