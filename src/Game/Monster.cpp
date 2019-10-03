#include "Monster.h"
#include "Player.h"
#include "Game.h"
#include "GameUtils.h"
#include "Level.h"
#include "Utils/Utils.h"

Monster::Monster(const MonsterClass* class__, const Level& level) : LevelObject(class__)
{
	animation.animType = AnimationType::Looped;
	lightSource = class__->getLightSource();
	cellSize.x = -2;
	cellSize.y = -2;
	sprite.setOutline(class__->Outline(), class__->OutlineIgnore());
	calculateRange();
	applyDefaults(level);
}

void Monster::calculateRange()
{
	animation.setTexturePack(Class()->getTexturePack(textureIdx));
	Class()->getTextureAnimationRange(
		textureIdx, playerAnimation, (uint32_t)playerDirection, animation
	);
}

void Monster::updateSpeed()
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

void Monster::updateWalkPathStep(PairFloat& newMapPos)
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

void Monster::updateWalkPath(Game& game, LevelMap& map)
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

void Monster::setWalkPath(const std::vector<PairFloat>& walkPath_, bool doAction)
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

void Monster::Walk(const LevelMap& map, const PairFloat& walkToMapPos, bool doAction)
{
	setWalkPath(map.getPath(mapPosition, walkToMapPos), doAction);
}

void Monster::Walk(const LevelMap& map, const PlayerDirection direction, bool doAction)
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

void Monster::setDefaultSpeed(const AnimationSpeed& speed_)
{
	defaultSpeed = speed_;

	speed = Class()->getSpeed(playerAnimation);
	animation.elapsedTime.timeout = speed.animation;
	updateSpeed();
}

void Monster::setDirection(PlayerDirection direction_)
{
	if (playerDirection != direction_)
	{
		playerDirection = direction_;
		calculateRange();
	}
}

void Monster::setAnimation(PlayerAnimation animation_)
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

void Monster::setTextureIdx(size_t idx_)
{
	if (textureIdx != idx_)
	{
		textureIdx = idx_;
		calculateRange();
	}
}

void Monster::setRestStatus(uint16_t restStatus_) noexcept
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

void Monster::setStatus(PlayerStatus status_) noexcept
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

bool Monster::getTexture(uint32_t textureNumber, TextureInfo& ti) const
{
	switch (textureNumber)
	{
	case 0:
		return getCurrentTexture(ti);
	default:
		return false;
	}
}

bool Monster::MapPosition(LevelMap& map, const PairFloat& pos)
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

bool Monster::move(LevelMap& map, const PairFloat& pos)
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

void Monster::updateAI(Level& level)
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

void Monster::updateWalk(Game& game, Level& level)
{
	updateWalkPath(game, level.Map());
	updateAnimation(game);
}

void Monster::updateAttack(Game& game, Level& level)
{
	updateAnimation(game);
}

void Monster::updateDead(Game& game, Level& level)
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

void Monster::updateAnimation(const Game& game)
{
	if (animation.update(game.getElapsedTime()))
	{
		updateTexture();
	}
}

void Monster::update(Game& game, Level& level, std::weak_ptr<LevelObject> thisPtr)
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

const std::string& Monster::Name() const
{
	updateNameAndDescriptions();
	return name;
}

bool Monster::getProperty(const std::string_view prop, Variable& var) const
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

void Monster::setProperty(const std::string_view prop, const Variable& val)
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

const Queryable* Monster::getQueryable(const std::string_view prop) const
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

bool Monster::hasIntByHash(uint16_t propHash) const noexcept
{
	return customProperties.hasValue(propHash);
}

bool Monster::hasInt(const std::string_view prop) const noexcept
{
	return hasIntByHash(str2int16(prop));
}

bool Monster::getNumberProp(const std::string_view prop, Number32& value) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	return getNumberByHash(propHash, props.second, value);
}

