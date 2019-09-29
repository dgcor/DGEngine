#include "Player.h"
#include "Game.h"
#include "GameUtils.h"
#include "Level.h"
#include "Utils/Utils.h"

Player::Player(const PlayerClass* class__, const Level& level) : LevelObject(class__)
{
	animation.animType = AnimationType::Looped;
	lightSource = class__->getLightSource();
	cellSize.x = -2;
	cellSize.y = -2;
	sprite.setOutline(class__->Outline(), class__->OutlineIgnore());
	calculateRange();
	applyDefaults(level);
}

void Player::calculateRange()
{
	animation.setTexturePack(Class()->getTexturePack(textureIdx));
	Class()->getTextureAnimationRange(
		textureIdx, playerAnimation, (uint32_t)playerDirection, animation
	);
}

void Player::updateSpeed()
{
	if (hasWalkingAnimation() == true)
	{
		if (defaultSpeed.animation != sf::Time::Zero)
		{
			speed.animation = defaultSpeed.animation;
			animation.elapsedTime.timeout = speed.animation;
		}
		if (defaultSpeed.walk != sf::Time::Zero)
		{
			speed.walk = defaultSpeed.walk;
		}
	}
}

void Player::updateWalkPathStep(PairFloat& newMapPos)
{
	newMapPos.x -= (mapPosA.x - mapPosB.x) * currPositionStep;
	newMapPos.y -= (mapPosA.y - mapPosB.y) * currPositionStep;

	if (currPositionStep >= 1.f)
	{
		if (walkPath.empty() == false)
		{
			walkPath.pop_back();
		}
		mapPosA = mapPosB;
		newMapPos = mapPosB;
	}
	else
	{
		currPositionStep += positionStep;
	}
}

void Player::updateWalkPath(Game& game, LevelMap& map)
{
	currentWalkTime += game.getElapsedTime();

	while (currentWalkTime >= speed.walk)
	{
		currentWalkTime -= speed.walk;

		auto newMapPos = mapPosA;
		if (mapPosA == mapPosB)
		{
			if (walkPath.empty() == true &&
				hasWalkingAnimation() == true)
			{
				setStandAnimation();
				resetAnimationTime();
				playerStatus = PlayerStatus::Stand;
			}
			while (walkPath.empty() == false)
			{
				const auto& nextMapPos = walkPath.back();
				if (walkPath.size() == 1)
				{
					const auto levelObj = map[nextMapPos].front();
					if (levelObj != nullptr)
					{
						if (executeActionOnDestination == true)
						{
							levelObj->executeAction(game);
						}
						walkPath.pop_back();

						setStandAnimation();
						resetAnimationTime();
						playerStatus = PlayerStatus::Stand;
						return;
					}
				}
				if (nextMapPos == mapPosition)
				{
					walkPath.pop_back();
					continue;
				}
				playSound(walkSound);
				setWalkAnimation();
				setDirection(getPlayerDirection(mapPosition, nextMapPos));
				MapPosition(map, nextMapPos);
				positionStep = 1.6f / map.DefaultTileWidth();
				currPositionStep = positionStep;
				updateWalkPathStep(newMapPos);
				break;
			}
		}
		else
		{
			updateWalkPathStep(newMapPos);
		}
		LevelObject::updateDrawPosition(map, newMapPos);
	}
}

void Player::setWalkPath(const std::vector<PairFloat>& walkPath_, bool doAction)
{
	if (walkPath_.empty() == true ||
		playerStatus == PlayerStatus::Dead)
	{
		return;
	}
	walkPath = walkPath_;
	executeActionOnDestination = doAction;
	playerStatus = PlayerStatus::Walk;
	if (walkPath.empty() == false)
	{
		mapPositionMoveTo = walkPath.front();
	}
}

void Player::Walk(const LevelMap& map, const PairFloat& walkToMapPos, bool doAction)
{
	setWalkPath(map.getPath(mapPosition, walkToMapPos), doAction);
}

