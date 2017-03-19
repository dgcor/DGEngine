#include "Item.h"
#include "Game.h"
#include "GameUtils.h"
#include "Game/Level.h"
#include "ItemProperties.h"
#include "Utils.h"

void Item::resetDropAnimation()
{
	currentFrame = 0;
	if (enableHover == true)
	{
		wasHoverEnabledOnItemDrop = true;
		enableHover = false;
	}
}

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
	if (enableHover == true)
	{
		if (level.HasMouseInside() == true &&
			mapPosition == level.getMapCoordOverMouse())
		{
			if (level.getClickedObject() == nullptr)
			{
				level.setClickedObject(this);
			}
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
		else
		{
			if (wasHoverEnabledOnItemDrop == true)
			{
				enableHover = true;
				wasHoverEnabledOnItemDrop = false;
			}
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
	auto drawPos = level.Map().getCoord(mapPosition);
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
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int32(props.first.c_str()))
	{
	case str2int32("shortName"):
		var = Variable(ShortName());
		break;
	case str2int32("name"):
		var = Variable(Name());
		break;
	case str2int32("simpleName"):
		var = Variable(SimpleName());
		break;
	case str2int32("d"):
	case str2int32("description"):
	{
		size_t idx = std::strtoul(props.second.c_str(), NULL, 10);
		if (idx >= descriptions.size())
		{
			idx = 0;
		}
		var = Variable(descriptions[idx]);
		break;
	}
	case str2int32("identified"):
		var = Variable((bool)identified);
		break;
	case str2int32("type"):
		var = Variable(std::string("item"));
		break;
	case str2int32("itemType"):
		var = Variable(ItemType());
		break;
	case str2int32("propertyCount"):
		var = Variable((int64_t)propertiesSize);
		break;
	case str2int32("hasProperty"):
		var = Variable(hasItemProperty(props.second));
		break;
	default:
	{
		int16_t value;
		if (getItemProperty(props.first.c_str(), value) == true)
		{
			var = Variable((int64_t)value);
			break;
		}
		else
		{
			return false;
		}
	}
	}
	return true;
}

bool Item::hasItemProperty(const char* prop) const
{
	if (propertiesSize > 0)
	{
		auto propHash = str2int16(prop);
		for (size_t i = 0; i < propertiesSize; i++)
		{
			if (properties[i].first == propHash)
			{
				return true;
			}
		}
	}
	return false;
}

int16_t Item::getItemPropertyByHash(uint16_t propHash) const
{
	int16_t value = 0;
	getItemPropertyByHash(propHash, value);
	return value;
}

int16_t Item::getItemProperty(const char* prop) const
{
	int16_t value = 0;
	getItemProperty(prop, value);
	return value;
}

bool Item::getItemPropertyByHash(uint16_t propHash, int16_t& value) const
{
	switch (propHash)
	{
	case str2int16("identified"):
		value = (int16_t)identified;
		break;
	case str2int16("indestructible"):
		value = (int16_t)(getItemPropertyByHash(ItemProp::DurabilityMax) == std::numeric_limits<int16_t>::max());
		break;
	case str2int16("unlimitedCharges"):
		value = (int16_t)(getItemPropertyByHash(ItemProp::ChargesMax) == std::numeric_limits<int16_t>::max());
		break;
	default:
	{
		if (propertiesSize > 0)
		{
			for (size_t i = 0; i < propertiesSize; i++)
			{
				if (properties[i].first == propHash)
				{
					value = properties[i].second;
					return true;
				}
			}
		}
		return false;
	}
	}
	return true;
}

bool Item::getItemProperty(const char* prop, int16_t& value) const
{
	if (propertiesSize > 0)
	{
		return getItemPropertyByHash(str2int16(prop), value);
	}
	return false;
}

void Item::setItemPropertyByHash(uint16_t propHash, int16_t value)
{
	switch (propHash)
	{
	case str2int16("identified"):
		identified = value != 0;
		return;
	default:
	{
		for (size_t i = 0; i < propertiesSize; i++)
		{
			if (properties[i].first == propHash)
			{
				properties[i].second = value;
				return;
			}
		}
		if (propertiesSize < properties.size())
		{
			properties[propertiesSize] = std::make_pair(propHash, value);
			propertiesSize++;
		}
	}
	}
}

void Item::setItemProperty(const char* prop, int16_t value)
{
	setItemPropertyByHash(str2int16(prop), value);
}

void Item::updateFullName()
{
	if (class_->getFullName(*this, name) == false)
	{
		name = class_->Name();
	}
}

void Item::updateDescriptions()
{
	for (size_t i = 0; i < descriptions.size(); i++)
	{
		class_->getDescription(i, *this, descriptions[i]);
	}
}

void Item::applyDefaults()
{
	for (const auto& prop : class_->Defaults())
	{
		setItemPropertyByHash(prop.first, prop.second);
	}
}