bool Monster::getNumberByHash(uint16_t propHash,
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

bool Monster::getCustomIntByHash(uint16_t propHash, Number32& value) const noexcept
{
	return customProperties.getValue(propHash, value);
}

bool Monster::getCustomInt(const std::string_view prop, Number32& value) const noexcept
{
	return getCustomIntByHash(str2int16(prop), value);
}

bool Monster::getIntByHash(uint16_t propHash, LevelObjValue& value) const noexcept
{
	switch (propHash)
	{
	case str2int16("life"):
		value = life;
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
	case str2int16("manaDamage"):
		value = manaDamage;
		break;
	case str2int16("manaNow"):
		value = ManaNow();
		break;
	case str2int16("armor"):
		value = armor;
		break;
	case str2int16("toArmor"):
		value = toArmor;
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
	default:
		return false;
	}
	return true;
}

bool Monster::getInt(const std::string_view prop, LevelObjValue& value) const noexcept
{
	return getIntByHash(str2int16(prop), value);
}

bool Monster::getUIntByHash(uint16_t propHash, uint32_t& value) const noexcept
{
	switch (propHash)
	{
	case str2int16("level"):
		value = currentLevel;
		break;
	default:
		return false;
	}
	return true;
}

bool Monster::getUInt(const std::string_view prop, uint32_t& value) const noexcept
{
	return getUIntByHash(str2int16(prop), value);
}

bool Monster::setIntByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept
{
	switch (propHash)
	{
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

bool Monster::setInt(const std::string_view prop, LevelObjValue value, const Level* level) noexcept
{
	return setIntByHash(str2int16(prop), value, level);
}

bool Monster::setUIntByHash(uint16_t propHash, uint32_t value, const Level* level) noexcept
{
	// TODO
	switch (propHash)
	{
	default:
		break;
	}
	updateNameAndDescr = true;
	return true;
}

bool Monster::setUInt(const std::string_view prop, uint32_t value, const Level* level) noexcept
{
	return setUIntByHash(str2int16(prop), value, level);
}

bool Monster::setNumberByHash(uint16_t propHash, LevelObjValue value, const Level* level) noexcept
{
	if (setIntByHash(propHash, value, nullptr) == false)
	{
		return setUIntByHash(propHash, (uint32_t)value, level);
	}
	return true;
}

bool Monster::setNumber(const std::string_view prop, LevelObjValue value, const Level* level) noexcept
{
	return setNumberByHash(str2int16(prop), value, level);
}

bool Monster::setNumber(const std::string_view prop, const Number32& value, const Level* level) noexcept
{
	return setNumberByHash(str2int16(prop), value, level);
}

bool Monster::setNumberByHash(uint16_t propHash, const Number32& value, const Level* level) noexcept
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

void Monster::updateNameAndDescriptions() const
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

bool Monster::hasSpell(const std::string& key) const
{
	return spells.find(key) != spells.end();
}

void Monster::addSpell(const std::string key, Spell* obj, LevelObjValue spellLevel)
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

Spell* Monster::getSpell(const std::string& key) const
{
	auto it = spells.find(key);
	if (it != spells.end())
	{
		return it->second.spell;
	}
	return nullptr;
}

const SpellInstance* Monster::getSpellInstance(const std::string& key) const
{
	auto it = spells.find(key);
	if (it != spells.end())
	{
		return &it->second;
	}
	return nullptr;
}

void Monster::SelectedSpell(const std::string& id) noexcept
{
	selectedSpell = getSpellInstance(id);
}

void Monster::updateBodyItemValues()
{
	armor = 0;
	toArmor = 0;
	damageMin = 0;
	damageMax = 0;
	toDamage = 0;
	resistMagicItems = 0;
	resistFireItems = 0;
	resistLightningItems = 0;
	lightSource = getDefaultLightSource();
	LevelObjValue toLightRadius = 0;
	LevelObjValue singleItemDamage = 0;
}

void Monster::updateProperties()
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

void Monster::applyDefaults(const Level& level) noexcept
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

void Monster::playSound(int16_t soundIdx)
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
