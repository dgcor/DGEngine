#include "Player.h"
#include <cstdlib>
#include "Game.h"
#include "GameUtils.h"
#include "ItemProperties.h"
#include "ItemTypes.h"
#include "Level.h"
#include "Utils.h"

void Player::calculateRange()
{
	celTexture = class_->getCelTexture(palette);
	if (celTexture != nullptr
		&& direction < PlayerDirection::Size)
	{
		celIdx = class_->getStatusCelIndex(status);
		auto numFrames = celTexture->size(celIdx);
		if (direction == PlayerDirection::All)
		{
			frameRange.first = 0;
			frameRange.second = numFrames;
		}
		else
		{
			auto period = (numFrames / 8);
			frameRange.first = (size_t)direction * period;
			frameRange.second = frameRange.first + period;
		}
	}
	else
	{
		celIdx = 0;
		frameRange.first = 0;
		frameRange.second = 0;
	}
}

void Player::updateDrawPosition(sf::Vector2f pos)
{
	pos.x += (float)(-(sprite.getTextureRect().width / 2)) + LevelMap::TileSize();
	pos.y += (float)(224 - (sprite.getTextureRect().height - LevelMap::TileSize()));
	sprite.setPosition(pos);
}

void Player::updateTexture()
{
	if (currentFrame < frameRange.first || currentFrame >= frameRange.second)
	{
		currentFrame = frameRange.first;
	}
	if (currentFrame < celTexture->size(celIdx))
	{
		sprite.setTexture(celTexture->get(celIdx, currentFrame), true);
	}
	currentFrame++;
}

void Player::updateSpeed()
{
	if (hasWalkingStatus() == true)
	{
		if (defaultSpeed.animation != sf::Time::Zero)
		{
			speed.animation = defaultSpeed.animation;
		}
		if (defaultSpeed.walk != sf::Time::Zero)
		{
			speed.walk = defaultSpeed.walk;
		}
	}
}

void Player::updateWalkPathStep(sf::Vector2f& newDrawPos)
{
	newDrawPos.x -= std::round((drawPosA.x - drawPosB.x) * currPositionStep);
	newDrawPos.y -= std::round((drawPosA.y - drawPosB.y) * currPositionStep);

	if (currPositionStep >= 1.f)
	{
		if (walkPath.empty() == false)
		{
			walkPath.pop_back();
		}
		drawPosA = drawPosB;
		newDrawPos = drawPosB;
	}
	else
	{
		currPositionStep += 0.1f;
	}
}

void Player::updateWalkPath(Game& game, Level& level)
{
	auto newDrawPos = drawPosA;
	if (drawPosA == drawPosB)
	{
		if (walkPath.empty() == true &&
			hasWalkingStatus() == true)
		{
			setStandStatus();
			resetAnimationTime();
		}
		while (walkPath.empty() == false)
		{
			const auto& nextMapPos = walkPath.back();
			if (walkPath.size() == 1)
			{
				const auto levelObj = level.Map()[nextMapPos].front();
				if (levelObj != nullptr)
				{
					levelObj->executeAction(game);
					walkPath.pop_back();
					return;
				}
			}
			if (nextMapPos == mapPosition)
			{
				walkPath.pop_back();
				continue;
			}
			setWalkStatus();
			setDirection(getPlayerDirection(mapPosition, nextMapPos));
			MapPosition(level, nextMapPos);
			currPositionStep = 0.1f;
			updateWalkPathStep(newDrawPos);
			break;
		}
	}
	else
	{
		updateWalkPathStep(newDrawPos);
	}
	updateDrawPosition(newDrawPos);
}

void Player::setWalkPath(const std::vector<MapCoord>& walkPath_)
{
	if (walkPath_.empty() == true)
	{
		return;
	}
	walkPath = walkPath_;
	if (walkPath.empty() == false)
	{
		mapPositionMoveTo = walkPath.front();
	}
}

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

void Player::updateMapPosition(Level& level, const MapCoord& pos)
{
	auto oldObj = level.Map()[mapPosition].getObject(this);
	level.Map()[mapPosition].deleteObject(this);
	mapPosition = pos;
	level.Map()[mapPosition].addBack(oldObj);
}

void Player::MapPosition(Level& level, const MapCoord& pos)
{
	drawPosA = level.Map().getCoord(mapPosition);
	drawPosB = level.Map().getCoord(pos);
	updateMapPosition(level, pos);
}

