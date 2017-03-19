#include "Player.h"
#include <cstdlib>
#include "Game.h"
#include "GameUtils.h"
#include "ItemProperties.h"
#include "Level.h"
#include "Utils.h"

sf::Vector2f Player::getBasePosition() const
{
	return sf::Vector2f(
		std::round(sprite.getPosition().x + (float)(sprite.getTextureRect().width / 2)),
		std::round(sprite.getPosition().y + (float)(sprite.getTextureRect().height - LevelMap::TileSize() / 2)));
}

void Player::executeAction(Game& game) const
{
	if (action != nullptr)
	{
		game.Events().addBack(action);
	}
}

void Player::MapPosition(Level& level, const MapCoord& pos)
{
	auto oldObj = level.Map()[mapPosition.x][mapPosition.y].object;
	level.Map()[mapPosition.x][mapPosition.y].object = nullptr;
	mapPosition = pos;
	level.Map()[mapPosition.x][mapPosition.y].object = oldObj;
}

void Player::updateWalkPath(Game& game, Level& level, const sf::Vector2u& texSize)
{
	if (walkPath.empty() == true)
	{
		setStatus(PlayerStatus::Stand1);
	}
	else
	{
		setStatus(PlayerStatus::Walk1);
	}

	if (walkPath.empty() == false)
	{
		const auto& nextMapPos = walkPath.front();
		if (walkPath.size() == 1)
		{
			const auto levelObj = level.Map()[nextMapPos.x][nextMapPos.y].object;
			if (levelObj != nullptr)
			{
				levelObj->executeAction(game);
				walkPath.pop();
				return;
			}
		}

		setDirection(getPlayerDirection(mapPosition, nextMapPos));
		MapPosition(level, nextMapPos);
		walkPath.pop();
	}

	auto drawPos = level.Map().getCoord(mapPosition);
	drawPos.x += (float)(-((int)texSize.x / 2)) + LevelMap::TileSize();
	drawPos.y += (float)(224 - ((int)texSize.y - LevelMap::TileSize()));
	sprite.setPosition(drawPos);
}

void Player::update(Game& game, Level& level)
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
	if (rect.width > 0 && rect.height > 0)
	{
		updateWalkPath(game, level, sf::Vector2u((unsigned)rect.width, (unsigned)rect.height));
	}

	if (celTexture == nullptr
		|| frameRange.first > frameRange.second)
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

		currentFrame++;
		if (currentFrame < frameRange.first || currentFrame >= frameRange.second)
		{
			currentFrame = frameRange.first;
		}

		if (currentFrame < celTexture->size(celIdx))
		{
			sprite.setTexture(celTexture->get(celIdx, currentFrame), true);
		}
	}
}