void Player::Walk(const LevelMap& map, const PlayerDirection direction, bool doAction)
{
	PairFloat a = mapPosition;
	PairFloat b = a;
	switch (direction)
	{
	case PlayerDirection::Front:
		b.x++;
		b.y++;
		break;
	case PlayerDirection::FrontLeft:
		b.y++;
		break;
	case PlayerDirection::Left:
		b.x--;
		b.y++;
		break;
	case PlayerDirection::BackLeft:
		b.x--;
		break;
	case PlayerDirection::Back:
		b.x--;
		b.y--;
		break;
	case PlayerDirection::BackRight:
		b.y--;
		break;
	case PlayerDirection::Right:
		b.x++;
		b.y--;
		break;
	case PlayerDirection::FrontRight:
		b.x++;
		break;
	default:
		break;
	}
	if (a != b &&
		map.isMapCoordValid(b) == true &&
		map[b].Passable() == true)
	{
		std::vector<PairFloat> path;
		path.push_back(b);
		path.push_back(a);
		setWalkPath(path, doAction);
	}
}

void Player::setDefaultSpeed(const AnimationSpeed& speed_)
{
	defaultSpeed = speed_;
	speed = Class()->getSpeed(playerAnimation);
	animation.elapsedTime.timeout = speed.animation;
	updateSpeed();
}

void Player::setDirection(PlayerDirection direction_)
{
	if (playerDirection != direction_)
	{
		playerDirection = direction_;
		calculateRange();
	}
}

void Player::setAnimation(PlayerAnimation animation_)
{
	if (playerAnimation != animation_)
	{
		playerAnimation = animation_;
		speed = Class()->getSpeed(playerAnimation);
		animation.elapsedTime.timeout = speed.animation;
		updateSpeed();
		calculateRange();
	}
}

void Player::setTextureIdx(size_t idx_)
{
	if (textureIdx != idx_)
	{
		textureIdx = idx_;
		calculateRange();
	}
}

void Player::setRestStatus(uint16_t restStatus_) noexcept
{
	restStatus = std::min(restStatus_, (uint16_t)1);
	switch (playerStatus)
	{
	case PlayerStatus::Stand:
		setStandAnimation();
		break;
	case PlayerStatus::Walk:
		setWalkAnimation();
		break;
	default:
		break;
	}
}

void Player::setStatus(PlayerStatus status_) noexcept
{
	playerStatus = status_;
	switch (playerStatus)
	{
	case PlayerStatus::Dead:
	{
		animation.currentTextureIdx = animation.textureIndexRange.second;
		break;
	}
	default:
		break;
	}
}

bool Player::getTexture(uint32_t textureNumber, TextureInfo& ti) const
{
	switch (textureNumber)
	{
	case 0:
		return getCurrentTexture(ti);
	default:
		return false;
	}
}

bool Player::MapPosition(LevelMap& map, const PairFloat& pos)
{
	mapPosA = mapPosition;
	mapPosB = pos;
	bool success = false;
	if (mapPosition != pos)
	{
		success = updateMapPositionBack(map, pos);
	}
	if (success == true || map.isMapCoordValid(pos) == true)
	{
		updateDrawPosition(map);
	}
	return success;
}

bool Player::move(LevelMap& map, const PairFloat& pos)
{
	if (mapPosition == pos ||
		playerStatus == PlayerStatus::Dead ||
		map.isMapCoordValid(pos) == false)
	{
		return false;
	}
	clearWalkPath();
	setStandAnimation();
	playerStatus = PlayerStatus::Stand;
	resetAnimationTime();
	mapPosA = mapPosB = pos;
	bool success = updateMapPositionBack(map, pos);
	if (success == true)
	{
		updateDrawPosition(map);
	}
	return success;
}

void Player::updateAI(Level& level)
{
	switch (playerStatus)
	{
	case PlayerStatus::Walk:
		return;
	default:
		break;
	}
	auto plr = level.getCurrentPlayer();
	if (plr != nullptr)
	{
		setWalkPath(level.Map().getPath(mapPosition, plr->MapPosition()), false);
	}
}

void Player::updateWalk(Game& game, Level& level)
{
	updateWalkPath(game, level.Map());
	updateAnimation(game);
}

void Player::updateAttack(Game& game, Level& level)
{
	updateAnimation(game);
}

void Player::updateDead(Game& game, Level& level)
{
	if (playerAnimation != PlayerAnimation::Die1)
	{
		animation.currentTextureIdx = 0;
		setAnimation(PlayerAnimation::Die1);
	}
	if (animation.currentTextureIdx >= animation.textureIndexRange.second)
	{
		return;
	}
	updateAnimation(game);
}

