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
	auto oldObj = level.Map()[mapPosition].getObject(this);
	level.Map()[mapPosition].deleteObject(this);
	mapPosition = pos;
	level.Map()[mapPosition].addFront(oldObj);
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
	auto propHash = str2int16(props.first.c_str());
	switch (propHash)
	{
	case str2int16("shortName"):
		var = Variable(ShortName());
		break;
	case str2int16("name"):
	{
		updateNameAndDescriptions();
		var = Variable(Name());
		break;
	}
	case str2int16("simpleName"):
		var = Variable(SimpleName());
		break;
	case str2int16("d"):
	case str2int16("description"):
	{
		updateNameAndDescriptions();
		size_t idx = std::strtoul(props.second.c_str(), NULL, 10);
		if (idx >= descriptions.size())
		{
			idx = 0;
		}
		var = Variable(descriptions[idx]);
		break;
	}
	case str2int16("hasDescription"):
	{
		updateNameAndDescriptions();
		bool hasDescr = false;
		size_t idx = std::strtoul(props.second.c_str(), NULL, 10);
		if (idx < descriptions.size())
		{
			hasDescr = descriptions[idx].empty() == false;
		}
		var = Variable((bool)hasDescr);
		break;
	}
	case str2int16("identified"):
		var = Variable((bool)identified);
		break;
	case str2int16("type"):
		var = Variable(std::string("item"));
		break;
	case str2int16("itemType"):
		var = Variable(ItemType());
		break;
	case str2int16("needsRecharge"):
		var = Variable(needsRecharge());
		break;
	case str2int16("needsRepair"):
		var = Variable(needsRepair());
		break;
	case str2int16("propertyCount"):
		var = Variable((int64_t)propertiesSize);
		break;
	case str2int16("hasProperty"):
		var = Variable(hasItemProperty(props.second));
		break;
	default:
	{
		LevelObjValue value;
		if (getItemPropertyByHash(propHash, value) == true)
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

void Item::setProperty(const std::string& prop, const Variable& val)
{
	if (prop.empty() == true)
	{
		return;
	}
	LevelObjValue val2 = 0;
	if (val.is<bool>() == true)
	{
		val2 = val.get<bool>() ? 1 : 0;
	}
	else if (val.is<int64_t>() == true)
	{
		val2 = (LevelObjValue)val.get<int64_t>();
	}
	setItemPropertyByHash(str2int16(prop.c_str()), val2);
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

LevelObjValue Item::getItemPropertyByHash(uint16_t propHash) const
{
	LevelObjValue value = 0;
	getItemPropertyByHash(propHash, value);
	return value;
}

LevelObjValue Item::getItemProperty(const char* prop) const
{
	LevelObjValue value = 0;
	getItemProperty(prop, value);
	return value;
}

bool Item::getItemPropertyByHash(uint16_t propHash, LevelObjValue& value) const
{
	switch (propHash)
	{
	case str2int16("identified"):
		value = (LevelObjValue)identified;
		break;
	case str2int16("indestructible"):
		value = (LevelObjValue)(getItemPropertyByHash(ItemProp::DurabilityMax)
			== std::numeric_limits<LevelObjValue>::max());
		break;
	case str2int16("unlimitedCharges"):
		value = (LevelObjValue)(getItemPropertyByHash(ItemProp::ChargesMax)
			== std::numeric_limits<LevelObjValue>::max());
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

bool Item::getItemProperty(const char* prop, LevelObjValue& value) const
{
	if (propertiesSize > 0)
	{
		return getItemPropertyByHash(str2int16(prop), value);
	}
	return false;
}

void Item::setItemPropertyByHash(uint16_t propHash, LevelObjValue value)
{
	switch (propHash)
	{
	case str2int16("identified"):
		identified = value != 0;
		break;
	default:
	{
		bool noUpdate = true;
		for (size_t i = 0; i < propertiesSize; i++)
		{
			if (properties[i].first == propHash)
			{
				properties[i].second = value;
				noUpdate = false;
				break;
			}
		}
		if (noUpdate == true &&
			propertiesSize < properties.size())
		{
			properties[propertiesSize] = std::make_pair(propHash, value);
			propertiesSize++;
			noUpdate = false;
		}
		if (noUpdate == true)
		{
			return;
		}
	}
	}
	updateNameAndDescr = true;
}

void Item::setItemProperty(const char* prop, LevelObjValue value)
{
	setItemPropertyByHash(str2int16(prop), value);
}

void Item::updateNameAndDescriptions() const
{
	if (updateNameAndDescr == true)
	{
		updateNameAndDescr = false;
		if (identified == false)
		{
			name = SimpleName();
		}
		else
		{
			if (class_->getFullName(*this, name) == false)
			{
				name = SimpleName();
			}
		}
		for (size_t i = 0; i < descriptions.size(); i++)
		{
			class_->getDescription(i, *this, descriptions[i]);
		}
	}
}

void Item::applyDefaults()
{
	for (const auto& prop : class_->Defaults())
	{
		setItemPropertyByHash(prop.first, prop.second);
	}
}

bool Item::needsRecharge() const
{
	LevelObjValue charges = 0;
	if (getItemPropertyByHash(ItemProp::ChargesMax, charges) == true)
	{
		if (charges < std::numeric_limits<LevelObjValue>::max())
		{
			return getItemPropertyByHash(ItemProp::Charges) < charges;
		}
	}
	return false;
}

bool Item::needsRepair() const
{
	LevelObjValue durability = 0;
	if (getItemPropertyByHash(ItemProp::DurabilityMax, durability) == true)
	{
		if (durability < std::numeric_limits<LevelObjValue>::max())
		{
			return getItemPropertyByHash(ItemProp::Durability) < durability;
		}
	}
	return false;
}