bool Player::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int32(props.first.c_str()))
	{
	case str2int32("type"):
		var = Variable(std::string("player"));
		break;
	case str2int32("id"):
		var = Variable(id);
		break;
	case str2int32("name"):
		var = Variable(name);
		break;
	case str2int32("class"):
		var = Variable(class_->Name());
		break;
	case str2int32("level"):
		var = Variable((int64_t)currentLevel);
		break;
	case str2int32("experience"):
		var = Variable((int64_t)experience);
		break;
	case str2int32("expNextLevel"):
		var = Variable((int64_t)expNextLevel);
		break;
	case str2int32("points"):
		var = Variable((int64_t)points);
		break;
	case str2int32("gold"):
		var = Variable((int64_t)gold);
		break;
	case str2int32("canEquipSelectedItem"):
	{
		if (selectedItem == nullptr)
		{
			return false;
		}
		var = Variable(canEquipItem(*selectedItem));
		break;
	}
	case str2int32("canEquipItem"):
	{
		std::string props2;
		size_t invIdx;
		size_t itemIdx;
		if (parseInventoryAndItem(props.second, props2, invIdx, itemIdx) == true)
		{
			if (inventories[invIdx][itemIdx] != nullptr)
			{
				var = Variable(canEquipItem(*inventories[invIdx][itemIdx]));
				break;
			}
		}
		return false;
	}
	case str2int32("hasSelectedItem"):
		var = Variable(selectedItem != nullptr);
		break;
	case str2int32("hasItem"):
	{
		std::string props2;
		size_t invIdx;
		size_t itemIdx;
		if (parseInventoryAndItem(props.second, props2, invIdx, itemIdx) == true)
		{
			var = Variable(inventories[invIdx][itemIdx] != nullptr);
			break;
		}
		return false;
	}
	case str2int32("isItemSlotInUse"):
	{
		std::string props2;
		size_t invIdx;
		size_t itemIdx;
		if (parseInventoryAndItem(props.second, props2, invIdx, itemIdx) == true)
		{
			var = Variable(inventories[invIdx].isItemSlotInUse(itemIdx));
			break;
		}
		return false;
	}
	case str2int32("selectedItem"):
	{
		if (selectedItem != nullptr)
		{
			return selectedItem->getProperty(props.second, var);
		}
		return false;
	}
	break;
	case str2int32("item"):
	{
		std::string props2;
		size_t invIdx;
		size_t itemIdx;
		if (parseInventoryAndItem(props.second, props2, invIdx, itemIdx) == true)
		{
			auto item = inventories[invIdx][itemIdx].get();
			if (item != nullptr)
			{
				return item->getProperty(props2, var);
			}
		}
		return false;
	}
	case str2int32("inventory"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto invIdx = GameUtils::getPlayerInventoryIndex(props2.first);
		if (invIdx < inventories.size())
		{
			return inventories[invIdx].getProperty(props2.second, var);
		}
		return false;
	}
	default:
	{
		int32_t value;
		if (getPlayerProperty(prop.c_str(), value) == true)
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

void Player::setProperty(const std::string& prop, const Variable& val)
{
	if (prop.empty() == true)
	{
		return;
	}
	switch (str2int32(prop.c_str()))
	{
	case str2int32("lifeBase"):
	{
		if (val.is<int64_t>() == true)
		{
			LifeBase((int32_t)val.get<int64_t>());
		}
	}
	break;
	case str2int32("manaBase"):
	{
		if (val.is<int64_t>() == true)
		{
			ManaBase((int32_t)val.get<int64_t>());
		}
	}
	break;
	}
}

const Queryable* Player::getQueryable(const std::string& prop) const
{
	if (prop.empty() == true)
	{
		return this;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int32(props.first.c_str());
	const Queryable* queryable = nullptr;
	switch (propHash)
	{
	case str2int32("selectedItem"):
	{
		queryable = selectedItem.get();
		break;
	}
	break;
	case str2int32("item"):
	{
		size_t invIdx;
		size_t itemIdx;
		if (parseInventoryAndItem(props.second, props.second, invIdx, itemIdx) == true)
		{
			queryable = inventories[invIdx][itemIdx].get();
			break;
		}
	}
	break;
	default:
		break;
	}
	if (queryable != nullptr &&
		props.second.empty() == false)
	{
		return queryable->getQueryable(props.second);
	}
	return queryable;
}

bool Player::getPlayerPropertyByHash(uint16_t propHash, int32_t& value) const
{
	switch (propHash)
	{
	case str2int16("strengthBase"):
		value = strengthBase;
		break;
	case str2int16("strengthNow"):
		value = strengthNow;
		break;
	case str2int16("magicBase"):
		value = magicBase;
		break;
	case str2int16("magicNow"):
		value = magicNow;
		break;
	case str2int16("dexterityBase"):
		value = dexterityBase;
		break;
	case str2int16("dexterityNow"):
		value = dexterityNow;
		break;
	case str2int16("vitalityBase"):
		value = vitalityBase;
		break;
	case str2int16("vitalityNow"):
		value = vitalityNow;
		break;
	case str2int16("lifeBase"):
		value = lifeBase;
		break;
	case str2int16("lifeNow"):
		value = lifeNow;
		break;
	case str2int16("manaBase"):
		value = manaBase;
		break;
	case str2int16("manaNow"):
		value = manaNow;
		break;
	case str2int16("armorClass"):
		value = armorClass;
		break;
	case str2int16("toHit"):
		value = toHit;
		break;
	case str2int16("damageMin"):
		value = damageMin;
		break;
	case str2int16("damageMax"):
		value = damageMax;
		break;
	case str2int16("resistMagic"):
		value = resistMagic;
		break;
	case str2int16("resistFire"):
		value = resistFire;
		break;
	case str2int16("resistLightning"):
		value = resistLightning;
		break;
	default:
		return false;
	}
	return true;
}

bool Player::getPlayerProperty(const char* prop, int32_t& value) const
{
	return getPlayerPropertyByHash(str2int16(prop), value);
}

void Player::setPlayerPropertyByHash(uint16_t propHash, int32_t value)
{
	switch (propHash)
	{
	case str2int16("strengthBase"):
		strengthBase = value;
		break;
	case str2int16("strengthNow"):
		strengthNow = value;
		break;
	case str2int16("magicBase"):
		magicBase = value;
		break;
	case str2int16("magicNow"):
		magicNow = value;
		break;
	case str2int16("dexterityBase"):
		dexterityBase = value;
		break;
	case str2int16("dexterityNow"):
		dexterityNow = value;
		break;
	case str2int16("vitalityBase"):
		vitalityBase = value;
		break;
	case str2int16("vitalityNow"):
		vitalityNow = value;
		break;
	case str2int16("lifeBase"):
		lifeBase = value;
		break;
	case str2int16("lifeNow"):
		lifeNow = value;
		break;
	case str2int16("manaBase"):
		manaBase = value;
		break;
	case str2int16("manaNow"):
		manaNow = value;
		break;
	case str2int16("armorClass"):
		armorClass = value;
		break;
	case str2int16("toHit"):
		toHit = value;
		break;
	case str2int16("damageMin"):
		damageMin = value;
		break;
	case str2int16("damageMax"):
		damageMax = value;
		break;
	case str2int16("resistMagic"):
		resistMagic = value;
		break;
	case str2int16("resistFire"):
		resistFire = value;
		break;
	case str2int16("resistLightning"):
		resistLightning = value;
		break;
	}
}

void Player::setPlayerProperty(const char* prop, int32_t value)
{
	setPlayerPropertyByHash(str2int16(prop), value);
}

bool Player::parseInventoryAndItem(const std::string& str,
	std::string& props, size_t& invIdx, size_t& itemIdx) const
{
	auto strPair = Utils::splitStringIn2(str, '.');
	invIdx = GameUtils::getPlayerInventoryIndex(strPair.first);
	if (invIdx < inventories.size())
	{
		auto strPair2 = Utils::splitStringIn2(strPair.second, '.');
		auto strPair3 = Utils::splitStringIn2(strPair2.first, ',');
		itemIdx = 0;
		if (strPair3.second.empty() == false)
		{
			size_t x = std::strtoul(strPair3.first.c_str(), NULL, 10);
			size_t y = std::strtoul(strPair3.second.c_str(), NULL, 10);
			itemIdx = inventories[invIdx].getIndex(x, y);
		}
		else
		{
			if (invIdx == (size_t)PlayerInventory::Body)
			{
				itemIdx = GameUtils::getPlayerItemMountIndex(strPair2.first);
			}
			else
			{
				itemIdx = std::strtoul(strPair2.first.c_str(), NULL, 10);
			}
		}
		if (itemIdx < inventories[invIdx].Size())
		{
			props = strPair2.second;
			return true;
		}
	}
	return false;
}

void Player::updatePlayerProperties(size_t idx)
{
	strengthNow = strengthBase;
	magicNow = magicBase;
	dexterityNow = dexterityBase;
	vitalityNow = vitalityBase;
	lifeNow = lifeBase;
	manaNow = manaBase;

	if (idx < inventories.size())
	{
		for (const auto& item : inventories[idx])
		{
			if (item != nullptr)
			{
				setPlayerPropertyByHash(str2int16("damageMin"), item->getItemProperty("damageMin"));
				setPlayerPropertyByHash(str2int16("damageMax"), item->getItemProperty("damageMax"));
				for (const auto& itemProp : (*item))
				{
					setPlayerPropertyByHash(itemProp.first, itemProp.second);
				}
			}
		}
	}
}

void Player::applyDefaults()
{
	for (const auto& prop : class_->Defaults())
	{
		setPlayerPropertyByHash(prop.first, prop.second);
	}
}

bool Player::canEquipItem(const Item& item) const
{
	return (item.getItemPropertyByHash(ItemProp::RequiredStrength) <= strengthNow &&
		item.getItemPropertyByHash(ItemProp::RequiredMagic) <= magicNow &&
		item.getItemPropertyByHash(ItemProp::RequiredDexterity) <= dexterityNow &&
		item.getItemPropertyByHash(ItemProp::RequiredVitality) <= vitalityNow);
}