void Player::updateAnimation(const Game& game)
{
	if (animation.update(game.getElapsedTime()))
	{
		updateTexture();
	}
}

void Player::update(Game& game, Level& level, std::weak_ptr<LevelObject> thisPtr)
{
	processQueuedActions(game);

	if (hasValidState() == false)
	{
		return;
	}
	if (useAI == true)
	{
		updateAI(level);
	}

	if (playerStatus != PlayerStatus::Dead &&
		LifeNow() <= 0)
	{
		playerStatus = PlayerStatus::Dead;
		playSound(dieSound);
	}

	switch (playerStatus)
	{
	default:
	case PlayerStatus::Stand:
		updateAnimation(game);
		break;
	case PlayerStatus::Walk:
		updateWalk(game, level);
		break;
	case PlayerStatus::Attack:
		updateAttack(game, level);
		break;
	case PlayerStatus::Dead:
		updateDead(game, level);
		break;
	}

	updateHover(game, level, thisPtr);
}

const std::string& Player::Name() const
{
	updateNameAndDescriptions();
	return name;
}

bool Player::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	if (getLevelObjProp(propHash, props.second, var) == true)
	{
		return true;
	}
	switch (propHash)
	{
	case str2int16("name"):
		var = Variable(Name());
		break;
	case str2int16("simpleName"):
		var = Variable(SimpleName());
		break;
	case str2int16("d"):
	case str2int16("description"):
	{
		updateNameAndDescriptions();
		size_t idx = Utils::strtou(props.second);
		if (idx >= descriptions.size())
		{
			idx = 0;
		}
		var = Variable(descriptions[idx]);
		break;
	}
	case str2int16("eval"):
		var = Variable((int64_t)Formula::evalString(props.second, *this));
		break;
	case str2int16("evalMin"):
		var = Variable((int64_t)Formula::evalMinString(props.second, *this));
		break;
	case str2int16("evalMax"):
		var = Variable((int64_t)Formula::evalMaxString(props.second, *this));
		break;
	case str2int16("evalf"):
		var = Variable(Formula::evalString(props.second, *this));
		break;
	case str2int16("evalMinf"):
		var = Variable(Formula::evalMinString(props.second, *this));
		break;
	case str2int16("evalMaxf"):
		var = Variable(Formula::evalMaxString(props.second, *this));
		break;
	case str2int16("totalKills"):
		var = Variable((int64_t)Class()->TotalKills());
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
		var = Variable(canUseObject(*selectedItem));
		break;
	}
	case str2int16("canUseSelectedSpell"):
	{
		if (selectedSpell == nullptr)
		{
			return false;
		}
		var = Variable(canUseObject(*selectedSpell->spell));
		break;
	}
	case str2int16("canUseItem"):
	{
		std::string_view props2;
		size_t invIdx;
		size_t itemIdx;
		if (inventories.parseInventoryAndItem(
			props.second, props2, invIdx, itemIdx) == true)
		{
			auto item = inventories[invIdx].get(itemIdx);
			if (item != nullptr)
			{
				var = Variable(canUseObject(*item));
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
		std::string_view props2;
		size_t invIdx;
		size_t itemIdx;
		if (inventories.parseInventoryAndItem(
			props.second, props2, invIdx, itemIdx) == true)
		{
			var = Variable(inventories[invIdx].get(itemIdx) != nullptr);
			break;
		}
		return false;
	}
	case str2int16("hasItemClass"):
		var = Variable(inventories.hasItem(str2int16(props.second)));
		break;
	case str2int16("hasSpell"):
	{
		var = Variable(getSpell(std::string(props.second)) != nullptr);
		break;
	}
	case str2int16("hasSelectedSpell"):
	{
		var = Variable(selectedSpell != nullptr);
		break;
	}
	case str2int16("isItemSlotInUse"):
	{
		std::string_view props2;
		size_t invIdx;
		size_t itemIdx;
		if (inventories.parseInventoryAndItem(
			props.second, props2, invIdx, itemIdx) == true)
		{
			var = Variable(inventories[invIdx].isSlotInUse(itemIdx));
			break;
		}
		return false;
	}
	case str2int16("isStanding"):
		var = Variable(playerStatus == PlayerStatus::Stand);
		break;
	case str2int16("isWalking"):
		var = Variable(playerStatus == PlayerStatus::Walk);
		break;
	case str2int16("isAttacking"):
		var = Variable(playerStatus == PlayerStatus::Attack);
		break;
	case str2int16("isDead"):
		var = Variable(playerStatus == PlayerStatus::Dead);
		break;
	case str2int16("selectedItem"):
	{
		if (selectedItem != nullptr)
		{
			return selectedItem->getProperty(props.second, var);
		}
		return false;
	}
	case str2int16("item"):
	{
		std::string_view props2;
		size_t invIdx;
		size_t itemIdx;
		if (inventories.parseInventoryAndItem(
			props.second, props2, invIdx, itemIdx) == true)
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
	case str2int16("itemQuantity"):
	{
		auto classIdHash16 = str2int16(props.second);
		uint32_t itemQuantity = 0;
		if (itemQuantityCache.getValue(classIdHash16, itemQuantity) == false)
		{
			if (inventories.getQuantity(classIdHash16, itemQuantity) == true)
			{
				itemQuantityCache.updateValue(classIdHash16, itemQuantity);
			}
		}
		var = Variable((int64_t)itemQuantity);
		break;
	}
	case str2int16("selectedSpell"):
	{
		if (selectedSpell != nullptr)
		{
			return selectedSpell->getProperty(props.second, var);
		}
		return false;
	}
	case str2int16("spell"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto spell = getSpellInstance(std::string(props2.first));
		if (spell != nullptr)
		{
			return spell->getProperty(props2.second, var);
		}
		return false;
	}
	default:
	{
		Number32 value;
		if (getNumberByHash(propHash, props.second, value) == true)
		{
			var = Variable(value.getInt64());
			break;
		}
		return false;
	}
	}
	return true;
}

void Player::setProperty(const std::string_view prop, const Variable& val)
{
	if (prop.empty() == true)
	{
		return;
	}
	auto propHash16 = str2int16(prop);
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

const Queryable* Player::getQueryable(const std::string_view prop) const
{
	if (prop.empty() == true)
	{
		return this;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	const Queryable* queryable = nullptr;
	switch (propHash)
	{
	case str2int16("selectedItem"):
		queryable = selectedItem.get();
		break;
	case str2int16("item"):
	{
		size_t invIdx;
		size_t itemIdx;
		if (inventories.parseInventoryAndItem(
			props.second, props.second, invIdx, itemIdx) == true)
		{
			queryable = inventories[invIdx].get(itemIdx);
			break;
		}
	}
	break;
	case str2int16("selectedSpell"):
		queryable = selectedSpell;
		break;
	case str2int16("spell"):
		return getSpellInstance(std::string(props.second));
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

std::vector<std::variant<const Queryable*, Variable>> Player::getQueryableList(
	const std::string_view prop) const
{
	std::vector<std::variant<const Queryable*, Variable>> queriableList;

	auto props = Utils::splitStringIn2(prop, '.');
	if (props.first.empty() == false)
	{
		auto invIdx = GameUtils::getPlayerInventoryIndex(props.second);
		if (invIdx < inventories.size())
		{
			const auto& inventory = inventories[invIdx];
			for (size_t i = 0; i < inventory.Size(); i++)
			{
				if (inventory.isSlotInUse(i) == false)
				{
					queriableList.push_back({ nullptr });
				}
				else
				{
					queriableList.push_back({ inventory.get(i) });
				}
			}
		}
	}
	return queriableList;
}

bool Player::hasIntByHash(uint16_t propHash) const noexcept
{
	return customProperties.hasValue(propHash);
}

bool Player::hasInt(const std::string_view prop) const noexcept
{
	return hasIntByHash(str2int16(prop));
}

bool Player::getNumberProp(const std::string_view prop, Number32& value) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	return getNumberByHash(propHash, props.second, value);
}

bool Player::getNumberByHash(uint16_t propHash,
	const std::string_view props, Number32& value) const noexcept
{
	switch (propHash)
	{
	case str2int16("selectedSpell"):
	{
		if (selectedSpell != nullptr)
		{
			return selectedSpell->getNumberProp(props, value);
		}
		return false;
	}
	case str2int16("spell"):
	{
		auto props2 = Utils::splitStringIn2(props, '.');
		auto spell = getSpellInstance(std::string(props2.first));
		if (spell != nullptr)
		{
			return spell->getNumberProp(props2.second, value);
		}
		return false;
	}
	default:
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
	}
	return true;
}

bool Player::getCustomIntByHash(uint16_t propHash, Number32& value) const noexcept
{
	return customProperties.getValue(propHash, value);
}

bool Player::getCustomInt(const std::string_view prop, Number32& value) const noexcept
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
		value = Class()->MaxStrength();
		break;
	case str2int16("maxMagic"):
		value = Class()->MaxMagic();
		break;
	case str2int16("maxDexterity"):
		value = Class()->MaxDexterity();
		break;
	case str2int16("maxVitality"):
		value = Class()->MaxVitality();
		break;
	case str2int16("maxResistMagic"):
		value = Class()->MaxResistMagic();
		break;
	case str2int16("maxResistFire"):
		value = Class()->MaxResistFire();
		break;
	case str2int16("maxResistLightning"):
		value = Class()->MaxResistLightning();
		break;
	default:
		return false;
	}
	return true;
}

bool Player::getInt(const std::string_view prop, LevelObjValue& value) const noexcept
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
	default:
		return false;
	}
	return true;
}

