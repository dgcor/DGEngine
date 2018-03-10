#include "Player.h"
#include <cstdlib>
#include "Game.h"
#include "GameUtils.h"
#include "ItemProperties.h"
#include "ItemTypes.h"
#include "Level.h"
#include "Utils/Utils.h"

Player::Player(const PlayerClass* class__, const Level& level) : class_(class__)
{
	base.animation.animType = AnimationType::Looped;
	base.hoverCellSize = 2;
	base.sprite.setOutline(class__->DefaultOutline(), class__->DefaultOutlineIgnore());
	calculateRange();
	applyDefaults(level);
}

void Player::calculateRange()
{
	auto oldTexturePack = base.texturePack;
	base.texturePack = class_->getTexturePack(textureIdx);
	if (base.texturePack != nullptr
		&& direction < PlayerDirection::Size)
	{
		if (base.texturePack != oldTexturePack)
		{
			if (base.texturePack->isIndexed() == true)
			{
				base.sprite.setPalette(base.texturePack->getPalette());
			}
			else
			{
				base.sprite.setPalette(nullptr);
			}
		}
		class_->getTextureAnimationRange(textureIdx, animation, base.animation);
		if (direction != PlayerDirection::All)
		{
			auto period = (((base.animation.textureIndexRange.second + 1) - base.animation.textureIndexRange.first) / 8);
			base.animation.textureIndexRange.first += ((size_t)direction * period);
			base.animation.textureIndexRange.second = base.animation.textureIndexRange.first + period - 1;
		}
	}
	else
	{
		base.sprite.setPalette(nullptr);
		base.animation.clear();
	}
	base.animation.reset();
}

void Player::updateTexture()
{
	base.updateTexture();
}

