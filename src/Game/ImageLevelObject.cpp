#include "ImageLevelObject.h"
#include "Game.h"
#include "Game/Level.h"
#include "Utils.h"

using Utils::str2int;

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
	if (rect.contains(level.MousePosition()))
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
			level.setHoverObject(nullptr);
			level.executeHoverLeaveAction(game);
		}
	}
	if (rect.width > 0 && rect.height > 0)
	{
		auto drawPos = level.Map().getCoords(mapPosition);
		drawPos.x += (float)(level.getLevelX() - ((int)rect.width / 2)) + 32;
		drawPos.y += (float)(level.getLevelY() + 224 - ((int)rect.height - 32));
		sprite.setPosition(drawPos);
	}
}

bool ImageLevelObject::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	switch (str2int(prop.c_str()))
	{
	case str2int("type"):
		var = Variable("levelObject");
		break;
	case str2int("id"):
		var = Variable(id);
		break;
	case str2int("name"):
		var = Variable(name);
		break;
	default:
		return false;
	}
	return true;
}