bool Player::getUInt(const std::string_view prop, uint32_t& value) const noexcept
{
	return getUIntByHash(str2int16(prop), value);
}

bool Player::setIntByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept
{
	switch (propHash)
	{
	case str2int16("strength"):
		strength = std::clamp(value, 0, Class()->MaxStrength());
		break;
	case str2int16("magic"):
		magic = std::clamp(value, 0, Class()->MaxMagic());
		break;
	case str2int16("dexterity"):
		dexterity = std::clamp(value, 0, Class()->MaxDexterity());
		break;
	case str2int16("vitality"):
		vitality = std::clamp(value, 0, Class()->MaxVitality());
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
	updateNameAndDescr = true;
	return true;
}

bool Player::setInt(const std::string_view prop, LevelObjValue value, const Level* level) noexcept
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
			updateLevelFromExperience(*level, true);
		}
		break;
	}
	case str2int16("points"):
		points = value;
		break;
	default:
		return false;
	}
	updateNameAndDescr = true;
	return true;
}

bool Player::setUInt(const std::string_view prop, uint32_t value, const Level* level) noexcept
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

bool Player::setNumber(const std::string_view prop, LevelObjValue value, const Level* level) noexcept
{
	return setNumberByHash(str2int16(prop), value, level);
}

bool Player::setNumber(const std::string_view prop, const Number32& value, const Level* level) noexcept
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
		updateNameAndDescr = true;
		return customProperties.setValue(propHash, value);
	}
	}
}

