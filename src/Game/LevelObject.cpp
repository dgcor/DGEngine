#include "LevelObject.h"
#include "Game.h"
#include "Game/Level.h"

bool LevelObject::getLevelObjProp(const uint16_t propHash16,
	const std::string_view prop, Variable& var) const
{
	switch (propHash16)
	{
	case str2int16("class"):
	case str2int16("classId"):
		var = Variable(getClassId());
		break;
	case str2int16("id"):
		var = Variable(id);
		break;
	case str2int16("mapPosition"):
	{
		if (prop == "x")
		{
			var = Variable((int64_t)mapPosition.x);
		}
		else
		{
			var = Variable((int64_t)mapPosition.y);
		}
		break;
	}
	case str2int16("type"):
		var = Variable(std::string(getType()));
		break;
	default:
		return false;
	}
	return true;
}

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
		ti.absoluteOffset = false;
		ti.palette = sprite.getPalette();
		return true;
	}
	return false;
}

void LevelObject::updateDrawPosition(const LevelMap& map)
{
	updateDrawPosition(map, map.toDrawCoord(mapPosition));
}

void LevelObject::updateDrawPosition(const LevelMap& map, const sf::Vector2f& drawPos)
{
	tileBlockHeight = (float)map.DefaultBlockHeight();
	basePosition.x = drawPos.x + (float)map.DefaultBlockWidth();
	basePosition.y = drawPos.y + (float)map.DefaultBlockHeight();

	updateSpriteDrawPosition();
}

void LevelObject::updateSpriteDrawPosition()
{
	auto drawPosition = basePosition + drawPositionOffset;
	if (absoluteOffset == false)
	{
		const auto& textureRect = sprite.getTextureRect();
		drawPosition.x += -(float)(textureRect.width / 2);
		drawPosition.y += -((float)textureRect.height) + tileBlockHeight;
	}
	sprite.setPosition(drawPosition);
}

void LevelObject::updateHover(Game& game, Level& level)
{
	if (enableHover == false || level.EnableHover() == false)
	{
		return;
	}
	bool objHovered = level.HasMouseInside() == true;
	if (cellSize.x == 0 || cellSize.y == 0)
	{
		objHovered = objHovered &&
			(sprite.getGlobalBounds().contains(level.MousePositionf()) == true);
	}
	else
	{
		auto minMapPosition = mapPosition;
		auto maxMapPosition = mapPosition;
		if (cellSize.x < 0)
		{
			minMapPosition.x = std::max(0.f, mapPosition.x + (float)(cellSize.x + 1));
		}
		else
		{
			maxMapPosition.x = std::max(0.f, mapPosition.x + (float)(cellSize.x - 1));
		}
		if (cellSize.x < 0)
		{
			minMapPosition.y = std::max(0.f, mapPosition.y + (float)(cellSize.y + 1));
		}
		else
		{
			maxMapPosition.y = std::max(0.f, mapPosition.y + (float)(cellSize.y - 1));
		}

		auto mouseMapCoord = level.getMapCoordOverMouse();

		objHovered = objHovered &&
			(mouseMapCoord.x <= maxMapPosition.x &&
				mouseMapCoord.x >= minMapPosition.x &&
				mouseMapCoord.y <= maxMapPosition.y &&
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

bool LevelObject::MapPosition(LevelMap& map, const PairFloat& pos)
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

bool LevelObject::MapPosition(Level& level, const PairFloat& pos)
{
	return MapPosition(level.Map(), pos);
}

bool LevelObject::move(LevelMap& map, const PairFloat& pos)
{
	return MapPosition(map, pos);
}

bool LevelObject::move(Level& level, const PairFloat& pos)
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

bool LevelObject::updateMapPositionBack(LevelMap& map, const PairFloat pos)
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

bool LevelObject::updateMapPositionFront(LevelMap& map, const PairFloat pos)
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
		drawPositionOffset = ti.offset;
		absoluteOffset = ti.absoluteOffset;
		updateSpriteDrawPosition();
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
