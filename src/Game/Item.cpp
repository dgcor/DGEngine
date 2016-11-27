#include "Item.h"
#include "Game.h"
#include "GameUtils.h"
#include "Game/Level.h"
#include "Utils.h"

using Utils::str2int;

void Item::executeAction(Game& game) const
{
	auto& action = class_->getAction();
	if (action != nullptr)
	{
		game.Events().addBack(action);
	}
}

void Item::MapPosition(Level& level, const MapCoord& pos)
{
	auto oldObj = level.Map()[mapPosition.x][mapPosition.y].object;
	level.Map()[mapPosition.x][mapPosition.y].object = nullptr;
	mapPosition = pos;
	level.Map()[mapPosition.x][mapPosition.y].object = oldObj;
}

void Item::update(Game& game, Level& level)
{
	auto rect = sprite.getGlobalBounds();
	if (enableHover == true)
	{
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
					level.executeHoverLeaveAction(game);
				}
			}
		}
	}

	if (frameRange.first > frameRange.second)
	{
		return;
	}

	// add delta time
	currentTime += game.getElapsedTime();

	// if current time is bigger then the frame time advance one frame
	if (currentTime >= frameTime)
	{
		// reset time, but keep the remainder
		currentTime = sf::microseconds(currentTime.asMicroseconds() % frameTime.asMicroseconds());

		if (currentFrame < frameRange.first)
		{
			currentFrame = frameRange.first;
		}
		else if (currentFrame < frameRange.second)
		{
			currentFrame++;
		}

		if (currentFrame < class_->getCelDropTextureSize())
		{
			sprite.setTexture(class_->getCelDropTexture(currentFrame), true);

			updateDrawPosition(level);
		}
	}
}

void Item::updateDrawPosition(const Level& level)
{
	const auto& texSize = sprite.getTextureRect();
	auto drawPos = level.Map().getCoords(mapPosition);
	drawPos.x += (float)(-((int)texSize.width / 2)) + LevelMap::TileSize();
	drawPos.y += (float)(224 - ((int)texSize.height - LevelMap::TileSize()));
	sprite.setPosition(drawPos);
}

bool Item::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	switch (str2int(prop.c_str()))
	{
	case str2int("name"):
		var = Variable(name);
		break;
	case str2int("description1"):
		var = Variable(description1);
		break;
	case str2int("description2"):
		var = Variable(description2);
		break;
	case str2int("description3"):
		var = Variable(description3);
		break;
	case str2int("type"):
		var = Variable(class_->Type());
		break;
	default:
		return false;
	}
	return true;
}
