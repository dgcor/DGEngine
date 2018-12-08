#include "LevelObject.h"
#include "Game.h"
#include "Game/Level.h"

bool LevelObject::hasValidState() const noexcept
{
	return (texturePack != nullptr);
}

bool LevelObject::getCurrentTexture(TextureInfo& ti) const
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

void LevelObject::updateDrawPosition(const LevelMap& map)
{
	updateDrawPosition(map, map.getCoord(mapPosition));
}

void LevelObject::updateDrawPosition(const LevelMap& map, const sf::Vector2f& drawPos)
{
	basePosition.x = drawPos.x + map.DefaultBlockWidth();
	basePosition.y = drawPos.y + map.DefaultBlockHeight();

	const auto& texSize = sprite.getTextureRect();

	originalPosition.x = basePosition.x - (texSize.width / 2);
	originalPosition.y = drawPos.y - texSize.height + map.DefaultTileHeight();

	sprite.setPosition(originalPosition + positionOffset);
}

void LevelObject::updateHover(Game& game, Level& level)
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
			level.setClickedObject(this);
		}
		if (hovered == false)
		{
			hovered = true;
			if (outlineOnHover == true)
			{
				sprite.setOutlineEnabled(true);
			}
			level.setHoverObject(this);
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
			if (level.getHoverObject() == this)
			{
				level.setHoverObject(nullptr);
				level.executeHoverLeaveAction(game);
			}
		}
	}
}

bool LevelObject::MapPosition(LevelMap& map, const MapCoord& pos)
{
	bool success = false;
	if (mapPosition != pos)
	{
		success = updateMapPositionFront(map, pos);
	}
	if (success == true || map.isMapCoordValid(pos) == true)
	{
		updateDrawPosition(map);
	}
	return success;
}

bool LevelObject::MapPosition(Level& level, const MapCoord& pos)
{
	return MapPosition(level.Map(), pos);
}

bool LevelObject::move(LevelMap& map, const MapCoord& pos)
{
	return MapPosition(map, pos);
}

bool LevelObject::move(Level& level, const MapCoord& pos)
{
	return move(level.Map(), pos);
}

bool LevelObject::remove(LevelMap& map) const
{
	bool success = false;
	if (map.isMapCoordValid(mapPosition) == true)
	{
		success = map[mapPosition].removeObject(this);
		if (success == true)
		{
			map.removeLight(mapPosition, getLightSource());
		}
	}
	return success;
}

bool LevelObject::updateMapPositionBack(LevelMap& map, const MapCoord pos)
{
	if (map.isMapCoordValid(mapPosition) == true &&
		map[mapPosition].removeObject(this) == true)
	{
		map.removeLight(mapPosition, getLightSource());
	}
	bool success = map.isMapCoordValid(pos);
	if (success == true)
	{
		map[pos].addBack(this);
		map.addLight(pos, getLightSource());
	}
	mapPosition = pos;
	return success;
}

bool LevelObject::updateMapPositionFront(LevelMap& map, const MapCoord pos)
{
	if (map.isMapCoordValid(mapPosition) == true &&
		map[mapPosition].removeObject(this) == true)
	{
		map.removeLight(mapPosition, getLightSource());
	}
	bool success = map.isMapCoordValid(pos);
	if (success == true)
	{
		map[pos].addFront(this);
		map.addLight(pos, getLightSource());
	}
	mapPosition = pos;
	return success;
}

bool LevelObject::updateTexture()
{
	TextureInfo ti;
	if (texturePack->get(animation.currentTextureIdx, ti) == true)
	{
		sprite.setTexture(ti, true);
		positionOffset = ti.offset;
		return true;
	}
	return false;
}

void LevelObject::queueAction(const std::shared_ptr<Action>& action)
{
	if (action != nullptr)
	{
		queuedActions.push_back(action);
	}
}

void LevelObject::queueAction(const LevelObjectClass& class_, uint16_t nameHash16)
{
	auto action = class_.getAction(nameHash16);
	queueAction(action);
}

void LevelObject::processQueuedActions(Game& game)
{
	for (auto it = queuedActions.begin(); it != queuedActions.end();)
	{
		game.Events().addBack(*it);
		it = queuedActions.erase(it);
	}
}