void Player::move(Level& level, const MapCoord& pos)
{
	if (mapPosition == pos)
	{
		return;
	}
	clearWalkPath();
	setStandStatus();
	resetAnimationTime();
	drawPosA = drawPosB = level.Map().getCoord(pos);
	updateMapPosition(level, pos);
}

void Player::update(Game& game, Level& level)
{
	if (celTexture == nullptr
		|| frameRange.first > frameRange.second)
	{
		return;
	}

	currentWalkTime += game.getElapsedTime();
	if (currentWalkTime >= speed.walk)
	{
		currentWalkTime = sf::microseconds(
			currentWalkTime.asMicroseconds() % speed.walk.asMicroseconds());

		updateWalkPath(game, level);
	}

	currentAnimationTime += game.getElapsedTime();
	if (currentAnimationTime >= speed.animation)
	{
		currentAnimationTime = sf::microseconds(
			currentAnimationTime.asMicroseconds() % speed.animation.asMicroseconds());

		updateTexture();
	}
	if (enableHover == false)
	{
		return;
	}
	if (level.HasMouseInside() == true &&
		sprite.getGlobalBounds().contains(level.MousePositionf()) == true)
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

bool Player::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	switch (str2int16(props.first.c_str()))
	{
	case str2int16("type"):
		var = Variable(std::string("player"));
		break;
	case str2int16("id"):
		var = Variable(id);
		break;
	case str2int16("name"):
		var = Variable(name);
		break;
	case str2int16("class"):
		var = Variable(class_->Name());
		break;
	case str2int16("canEquipSelectedItem"):
	{
		if (selectedItem == nullptr)
		{
			return false;
		}
		var = Variable(canEquipItem(*selectedItem));
		break;
	}
	case str2int16("canEquipItem"):
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
	case str2int16("hasSelectedItem"):
		var = Variable(selectedItem != nullptr);
		break;
	case str2int16("hasItem"):
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
	case str2int16("isItemSlotInUse"):
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
	case str2int16("selectedItem"):
	{
		if (selectedItem != nullptr)
		{
			return selectedItem->getProperty(props.second, var);
		}
		return false;
	}
	break;
	case str2int16("item"):
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
	case str2int16("inventory"):
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
		Number32 value;
		if (getNumberProp(prop.c_str(), value) == true)
		{
			var = Variable(value.getInt64());
			break;
		}
		return false;
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
	auto propHash16 = str2int16(prop.c_str());
	switch (propHash16)
	{
	case str2int16("name"):
	{
		if (val.is<std::string>() == true)
		{
			Name(val.get<std::string>());
		}
	}
	break;
	default:
	{
		if (val.is<int64_t>() == true)
		{
			if (setNumberByHash(propHash16, (LevelObjValue)val.get<int64_t>()))
			{
				updatePlayerProperties();
			}
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
	auto propHash = str2int16(props.first.c_str());
	const Queryable* queryable = nullptr;
	switch (propHash)
	{
	case str2int16("selectedItem"):
	{
		queryable = selectedItem.get();
		break;
	}
	break;
	case str2int16("item"):
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

bool Player::getNumberProp(const char* prop, Number32& value) const
{
	return getNumberByHash(str2int16(prop), value);
}

bool Player::getNumberByHash(uint16_t propHash, Number32& value) const
{
	LevelObjValue iVal;
	if (getIntByHash(propHash, iVal) == true)
	{
		value.setInt32(iVal);
		return true;
	}
	uint32_t uVal;
	if (getUIntByHash(propHash, uVal) == true)
	{
		value.setUInt32(uVal);
		return true;
	}
	for (size_t i = 0; i < customPropsSize; i++)
	{
		const auto& customProp = customProperties[i];
		if (customProp.first == propHash)
		{
			value = customProp.second;
			return true;
		}
	}
	return false;
}

bool Player::getIntByHash(uint16_t propHash, LevelObjValue& value) const
{
	switch (propHash)
	{
	case str2int16("strength"):
		value = strength;
		break;
	case str2int16("strengthItems"):
		value = strengthItems;
		break;
	case str2int16("strengthNow"):
		value = StrengthNow();
		break;
	case str2int16("magic"):
		value = magic;
		break;
	case str2int16("magicItems"):
		value = magicItems;
		break;
	case str2int16("magicNow"):
		value = MagicNow();
		break;
	case str2int16("dexterity"):
		value = dexterity;
		break;
	case str2int16("dexterityItems"):
		value = dexterityItems;
		break;
	case str2int16("dexterityNow"):
		value = DexterityNow();
		break;
	case str2int16("vitality"):
		value = vitality;
		break;
	case str2int16("vitalityItems"):
		value = vitalityItems;
		break;
	case str2int16("vitalityNow"):
		value = VitalityNow();
		break;
	case str2int16("life"):
		value = life;
		break;
	case str2int16("lifeItems"):
		value = lifeItems;
		break;
	case str2int16("lifeDamage"):
		value = lifeDamage;
		break;
	case str2int16("lifeNow"):
		value = LifeNow();
		break;
	case str2int16("mana"):
		value = mana;
		break;
	case str2int16("manaItems"):
		value = manaItems;
		break;
	case str2int16("manaDamage"):
		value = manaDamage;
		break;
	case str2int16("manaNow"):
		value = ManaNow();
		break;
	case str2int16("armor"):
		value = armor;
		break;
	case str2int16("armorItems"):
		value = armorItems;
		break;
	case str2int16("toArmor"):
		value = toArmor;
		break;
	case str2int16("toHit"):
		value = toHit;
		break;
	case str2int16("toHitItems"):
		value = toHitItems;
		break;
	case str2int16("damageMin"):
		value = damageMin;
		break;
	case str2int16("damageMinItems"):
		value = damageMinItems;
		break;
	case str2int16("damageMax"):
		value = damageMax;
		break;
	case str2int16("damageMaxItems"):
		value = damageMaxItems;
		break;
	case str2int16("toDamage"):
		value = toDamage;
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
	case str2int16("maxStrength"):
		value = class_->MaxStrength();
		break;
	case str2int16("maxMagic"):
		value = class_->MaxMagic();
		break;
	case str2int16("maxDexterity"):
		value = class_->MaxDexterity();
		break;
	case str2int16("maxVitality"):
		value = class_->MaxVitality();
		break;
	case str2int16("maxResistMagic"):
		value = class_->MaxResistMagic();
		break;
	case str2int16("maxResistFire"):
		value = class_->MaxResistFire();
		break;
	case str2int16("maxResistLightning"):
		value = class_->MaxResistLightning();
		break;
	default:
		return false;
	}
	return true;
}

bool Player::getInt(const char* prop, LevelObjValue& value) const
{
	return getIntByHash(str2int16(prop), value);
}

bool Player::getUIntByHash(uint16_t propHash, uint32_t& value) const
{
	switch (propHash)
	{
	case str2int16("level"):
		value = currentLevel;
		break;
	case str2int16("experience"):
		value = experience;
		break;
	case str2int16("expNextLevel"):
		value = expNextLevel;
		break;
	case str2int16("points"):
		value = points;
		break;
	case str2int16("gold"):
		value = gold;
		break;
	default:
		return false;
	}
	return true;
}

bool Player::getUInt(const char* prop, uint32_t& value) const
{
	return getUIntByHash(str2int16(prop), value);
}

bool Player::setIntByHash(uint16_t propHash, LevelObjValue value)
{
	switch (propHash)
	{
	case str2int16("strength"):
		strength = std::min(std::max(value, 0), class_->MaxStrength());
		break;
	case str2int16("magic"):
		magic = std::min(std::max(value, 0), class_->MaxMagic());
		break;
	case str2int16("dexterity"):
		dexterity = std::min(std::max(value, 0), class_->MaxDexterity());
		break;
	case str2int16("vitality"):
		vitality = std::min(std::max(value, 0), class_->MaxVitality());
		break;
	case str2int16("lifeDamage"):
		lifeDamage = std::max(value, 0);
		break;
	case str2int16("manaDamage"):
		manaDamage = std::max(value, 0);
		break;
	default:
		return false;
	}
	return true;
}

bool Player::setInt(const char* prop, LevelObjValue value)
{
	return setIntByHash(str2int16(prop), value);
}

bool Player::setUIntByHash(uint16_t propHash, uint32_t value)
{
	switch (propHash)
	{
	case str2int16("level"):
		currentLevel = value;
		break;
	case str2int16("experience"):
		experience = value;
		break;
	case str2int16("expNextLevel"):
		expNextLevel = value;
		break;
	case str2int16("points"):
		points = value;
		break;
	default:
		return false;
	}
	return true;
}

bool Player::setUInt(const char* prop, uint32_t value)
{
	return setUIntByHash(str2int16(prop), value);
}

bool Player::setNumberByHash(uint16_t propHash, LevelObjValue value)
{
	if (setIntByHash(propHash, value) == false)
	{
		return setUIntByHash(propHash, (uint32_t)value);
	}
	return true;
}

bool Player::setNumber(const char* prop, LevelObjValue value)
{
	return setNumberByHash(str2int16(prop), value);
}

bool Player::setNumber(const char* prop, const Number32& value)
{
	return setNumberByHash(str2int16(prop), value);
}

bool Player::setNumberByHash(uint16_t propHash, const Number32& value)
{
	if (setNumberByHash(propHash, value.getInt32()) == true)
	{
		return true;
	}
	switch (propHash)
	{
	case str2int16(""):
	case str2int16("strengthItems"):
	case str2int16("strengthNow"):
	case str2int16("magicItems"):
	case str2int16("magicNow"):
	case str2int16("dexterityItems"):
	case str2int16("dexterityNow"):
	case str2int16("vitalityItems"):
	case str2int16("vitalityNow"):
	case str2int16("life"):
	case str2int16("lifeItems"):
	case str2int16("lifeNow"):
	case str2int16("mana"):
	case str2int16("manaItems"):
	case str2int16("manaNow"):
	case str2int16("armor"):
	case str2int16("armorItems"):
	case str2int16("toArmor"):
	case str2int16("toHit"):
	case str2int16("toHitItems"):
	case str2int16("damageMin"):
	case str2int16("damageMinItems"):
	case str2int16("damageMax"):
	case str2int16("damageMaxItems"):
	case str2int16("toDamage"):
	case str2int16("resistMagic"):
	case str2int16("resistFire"):
	case str2int16("resistLightning"):
	case str2int16("maxStrength"):
	case str2int16("maxMagic"):
	case str2int16("maxDexterity"):
	case str2int16("maxVitality"):
	case str2int16("maxResistMagic"):
	case str2int16("maxResistFire"):
	case str2int16("maxResistLightning"):
		return false;
	default:
	{
		size_t i = 0;
		for (; i < customPropsSize; i++)
		{
			auto& elem = customProperties[i];
			if (elem.first == propHash)
			{
				elem.second = value;
				return true;
			}
		}
		if (i < customProperties.size())
		{
			customProperties[i] = std::make_pair(propHash, value);
			customPropsSize++;
			return true;
		}
		return false;
	}
	}
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

bool Player::addGold(const Level& level, LevelObjValue amount)
{
	if (amount == 0)
	{
		return false;
	}
	bool remove = amount < 0;
	amount = std::abs(amount);
	size_t invIdx = 0;
	size_t itemIdx = 0;
	std::shared_ptr<Item> item;
	while (findItem(ItemTypes::Gold, invIdx, itemIdx, item) == true)
	{
		auto itemGold = item->getIntByHash(ItemProp::Gold);
		auto itemMaxGold = item->getIntByHash(ItemProp::GoldMax);

		if (remove == true)
		{
			if (amount < itemGold)
			{
				item->setIntByHash(ItemProp::Gold, itemGold - amount);
				gold -= amount;
				return true;
			}
			else
			{
				amount -= itemGold;
				gold -= itemGold;
				inventories[invIdx].set(itemIdx, nullptr);
			}
		}
		else
		{
			LevelObjValue freeGoldSlots = itemMaxGold - itemGold;
			if (freeGoldSlots > 0)
			{
				if (amount <= freeGoldSlots)
				{
					item->setIntByHash(ItemProp::Gold, itemGold + amount);
					gold += amount;
					return true;
				}
				else
				{
					item->setIntByHash(ItemProp::Gold, itemMaxGold);
					amount -= freeGoldSlots;
					gold += freeGoldSlots;
				}
			}
		}
		itemIdx++;
		if (itemIdx >= inventories[invIdx].Size())
		{
			invIdx++;
		}
	}
	if (remove == false)
	{
		auto goldClass = level.getItemClass("gold");
		if (goldClass == nullptr)
		{
			return false;
		}
		while (true)
		{
			if (amount <= 0)
			{
				return true;
			}
			auto newItem = std::make_shared<Item>(goldClass);
			auto itemMaxGold = newItem->getIntByHash(ItemProp::GoldMax);
			if (itemMaxGold <= 0)
			{
				return false;
			}
			LevelObjValue goldVal = itemMaxGold;
			if (amount <= itemMaxGold)
			{
				goldVal = amount;
			}
			newItem->setIntByHash(ItemProp::Gold, goldVal);

			size_t invIdx2 = 0;
			size_t itemIdx2 = 0;
			if (getItemSlot(*newItem, invIdx2, itemIdx2) == true)
			{
				inventories[invIdx2].set(itemIdx2, newItem);
				amount -= goldVal;
				gold += goldVal;
			}
			else
			{
				break;
			}
		}
	}
	return false;
}

void Player::updateGoldAdd(const std::shared_ptr<Item>& item)
{
	if (item != nullptr &&
		item->Class()->TypeHash16() == ItemTypes::Gold)
	{
		gold += item->getIntByHash(ItemProp::Gold);
	}
}

void Player::updateGoldRemove(const std::shared_ptr<Item>& item)
{
	if (item != nullptr &&
		item->Class()->TypeHash16() == ItemTypes::Gold)
	{
		auto val = item->getIntByHash(ItemProp::Gold);
		if (val > 0)
		{
			gold -= val;
		}
		else
		{
			gold += val;
		}
	}
}

uint32_t Player::getMaxGoldCapacity(const Level& level) const
{
	uint64_t maxGold = 0;
	size_t invIdx = 0;
	size_t itemIdx = 0;
	std::shared_ptr<Item> item;
	while (findItem(ItemTypes::Gold, invIdx, itemIdx, item) == true)
	{
		auto itemGold = item->getIntByHash(ItemProp::Gold);
		auto itemMaxGold = item->getIntByHash(ItemProp::GoldMax);

		if (itemGold < itemMaxGold)
		{
			maxGold += itemMaxGold - itemGold;
			if (maxGold >= std::numeric_limits<uint32_t>::max())
			{
				return std::numeric_limits<uint32_t>::max();
			}
		}

		itemIdx++;
		if (itemIdx >= inventories[invIdx].Size())
		{
			invIdx++;
		}
	}
	auto goldClass = level.getItemClass("gold");
	if (goldClass != nullptr)
	{
		auto freeSlots = countFreeSlots(*goldClass);
		if (freeSlots > 0)
		{
			auto defaultMaxGold = goldClass->getDefaultByHash(ItemProp::GoldMax);
			maxGold += defaultMaxGold * freeSlots;
			if (maxGold >= std::numeric_limits<uint32_t>::max())
			{
				return std::numeric_limits<uint32_t>::max();
			}
		}
	}
	return (uint32_t)maxGold;
}

bool Player::getItemSlot(const Item& item, size_t& invIdx,
	size_t& itemIdx, InventoryPosition invPos) const
{
	for (size_t i = 0; i < inventories.size(); i++)
	{
		if (inventories[i].getItemSlot(item, itemIdx, invPos) == true)
		{
			invIdx = i;
			return true;
		}
	}
	return false;
}

bool Player::hasItemSlot(const Item& item) const
{
	size_t invIdx;
	size_t itemIdx;
	return getItemSlot(item, invIdx, itemIdx);
}

bool Player::findItem(uint16_t itemTypeHash16, size_t& invIdx,
	size_t& itemIdx, std::shared_ptr<Item>& item) const
{
	auto size = inventories.size();
	if (invIdx < size)
	{
		for (size_t i = invIdx; i < size; i++)
		{
			size_t itemIdx2 = itemIdx;
			if (inventories[i].find(itemTypeHash16, itemIdx2, item) == true)
			{
				invIdx = i;
				itemIdx = itemIdx2;
				return true;
			}
		}
	}
	invIdx = size;
	itemIdx = 0;
	return false;
}

unsigned Player::countFreeSlots(const ItemClass& itemClass) const
{
	unsigned count = 0;
	for (const auto& inv : inventories)
	{
		count += inv.countFreeSlots(itemClass);
	}
	return count;
}

bool Player::setItem(size_t invIdx, size_t itemIdx, const std::shared_ptr<Item>& item)
{
	std::shared_ptr<Item> oldItem;
	return setItem(invIdx, itemIdx, item, oldItem);
}

bool Player::setItem(size_t invIdx, size_t itemIdx, const std::shared_ptr<Item>& item,
	std::shared_ptr<Item>& oldItem)
{
	if (invIdx >= inventories.size())
	{
		return false;
	}
	auto& inventory = inventories[invIdx];
	auto ret = inventory.set(itemIdx, item, oldItem);
	if (ret == true)
	{
		updateGoldRemove(oldItem);
		updateGoldAdd(item);
		if (bodyInventoryIdx == invIdx)
		{
			updatePlayerProperties();
		}
	}
	return ret;
}

bool Player::setItemInFreeSlot(size_t invIdx,
	const std::shared_ptr<Item>& item, InventoryPosition invPos)
{
	if (invIdx < inventories.size())
	{
		auto& inventory = inventories[invIdx];
		size_t itemIdx = 0;
		if (inventory.getItemSlot(*item, itemIdx, invPos) == true)
		{
			return setItem(invIdx, itemIdx, item);
		}
	}
	return false;
}

void Player::updateBodyItemValues()
{
	strengthItems = 0;
	magicItems = 0;
	dexterityItems = 0;
	vitalityItems = 0;
	lifeItems = 0;
	manaItems = 0;
	armor = 0;
	armorItems = 0;
	toArmor = 0;
	toHitItems = 0;
	damageMin = 1;
	damageMax = 1;
	damageMinItems = 0;
	damageMaxItems = 0;
	toDamage = 0;
	resistMagic = 0;
	resistFire = 0;
	resistLightning = 0;

	if (bodyInventoryIdx >= inventories.size())
	{
		return;
	}
	for (size_t i = 0; i < inventories[bodyInventoryIdx].Size(); i++)
	{
		if (inventories[bodyInventoryIdx].isItemSlotInUse(i) == false)
		{
			continue;
		}
		const auto& item = inventories[bodyInventoryIdx].get(i);

		if (item->Identified() == true)
		{
			auto allAttributes = item->getIntByHash(ItemProp::AllAttributes);
			strengthItems += allAttributes + item->getIntByHash(ItemProp::Strength);
			magicItems += allAttributes + item->getIntByHash(ItemProp::Magic);
			dexterityItems += allAttributes + item->getIntByHash(ItemProp::Dexterity);
			vitalityItems += allAttributes + item->getIntByHash(ItemProp::Vitality);

			lifeItems += item->getIntByHash(ItemProp::Life);
			manaItems += item->getIntByHash(ItemProp::Mana);

			auto resistAll = item->getIntByHash(ItemProp::ResistAll);
			resistMagic += resistAll + item->getIntByHash(ItemProp::ResistMagic);
			resistFire += resistAll + item->getIntByHash(ItemProp::ResistFire);
			resistLightning += resistAll + item->getIntByHash(ItemProp::ResistLightning);

			toArmor += item->getIntByHash(ItemProp::ToArmor);
			toDamage += item->getIntByHash(ItemProp::ToDamage);

			resistMagic = std::min(std::max(resistMagic, 0), class_->MaxResistMagic());
			resistFire = std::min(std::max(resistFire, 0), class_->MaxResistFire());
			resistLightning = std::min(std::max(resistLightning, 0), class_->MaxResistLightning());

			auto damage = item->getIntByHash(ItemProp::Damage);
			damageMin += damage;
			damageMax += damage;
		}
		armorItems += item->getIntByHash(ItemProp::Armor);
		toHitItems += item->getIntByHash(ItemProp::ToHit);
		damageMinItems += item->getIntByHash(ItemProp::DamageMin);
		damageMaxItems += item->getIntByHash(ItemProp::DamageMax);
	}
	auto toArmorPercentage = (float)toArmor * 0.01f;
	armor += (LevelObjValue)(armorItems * toArmorPercentage);

	auto toDamagePercentage = 1.f + ((float)toDamage * 0.01f);
	if (damageMinItems > 0)
	{
		damageMin--;
		damageMin += (LevelObjValue)(damageMinItems * toDamagePercentage);
	}
	if (damageMaxItems > 0)
	{
		damageMax--;
		damageMax += (LevelObjValue)(damageMaxItems * toDamagePercentage);
	}
}

void Player::updatePlayerProperties()
{
	updateBodyItemValues();

	life = class_->getActualLife(*this, 0);
	mana = class_->getActualMana(*this, 0);
	armor += class_->getActualArmor(*this, 0);
	toHit = class_->getActualToHit(*this, 0);
	auto damage = class_->getActualDamage(*this, 0);
	damageMin += damage;
	damageMax += damage;
}

void Player::applyDefaults()
{
	for (const auto& prop : class_->Defaults())
	{
		setNumberByHash(prop.first, prop.second);
	}
}

bool Player::canEquipItem(const Item& item) const
{
	return (item.getIntByHash(ItemProp::RequiredStrength) <= StrengthNow() &&
		item.getIntByHash(ItemProp::RequiredMagic) <= MagicNow() &&
		item.getIntByHash(ItemProp::RequiredDexterity) <= DexterityNow() &&
		item.getIntByHash(ItemProp::RequiredVitality) <= VitalityNow());
}