void Player::updateSpeed()
{
	if (hasWalkingAnimation() == true)
	{
		if (defaultSpeed.animation != sf::Time::Zero)
		{
			speed.animation = defaultSpeed.animation;
			base.animation.frameTime = speed.animation;
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
	currentWalkTime += game.getElapsedTime();
	if (currentWalkTime < speed.walk)
	{
		return;
	}

	currentWalkTime = sf::microseconds(
		currentWalkTime.asMicroseconds() % speed.walk.asMicroseconds());

	auto newDrawPos = drawPosA;
	if (drawPosA == drawPosB)
	{
		if (walkPath.empty() == true &&
			hasWalkingAnimation() == true)
		{
			setStandAnimation();
			resetAnimationTime();
			playerAction = PlayerAction::Stand;
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

					setStandAnimation();
					resetAnimationTime();
					playerAction = PlayerAction::Stand;
					return;
				}
			}
			if (nextMapPos == base.mapPosition)
			{
				walkPath.pop_back();
				continue;
			}
			playSound(walkSound);
			setWalkAnimation();
			setDirection(getPlayerDirection(base.mapPosition, nextMapPos));
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
	base.updateDrawPosition(level, newDrawPos);
}

void Player::setWalkPath(const std::vector<MapCoord>& walkPath_)
{
	if (walkPath_.empty() == true)
	{
		return;
	}
	walkPath = walkPath_;
	playerAction = PlayerAction::Walk;
	if (walkPath.empty() == false)
	{
		mapPositionMoveTo = walkPath.front();
	}
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
	drawPosA = level.Map().getCoord(base.mapPosition);
	drawPosB = level.Map().getCoord(pos);
	base.updateMapPositionBack(level, pos, this);
}

void Player::move(Level& level, const MapCoord& pos)
{
	if (base.mapPosition == pos)
	{
		return;
	}
	clearWalkPath();
	setStandAnimation();
	playerAction = PlayerAction::Stand;
	resetAnimationTime();
	drawPosA = drawPosB = level.Map().getCoord(pos);
	base.updateMapPositionBack(level, pos, this);
	updateDrawPosition(level);
}

void Player::updateAI(Level& level)
{
	switch (playerAction)
	{
	case PlayerAction::Walk:
		return;
	default:
		break;
	}
	auto plr = level.getCurrentPlayer();
	if (plr != nullptr)
	{
		setWalkPath(level.Map().getPath(base.mapPosition, plr->MapPosition()));
	}
}

void Player::updateWalk(Game& game, Level& level)
{
	updateWalkPath(game, level);
	updateAnimation(game);
}

void Player::updateAttack(Game& game, Level& level)
{
	updateAnimation(game);
}

void Player::updateDead(Game& game, Level& level)
{
	if (animation != PlayerAnimation::Die1)
	{
		base.animation.currentTextureIdx = 0;
		setAnimation(PlayerAnimation::Die1);
	}
	if (base.animation.currentTextureIdx >= base.animation.textureIndexRange.second)
	{
		return;
	}
	updateAnimation(game);
}

void Player::updateAnimation(const Game& game)
{
	if (base.animation.update(game.getElapsedTime()))
	{
		base.updateTexture();
	}
}

void Player::update(Game& game, Level& level)
{
	base.processQueuedActions(game);

	if (base.hasValidState() == false)
	{
		return;
	}
	if (useAI == true)
	{
		updateAI(level);
	}

	switch (playerAction)
	{
	default:
	case PlayerAction::Stand:
		updateAnimation(game);
		break;
	case PlayerAction::Walk:
		updateWalk(game, level);
		break;
	case PlayerAction::Attack:
		updateAttack(game, level);
		break;
	case PlayerAction::Dead:
		updateDead(game, level);
		break;
	}

	base.updateHover(game, level, this);
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
	case str2int16("d"):
	case str2int16("description"):
		var = Variable(GameUtils::replaceStringWithQueryable(class_->Description(), *this));
		break;
	case str2int16("class"):
		var = Variable(class_->Name());
		break;
	case str2int16("totalKills"):
		var = Variable((int64_t)class_->TotalKills());
		break;
	case str2int16("hasMaxStats"):
		var = Variable(hasMaxStats());
		break;
	case str2int16("hasProperty"):
		var = Variable(hasInt(props.second));
		break;
	case str2int16("canUseSelectedItem"):
	{
		if (selectedItem == nullptr)
		{
			return false;
		}
		var = Variable(canUseItem(*selectedItem));
		break;
	}
	case str2int16("canUseItem"):
	{
		std::string props2;
		size_t invIdx;
		size_t itemIdx;
		if (parseInventoryAndItem(props.second, props2, invIdx, itemIdx) == true)
		{
			auto item = inventories[invIdx].get(itemIdx);
			if (item != nullptr)
			{
				var = Variable(canUseItem(*item));
				break;
			}
		}
		return false;
	}
	case str2int16("hasEquipedItemType"):
		var = Variable(hasEquipedItemType(props.second));
		break;
	case str2int16("hasEquipedItemSubType"):
		var = Variable(hasEquipedItemSubType(props.second));
		break;
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
			var = Variable(inventories[invIdx].get(itemIdx) != nullptr);
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
			auto item = inventories[invIdx].get(itemIdx);
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
		if (std::holds_alternative<std::string>(val) == true)
		{
			Name(std::get<std::string>(val));
		}
	}
	break;
	default:
	{
		if (std::holds_alternative<int64_t>(val) == true)
		{
			if (setNumberByHash(propHash16, (LevelObjValue)std::get<int64_t>(val), nullptr))
			{
				updateProperties();
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
			queryable = inventories[invIdx].get(itemIdx);
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

bool Player::hasIntByHash(uint16_t propHash) const noexcept
{
	if (customPropsSize > 0)
	{
		for (size_t i = 0; i < customPropsSize; i++)
		{
			if (customProperties[i].first == propHash)
			{
				return true;
			}
		}
	}
	return false;
}

bool Player::hasInt(const char* prop) const noexcept
{
	return hasIntByHash(str2int16(prop));
}

bool Player::getNumberProp(const char* prop, Number32& value) const noexcept
{
	return getNumberByHash(str2int16(prop), value);
}

bool Player::getNumberByHash(uint16_t propHash, Number32& value) const noexcept
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
	return getCustomIntByHash(propHash, value);
}

bool Player::getCustomIntByHash(uint16_t propHash, Number32& value) const noexcept
{
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

bool Player::getCustomInt(const char* prop, Number32& value) const noexcept
{
	return getCustomIntByHash(str2int16(prop), value);
}

bool Player::getIntByHash(uint16_t propHash, LevelObjValue& value) const noexcept
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
	case str2int16("resistMagicItems"):
		value = resistMagicItems;
		break;
	case str2int16("resistFire"):
		value = resistFire;
		break;
	case str2int16("resistFireItems"):
		value = resistFireItems;
		break;
	case str2int16("resistLightning"):
		value = resistLightning;
		break;
	case str2int16("resistLightningItems"):
		value = resistLightningItems;
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

bool Player::getInt(const char* prop, LevelObjValue& value) const noexcept
{
	return getIntByHash(str2int16(prop), value);
}

bool Player::getUIntByHash(uint16_t propHash, uint32_t& value) const noexcept
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

bool Player::getUInt(const char* prop, uint32_t& value) const noexcept
{
	return getUIntByHash(str2int16(prop), value);
}

bool Player::setIntByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept
{
	switch (propHash)
	{
	case str2int16("strength"):
		strength = std::clamp(value, 0, class_->MaxStrength());
		break;
	case str2int16("magic"):
		magic = std::clamp(value, 0, class_->MaxMagic());
		break;
	case str2int16("dexterity"):
		dexterity = std::clamp(value, 0, class_->MaxDexterity());
		break;
	case str2int16("vitality"):
		vitality = std::clamp(value, 0, class_->MaxVitality());
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

bool Player::setInt(const char* prop, LevelObjValue value, const Level* level) noexcept
{
	return setIntByHash(str2int16(prop), value, level);
}

bool Player::setUIntByHash(uint16_t propHash, uint32_t value, const Level* level) noexcept
{
	switch (propHash)
	{
	case str2int16("experience"):
	{
		experience = value;
		if (level != nullptr)
		{
			updateExperience(*level);
		}
		break;
	}
	case str2int16("points"):
		points = value;
		break;
	default:
		return false;
	}
	return true;
}

bool Player::setUInt(const char* prop, uint32_t value, const Level* level) noexcept
{
	return setUIntByHash(str2int16(prop), value, level);
}

bool Player::setNumberByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept
{
	if (setIntByHash(propHash, value, nullptr) == false)
	{
		return setUIntByHash(propHash, (uint32_t)value, level);
	}
	return true;
}

bool Player::setNumber(const char* prop, LevelObjValue value, const Level* level) noexcept
{
	return setNumberByHash(str2int16(prop), value, level);
}

bool Player::setNumber(const char* prop, const Number32& value, const Level* level) noexcept
{
	return setNumberByHash(str2int16(prop), value, level);
}

bool Player::setNumberByHash(uint16_t propHash, const Number32& value, const Level* level) noexcept
{
	if (setNumberByHash(propHash, value.getInt32(), level) == true)
	{
		return true;
	}
	switch (propHash)
	{
	case str2int16(""):
	case str2int16("hasMaxStats"):
	case str2int16("level"):
	case str2int16("expNextLevel"):
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
	Item* item;
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
				std::unique_ptr<Item> nullItem;
				inventories[invIdx].set(itemIdx, nullItem);
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
			auto newItem = std::make_unique<Item>(goldClass);
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
			if (getFreeItemSlot(*newItem, invIdx2, itemIdx2) == true)
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

void Player::updateGoldAdd(const Item* item)
{
	if (item != nullptr &&
		item->Class()->TypeHash16() == ItemTypes::Gold)
	{
		gold += item->getIntByHash(ItemProp::Gold);
	}
}

void Player::updateGoldRemove(const Item* item)
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
	Item* item;
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

bool Player::getFreeItemSlot(const Item& item, size_t& invIdx,
	size_t& itemIdx, InventoryPosition invPos) const
{
	for (size_t i = 0; i < inventories.size(); i++)
	{
		if (inventories[i].getFreeItemSlot(item, itemIdx, invPos) == true)
		{
			invIdx = i;
			return true;
		}
	}
	return false;
}

bool Player::hasFreeItemSlot(const Item& item) const
{
	size_t invIdx;
	size_t itemIdx;
	return getFreeItemSlot(item, invIdx, itemIdx);
}

bool Player::findItem(uint16_t itemTypeHash16, size_t& invIdx,
	size_t& itemIdx, Item*& item) const
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

bool Player::setItem(size_t invIdx, size_t itemIdx, std::unique_ptr<Item>& item)
{
	std::unique_ptr<Item> oldItem;
	return setItem(invIdx, itemIdx, item, oldItem);
}

bool Player::setItem(size_t invIdx, size_t itemIdx, std::unique_ptr<Item>& item,
	std::unique_ptr<Item>& oldItem)
{
	if (invIdx >= inventories.size())
	{
		return false;
	}
	auto itemPtr = item.get();
	auto& inventory = inventories[invIdx];
	auto ret = inventory.set(itemIdx, item, oldItem);
	if (ret == true)
	{
		updateGoldRemove(oldItem.get());
		updateGoldAdd(itemPtr);
		if (bodyInventoryIdx == invIdx)
		{
			updateProperties();
		}
	}
	return ret;
}

bool Player::setItemInFreeSlot(size_t invIdx,
	std::unique_ptr<Item>& item, InventoryPosition invPos)
{
	if (invIdx < inventories.size())
	{
		auto& inventory = inventories[invIdx];
		size_t itemIdx = 0;
		if (inventory.getFreeItemSlot(*item, itemIdx, invPos) == true)
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
	damageMin = 0;
	damageMax = 0;
	damageMinItems = 0;
	damageMaxItems = 0;
	toDamage = 0;
	resistMagicItems = 0;
	resistFireItems = 0;
	resistLightningItems = 0;
	LevelObjValue singleItemDamage = 0;
	int equipedItems = 0;

	if (bodyInventoryIdx >= inventories.size())
	{
		return;
	}
	for (const auto& item : inventories[bodyInventoryIdx])
	{
		equipedItems++;

		if (item.Identified() == true)
		{
			auto allAttributes = item.getIntByHash(ItemProp::AllAttributes);
			strengthItems += allAttributes + item.getIntByHash(ItemProp::Strength);
			magicItems += allAttributes + item.getIntByHash(ItemProp::Magic);
			dexterityItems += allAttributes + item.getIntByHash(ItemProp::Dexterity);
			vitalityItems += allAttributes + item.getIntByHash(ItemProp::Vitality);
		}
	}
	for (const auto& item : inventories[bodyInventoryIdx])
	{
		if (canUseItem(item) == false)
		{
			continue;
		}
		if (item.Identified() == true)
		{
			lifeItems += item.getIntByHash(ItemProp::Life);
			manaItems += item.getIntByHash(ItemProp::Mana);

			auto resistAll = item.getIntByHash(ItemProp::ResistAll);
			resistMagicItems += resistAll + item.getIntByHash(ItemProp::ResistMagic);
			resistFireItems += resistAll + item.getIntByHash(ItemProp::ResistFire);
			resistLightningItems += resistAll + item.getIntByHash(ItemProp::ResistLightning);

			toArmor += item.getIntByHash(ItemProp::ToArmor);
			toDamage += item.getIntByHash(ItemProp::ToDamage);

			auto damage = item.getIntByHash(ItemProp::Damage);
			damageMin += damage;
			damageMax += damage;
		}
		armorItems += item.getIntByHash(ItemProp::Armor);
		toHitItems += item.getIntByHash(ItemProp::ToHit);
		damageMinItems += item.getIntByHash(ItemProp::DamageMin);
		damageMaxItems += item.getIntByHash(ItemProp::DamageMax);

		if (equipedItems == 1)
		{
			singleItemDamage = item.getIntByHash(ItemProp::SingleItemDamage);
		}
	}
	auto toArmorPercentage = (float)toArmor * 0.01f;
	armor += (LevelObjValue)(armorItems * toArmorPercentage);

	auto toDamagePercentage = 1.f + ((float)toDamage * 0.01f);
	if (damageMinItems > 0)
	{
		damageMin += (LevelObjValue)(damageMinItems * toDamagePercentage);
	}
	else if (damageMin <= 0)
	{
		Number32 defaultDamage;
		if (getCustomIntByHash(ItemProp::DefaultDamageMin, defaultDamage))
		{
			damageMin = std::max(0, defaultDamage.getInt32());
		}
		else
		{
			damageMin = 0;
		}
	}
	if (damageMaxItems > 0)
	{
		damageMax += (LevelObjValue)(damageMaxItems * toDamagePercentage);
	}
	else if (damageMax <= 0)
	{
		Number32 defaultDamage;
		if (getCustomIntByHash(ItemProp::DefaultDamageMax, defaultDamage))
		{
			damageMax = defaultDamage.getInt32();
		}
		else
		{
			damageMax = damageMin;
		}
		if (singleItemDamage > 0)
		{
			damageMax += singleItemDamage;
		}
	}
	if (damageMin > damageMax)
	{
		damageMax = damageMin;
	}
}

void Player::updateProperties()
{
	updateBodyItemValues();

	life = class_->getActualLife(*this, 0);
	mana = class_->getActualMana(*this, 0);
	armor += class_->getActualArmor(*this, 0);
	toHit = class_->getActualToHit(*this, 0);

	resistMagic = class_->getActualResistMagic(*this, resistMagicItems);
	resistMagic = std::clamp(resistMagic, 0, class_->MaxResistMagic());
	resistFire = class_->getActualResistFire(*this, resistFireItems);
	resistFire = std::clamp(resistFire, 0, class_->MaxResistFire());
	resistLightning = class_->getActualResistLightning(*this, resistLightningItems);
	resistLightning = std::clamp(resistLightning, 0, class_->MaxResistLightning());

	auto damage = class_->getActualDamage(*this, 0);
	damageMin += damage;
	damageMax += damage;
}

void Player::applyDefaults(const Level& level) noexcept
{
	for (const auto& prop : class_->Defaults())
	{
		setNumberByHash(prop.first, prop.second, &level);
	}
	attackSound = class_->getDefaultAttackSound();
	defendSound = class_->getDefaultDefendSound();
	dieSound = class_->getDefaultDieSound();
	hitSound = class_->getDefaultHitSound();
	walkSound = class_->getDefaultWalkSound();
}

bool Player::canUseItem(const Item& item) const
{
	return (item.getIntByHash(ItemProp::RequiredStrength) <= StrengthNow() &&
		item.getIntByHash(ItemProp::RequiredMagic) <= MagicNow() &&
		item.getIntByHash(ItemProp::RequiredDexterity) <= DexterityNow() &&
		item.getIntByHash(ItemProp::RequiredVitality) <= VitalityNow());
}

bool Player::hasEquipedItemType(const std::string& type) const
{
	if (bodyInventoryIdx < inventories.size())
	{
		for (const auto& item : inventories[bodyInventoryIdx])
		{
			if (item.ItemType() == type)
			{
				return true;
			}
		}
	}
	return false;
}

bool Player::hasEquipedItemSubType(const std::string& type) const
{
	if (bodyInventoryIdx >= inventories.size())
	{
		for (const auto& item : inventories[bodyInventoryIdx])
		{
			if (item.ItemSubType() == type)
			{
				return true;
			}
		}
	}
	return false;
}

void Player::playSound(int16_t soundIdx)
{
	if (soundIdx < 0)
	{
		return;
	}
	auto sndBuffer = class_->getSound((size_t)soundIdx);
	if (sndBuffer == nullptr)
	{
		return;
	}
	currentSound.setBuffer(*sndBuffer);
	currentSound.play();
}

void Player::updateExperience(const Level& level)
{
	auto oldLevel = currentLevel;
	currentLevel = level.getLevelFromExperience(experience);
	expNextLevel = level.getExperienceFromLevel(currentLevel);
	if (currentLevel > oldLevel &&
		oldLevel != 0)
	{
		lifeDamage = 0;
		manaDamage = 0;
		Number32 levelUp;
		if (getNumberByHash(ItemProp::LevelUp, levelUp) == true)
		{
			points += levelUp.getUInt32();
			base.queueAction(*class_, str2int16("levelChange"));
		}
	}
}

bool Player::hasMaxStats() const noexcept
{
	return (strength >= class_->MaxStrength() &&
		magic >= class_->MaxMagic() &&
		dexterity >= class_->MaxDexterity() &&
		vitality >= class_->MaxVitality());
}