void Player::updateNameAndDescriptions() const
{
	if (updateNameAndDescr == true)
	{
		updateNameAndDescr = false;
		if (Class()->getFullName(*this, name) == false &&
			name.empty() == true)
		{
			name = SimpleName();
		}
		for (size_t i = 0; i < descriptions.size(); i++)
		{
			Class()->getDescription(i, *this, descriptions[i]);
		}
	}
}

LevelObjValue Player::addItemQuantity(const ItemClass& itemClass,
	const LevelObjValue amount, InventoryPosition invPos)
{
	auto remaining = inventories.addQuantity(itemClass, amount, invPos, this);
	if (amount != 0)
	{
		updateItemQuantityCache(itemClass.IdHash16(), amount - remaining);
	}
	return remaining;
}

void Player::updateItemQuantityCache(uint16_t classIdHash16, LevelObjValue amount) noexcept
{
	auto quantity = itemQuantityCache.getValue(classIdHash16);
	if (quantity != nullptr)
	{
		(*quantity) += amount;
	}
}

void Player::updateItemQuantityCache(uint16_t classIdHash16) noexcept
{
	auto quantity = itemQuantityCache.getValue(classIdHash16);
	if (quantity != nullptr)
	{
		(*quantity) = inventories.getQuantity(classIdHash16);
	}
}

