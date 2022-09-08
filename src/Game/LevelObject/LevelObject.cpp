#include "LevelObject.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/Utils/UIObjectUtils.h"

bool LevelObject::getLevelObjProp(const uint16_t propHash16, const std::string_view prop, Variable& var) const
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
	case str2int16("light"):
		var = Variable((int64_t)lightSource.light);
		break;
	case str2int16("lightRadius"):
		var = Variable((int64_t)lightSource.radius);
		break;
	case str2int16("mapPosition"):
		var = UIObjectUtils::getTuple2iProp(mapPosition, prop);
		break;
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
	return (animation.holdsNullTexturePack() == false);
}

bool LevelObject::getCurrentTexture(TextureInfo& ti) const
{
	// only returns the first layer of composite textures
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
	updateDrawPosition(map, mapPosition);
}

void LevelObject::updateDrawPosition(const LevelMap& map, const PairFloat& mapPos)
{
	tileBlockHeight = (float)map.DefaultBlockHeight();
	auto drawPos = map.toDrawCoord(mapPos);
	basePosition.x = drawPos.x + (float)map.DefaultBlockWidth();
	basePosition.y = drawPos.y + (float)map.DefaultBlockHeight();

	PairFloat centerPosition = getCenterMapPosition(mapPos);
	anchorPosition = map.toDrawCoord(centerPosition) + class_->getAnchorOffset();
	anchorPosition.x += (float)map.DefaultBlockWidth();
	anchorPosition.y += (float)map.DefaultBlockHeight();

	updateSpriteDrawPosition();
}

void LevelObject::updateSpriteDrawPosition()
{
	auto drawPosition = basePosition;
	if (absoluteOffset == false)
	{
		const auto& textureRect = sprite.getTextureRect();
		drawPosition.x += -(float)(textureRect.width / 2);
		drawPosition.y += -((float)textureRect.height) + tileBlockHeight;
	}
	sprite.setPosition(drawPosition);
}

PairFloat LevelObject::getCenterMapPosition(const PairFloat& mapPos)
{
	PairFloat minMapPosition;
	PairFloat maxMapPosition;
	getMinMaxMapPosition(mapPos, minMapPosition, maxMapPosition);

	minMapPosition.x = (minMapPosition.x + maxMapPosition.x) / 2.f;
	minMapPosition.y = (minMapPosition.y + maxMapPosition.y) / 2.f;

	return minMapPosition;
}

void LevelObject::getMinMaxMapPosition(const PairFloat& mapPos, PairFloat& minMapPos, PairFloat& maxMapPos)
{
	minMapPos = mapPos;
	maxMapPos = mapPos;
	if (cellSize.x < 0)
	{
		minMapPos.x = std::max(0.f, mapPos.x + (float)(cellSize.x + 1));
	}
	else
	{
		maxMapPos.x = std::max(0.f, mapPos.x + (float)(cellSize.x - 1));
	}
	if (cellSize.x < 0)
	{
		minMapPos.y = std::max(0.f, mapPos.y + (float)(cellSize.y + 1));
	}
	else
	{
		maxMapPos.y = std::max(0.f, mapPos.y + (float)(cellSize.y - 1));
	}
}

void LevelObject::updateHover(Game& game, Level& level, const std::shared_ptr<LevelObject>& thisPtr)
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
		PairFloat minMapPosition;
		PairFloat maxMapPosition;
		getMinMaxMapPosition(mapPosition, minMapPosition, maxMapPosition);

		auto mouseMapCoord = level.getMapCoordOverMouse();

		objHovered = objHovered &&
			(mouseMapCoord.x <= maxMapPosition.x &&
				mouseMapCoord.x >= minMapPosition.x &&
				mouseMapCoord.y <= maxMapPosition.y &&
				mouseMapCoord.y >= minMapPosition.y);
	}
	if (objHovered == true)
	{
		if (level.LevelObjects().hasClickedObject() == false)
		{
			level.LevelObjects().ClickedObject(thisPtr);
		}
		if (hovered == false)
		{
			hovered = true;
			if (outlineOnHover == true)
			{
				sprite.setOutlineEnabled(true);
			}
			level.LevelObjects().HoverObject(thisPtr);
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
			if (level.LevelObjects().HoverObject().get() == this)
			{
				level.LevelObjects().clearHoverObject();
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

bool LevelObject::move(LevelMap& map, const PairFloat& pos)
{
	return MapPosition(map, pos);
}

bool LevelObject::remove(LevelMap& map) const
{
	if (map.isMapCoordValid(mapPosition) == true)
	{
		return map[mapPosition].removeObject(this);
	}
	return false;
}

bool LevelObject::updateMapPositionBack(LevelMap& map, const PairFloat pos)
{
	if (map.isMapCoordValid(mapPosition) == true)
	{
		map[mapPosition].removeObject(this);
	}
	bool success = map.isMapCoordValid(pos);
	if (success == true)
	{
		map[pos].addBack(this);
	}
	mapPosition = pos;
	return success;
}

bool LevelObject::updateMapPositionFront(LevelMap& map, const PairFloat pos)
{
	if (map.isMapCoordValid(mapPosition) == true)
	{
		map[mapPosition].removeObject(this);
	}
	bool success = map.isMapCoordValid(pos);
	if (success == true)
	{
		map[pos].addFront(this);
	}
	mapPosition = pos;
	return success;
}

bool LevelObject::updateTexture()
{
	if (animation.updateTexture(sprite, absoluteOffset) == true)
	{
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
	queueAction(class_.getAction(nameHash16));
}

void LevelObject::processQueuedActions(Game& game)
{
	for (auto it = queuedActions.begin(); it != queuedActions.end();)
	{
		game.Events().addBack(*it);
		it = queuedActions.erase(it);
	}
}

void LevelObject::executeAction(Game& game) const
{
	class_->executeAction(game, str2int16("action"));
}

bool LevelObject::getTexture(uint32_t textureNumber, TextureInfo& ti) const
{
	switch (textureNumber)
	{
	case 0:
		return getCurrentTexture(ti);
	default:
		return false;
	}
}
