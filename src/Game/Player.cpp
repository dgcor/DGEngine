#include "Player.h"
#include <cstdlib>
#include "Game.h"
#include "GameUtils.h"
#include "ItemProperties.h"
#include "ItemTypes.h"
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
	auto oldObj = level.Map()[mapPosition.x][mapPosition.y].getObject(this);
	level.Map()[mapPosition.x][mapPosition.y].deleteObject(this);
	mapPosition = pos;
	level.Map()[mapPosition.x][mapPosition.y].addBack(oldObj);
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
			const auto levelObj = level.Map()[nextMapPos.x][nextMapPos.y].front();
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

		if (rect.width > 0 && rect.height > 0)
		{
			updateWalkPath(game, level, sf::Vector2u((unsigned)rect.width, (unsigned)rect.height));
		}

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
	case str2int16("level"):
		var = Variable((int64_t)currentLevel);
		break;
	case str2int16("experience"):
		var = Variable((int64_t)experience);
		break;
	case str2int16("expNextLevel"):
		var = Variable((int64_t)expNextLevel);
		break;
	case str2int16("points"):
		var = Variable((int64_t)points);
		break;
	case str2int16("gold"):
		var = Variable((int64_t)gold);
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
		LevelObjValue value;
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
	switch (str2int16(prop.c_str()))
	{
	case str2int16("lifeBase"):
	{
		if (val.is<int64_t>() == true)
		{
			LifeBase((LevelObjValue)val.get<int64_t>());
		}
	}
	break;
	case str2int16("manaBase"):
	{
		if (val.is<int64_t>() == true)
		{
			ManaBase((LevelObjValue)val.get<int64_t>());
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

bool Player::getPlayerPropertyByHash(uint16_t propHash, LevelObjValue& value) const
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

bool Player::getPlayerProperty(const char* prop, LevelObjValue& value) const
{
	return getPlayerPropertyByHash(str2int16(prop), value);
}

void Player::setPlayerPropertyByHash(uint16_t propHash, LevelObjValue value)
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

void Player::setPlayerProperty(const char* prop, LevelObjValue value)
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

bool Player::addGold(const Level& level, LevelObjValue amount)
{
	if (amount == 0)
	{
		return false;;
	}
	bool remove = amount < 0;
	amount = std::abs(amount);
	size_t invIdx = 0;
	size_t itemIdx = 0;
	std::shared_ptr<Item> item;
	while (findItem(ItemTypes::Gold, invIdx, itemIdx, item) == true)
	{
		auto itemGold = item->getItemPropertyByHash(ItemProp::Gold);
		auto itemMaxGold = item->getItemPropertyByHash(ItemProp::GoldMax);

		if (remove == true)
		{
			if (amount < itemGold)
			{
				item->setItemPropertyByHash(ItemProp::Gold, itemGold - amount);
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
					item->setItemPropertyByHash(ItemProp::Gold, itemGold + amount);
					gold += amount;
					return true;
				}
				else
				{
					item->setItemPropertyByHash(ItemProp::Gold, itemMaxGold);
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
			auto itemMaxGold = newItem->getItemPropertyByHash(ItemProp::GoldMax);
			if (itemMaxGold <= 0)
			{
				return false;
			}
			LevelObjValue goldVal = itemMaxGold;
			if (amount <= itemMaxGold)
			{
				goldVal = amount;
			}
			newItem->setItemPropertyByHash(ItemProp::Gold, goldVal);

			size_t invIdx = 0;
			size_t itemIdx = 0;
			if (getItemSlot(*newItem, invIdx, itemIdx) == true)
			{
				inventories[invIdx].set(itemIdx, newItem);
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
		gold += item->getItemPropertyByHash(ItemProp::Gold);
	}
}

void Player::updateGoldRemove(const std::shared_ptr<Item>& item)
{
	if (item != nullptr &&
		item->Class()->TypeHash16() == ItemTypes::Gold)
	{
		auto val = item->getItemPropertyByHash(ItemProp::Gold);
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
		auto itemGold = item->getItemPropertyByHash(ItemProp::Gold);
		auto itemMaxGold = item->getItemPropertyByHash(ItemProp::GoldMax);

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
				setPlayerPropertyByHash(ItemProp::DamageMin,
					item->getItemPropertyByHash(ItemProp::DamageMin));
				setPlayerPropertyByHash(ItemProp::DamageMax,
					item->getItemPropertyByHash(ItemProp::DamageMax));
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
