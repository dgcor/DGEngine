#include "ImageLevelObject.h"
#include "Game.h"
#include "Game/Level.h"
#include "Utils.h"

void ImageLevelObject::executeAction(Game& game) const
{
	if (action != nullptr)
	{
		game.Events().addBack(action);
	}
}

void ImageLevelObject::update(Game& game, Level& level)
{
	auto rect = sprite.getGlobalBounds();
	if (level.HasMouseInside() == true &&
		rect.contains(level.MousePositionf()) == true)
	{
		if (hovered == false)
		{
			hovered = true;
			level.setHoverObject(this);
			level.executeHoverEnterAction(game);
		}
	}
	else
	{
		if (hovered == true)
		{
			hovered = false;
			if (level.getHoverObject() == this)
			{
				level.setHoverObject(nullptr);
			}
			level.executeHoverLeaveAction(game);
		}
	}
	if (rect.width > 0 && rect.height > 0)
	{
		auto drawPos = level.Map().getCoord(mapPosition);
		drawPos.x += (float)(- ((int)rect.width / 2)) + 32;
		drawPos.y += (float)(224 - ((int)rect.height - 32));
		sprite.setPosition(drawPos);
	}
}

bool ImageLevelObject::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	switch (str2int32(prop.c_str()))
	{
	case str2int32("type"):
		var = Variable(std::string("levelObject"));
		break;
	case str2int32("id"):
		var = Variable(id);
		break;
	case str2int32("name"):
		var = Variable(name);
		break;
	default:
		return false;
	}
	return true;
}