uint32_t Player::getMaxItemCapacity(const ItemClass& itemClass) const
{
	return inventories.getMaxCapacity(itemClass);
}

bool Player::getFreeItemSlot(const Item& item, size_t& invIdx,
	size_t& itemIdx, InventoryPosition invPos) const
{
	return inventories.getFreeSlot(item, invIdx, itemIdx, invPos);
}

bool Player::hasFreeItemSlot(const Item& item) const
{
	return inventories.hasFreeSlot(item);
}

std::shared_ptr<Item> Player::SelectedItem(std::shared_ptr<Item> item) noexcept
{
	auto old = std::move(selectedItem);
	selectedItem = std::move(item);
	if (selectedItem != nullptr)
	{
		selectedItem->clearMapPosition();
		selectedItem->updateOwner(this);
	}
	if (old != nullptr)
	{
		old->updateOwner(nullptr);
	}
	return old;
}

bool Player::hasSpell(const std::string& key) const
{
	return spells.find(key) != spells.end();
}

void Player::addSpell(const std::string key, Spell* obj, LevelObjValue spellLevel)
{
	auto it = spells.find(key);
	if (it != spells.end())
	{
		it->second.spellLevel++;
		return;
	}
	SpellInstance spellInstance(obj, this, spellLevel);
	spells.insert(std::make_pair(key, spellInstance));
}

Spell* Player::getSpell(const std::string& key) const
{
	auto it = spells.find(key);
	if (it != spells.end())
	{
		return it->second.spell;
	}
	return nullptr;
}

const SpellInstance* Player::getSpellInstance(const std::string& key) const
{
	auto it = spells.find(key);
	if (it != spells.end())
	{
		return &it->second;
	}
	return nullptr;
}

void Player::SelectedSpell(const std::string& id) noexcept
{
	selectedSpell = getSpellInstance(id);
}

bool Player::setItem(size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item)
{
	std::shared_ptr<Item> oldItem;
	return setItem(invIdx, itemIdx, item, oldItem);
}

bool Player::setItem(size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item,
	std::shared_ptr<Item>& oldItem)
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
		if (itemPtr != nullptr)
		{
			updateItemQuantityCache(itemPtr->Class()->IdHash16());
			itemPtr->clearMapPosition();
			itemPtr->updateOwner(this);
		}
		else if (oldItem != nullptr)
		{
			updateItemQuantityCache(oldItem->Class()->IdHash16());
			oldItem->updateOwner(nullptr);
		}
		if (bodyInventoryIdx == invIdx)
		{
			updateProperties();
		}
	}
	return ret;
}

bool Player::setItemInFreeSlot(size_t invIdx, std::shared_ptr<Item>& item,
	InventoryPosition invPos, bool splitIntoMultiple)
{
	if (invIdx < inventories.size())
	{
		auto& inventory = inventories[invIdx];

		// if item has the quantity/capacity peoperties
		if (item != nullptr &&
			item->hasIntByHash(ItemProp::Capacity) == true)
		{
			// first, try and fit the item into the smallest existing item of the same class
			auto quantityNeeded = item->getIntByHash(ItemProp::Quantity);
			Item* quantItem;
			if (inventory.findBiggestFreeQuantity(
				item->Class()->IdHash16(), quantItem, quantityNeeded) > 0)
			{
				LevelObjValue transferedQuantity;
				if (Inventory::updateQuantities(
					quantItem, item.get(), transferedQuantity, true) == true)
				{
					updateItemQuantityCache(item->Class()->IdHash16(), transferedQuantity);
					return true;
				}
			}

			// if SplitIntoMultiple is true, try and add to all free items
			// and create new items, if possible (should not create more then 1 item)
			if (splitIntoMultiple == true)
			{
				// add full quantity
				LevelObjValue itemSlots;
				auto freeSlots = inventory.getMaxCapacity(*item->Class());
				if (item->getIntByHash(ItemProp::Quantity, itemSlots) == true &&
					itemSlots >= 0 &&
					(unsigned)itemSlots <= freeSlots)
				{
					inventory.addQuantity(*item->Class(), itemSlots, invPos, this);
					updateItemQuantityCache(item->Class()->IdHash16(), itemSlots);
					return true;
				}
				// if you can't add all of it, add none and return.
				return false;
			}
			// if it doesn't fit into the smallest, try and add it in a free slot
		}

		// try and add item to free slot
		size_t itemIdx = 0;
		if (inventory.getFreeSlot(*item, itemIdx, invPos) == true)
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
	lightSource = getDefaultLightSource();
	LevelObjValue toLightRadius = 0;
	LevelObjValue singleItemDamage = 0;

	if (bodyInventoryIdx >= inventories.size())
	{
		return;
	}
	for (const auto& item : inventories[bodyInventoryIdx])
	{
		if (item.Identified() == true)
		{
			auto allAttributes = item.getIntByHash(ItemProp::AllAttributes);
			strengthItems += allAttributes + item.getIntByHash(ItemProp::Strength);
			magicItems += allAttributes + item.getIntByHash(ItemProp::Magic);
			dexterityItems += allAttributes + item.getIntByHash(ItemProp::Dexterity);
			vitalityItems += allAttributes + item.getIntByHash(ItemProp::Vitality);
		}
		if (canUseObject(item) == false)
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

		toLightRadius += item.getIntByHash(ItemProp::ToLightRadius);

		if (damageMax <= 0)
		{
			singleItemDamage = std::max(singleItemDamage, item.getIntByHash(ItemProp::SingleItemDamage));
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
	if (toLightRadius != 0)
	{
		auto radiusPercentage = (float)std::clamp(toLightRadius, -80, 50) * 0.01f;
		auto newRadius = (float)lightSource.radius;
		newRadius = std::round(newRadius + (newRadius * radiusPercentage));
		lightSource.radius = (uint8_t)std::clamp(newRadius, 1.f, 255.f);
	}
}

void Player::updateProperties()
{
	updateBodyItemValues();

	life = Class()->getActualLife(*this, life);
	mana = Class()->getActualMana(*this, mana);
	armor += Class()->getActualArmor(*this, 0);
	toHit = Class()->getActualToHit(*this, toHit);

	resistMagic = Class()->getActualResistMagic(*this, resistMagicItems);
	resistMagic = std::clamp(resistMagic, 0, Class()->MaxResistMagic());
	resistFire = Class()->getActualResistFire(*this, resistFireItems);
	resistFire = std::clamp(resistFire, 0, Class()->MaxResistFire());
	resistLightning = Class()->getActualResistLightning(*this, resistLightningItems);
	resistLightning = std::clamp(resistLightning, 0, Class()->MaxResistLightning());

	auto damage = Class()->getActualDamage(*this, 0);
	damageMin += damage;
	damageMax += damage;
}

void Player::applyDefaults(const Level& level) noexcept
{
	for (const auto& prop : Class()->Defaults())
	{
		setNumberByHash(prop.first, prop.second, &level);
	}
	attackSound = Class()->getAttackSound();
	defendSound = Class()->getDefendSound();
	dieSound = Class()->getDieSound();
	hitSound = Class()->getHitSound();
	walkSound = Class()->getWalkSound();
}

bool Player::hasEquipedItemType(const std::string_view type) const
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

bool Player::hasEquipedItemSubType(const std::string_view type) const
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
	auto sndBuffer = Class()->getSound((size_t)soundIdx);
	if (sndBuffer == nullptr)
	{
		return;
	}
	currentSound.setBuffer(*sndBuffer);
	currentSound.play();
}

void Player::updateLevelFromExperience(const Level& level, bool updatePoints)
{
	auto oldLevel = currentLevel;
	currentLevel = level.getLevelFromExperience(experience);
	expNextLevel = level.getExperienceFromLevel(currentLevel);
	if (updatePoints == true &&
		currentLevel > oldLevel &&
		oldLevel != 0)
	{
		lifeDamage = 0;
		manaDamage = 0;
		Number32 levelUp;
		if (getNumberByHash(ItemProp::LevelUp, {}, levelUp) == true)
		{
			points += levelUp.getUInt32() * (currentLevel - oldLevel);
			queueAction(*class_, str2int16("levelChange"));
		}
	}
}

bool Player::hasMaxStats() const noexcept
{
	return (strength >= Class()->MaxStrength() &&
		magic >= Class()->MaxMagic() &&
		dexterity >= Class()->MaxDexterity() &&
		vitality >= Class()->MaxVitality());
}
