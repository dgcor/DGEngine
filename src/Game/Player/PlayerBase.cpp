#include "PlayerBase.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/Utils/GameUtils.h"
#include "PlayerLevelObject.h"
#include "PlayerMove.h"
#include "PlayerSave.h"
#include "Utils/Utils.h"

PlayerBase::PlayerBase(const PlayerClass* class__, const Level& level) : LevelObject(class__)
{
	animation.animType = AnimationType::Looped;
	lightSource = class__->getLightSource();
	cellSize.x = -2;
	cellSize.y = -2;
	sprite.setOutline(class__->Outline(), class__->OutlineIgnore());
	updateAnimation();
	applyDefaults(level);
}

void PlayerBase::applyDefaults(const Level& level) noexcept
{
	for (const auto& prop : Class()->Defaults())
	{
		setNumberByHash(prop.first, prop.second, &level);
	}
}

void PlayerBase::serialize(const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props) const
{
	PlayerSave::serialize(*this, level, serializeObj, props);
}

void PlayerBase::updateAnimation()
{
	Class()->setAnimation(
		textureIdx, playerAnimation, (uint32_t)playerDirection, animation
	);
}

void PlayerBase::updateSpeed()
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

void PlayerBase::setWalkPath(const std::vector<PairFloat>& walkPath_, bool doAction)
{
	PlayerMove::setWalkPath(*this, walkPath_, doAction);
}

void PlayerBase::Walk(const LevelMap& map, const PairFloat& walkToMapPos, bool doAction)
{
	setWalkPath(map.getPath(mapPosition, walkToMapPos), doAction);
}

void PlayerBase::Walk(const LevelMap& map, const PlayerDirection direction, bool doAction)
{
	PlayerMove::Walk(*this, map, direction, doAction);
}

void PlayerBase::setDefaultSpeed(const AnimationSpeed& speed_)
{
	defaultSpeed = speed_;
	speed = Class()->getSpeed(playerAnimation);
	animation.elapsedTime.timeout = speed.animation;
	updateSpeed();
}

void PlayerBase::setDirection(PlayerDirection direction_)
{
	if (playerDirection != direction_)
	{
		playerDirection = direction_;
		updateAnimation();
	}
}

void PlayerBase::setAnimation(PlayerAnimation animation_)
{
	if (playerAnimation != animation_)
	{
		playerAnimation = animation_;
		speed = Class()->getSpeed(playerAnimation);
		animation.elapsedTime.timeout = speed.animation;
		updateSpeed();
		updateAnimation();
	}
}

void PlayerBase::setTextureIdx(size_t idx_)
{
	if (textureIdx != idx_)
	{
		textureIdx = idx_;
		updateAnimation();
	}
}

void PlayerBase::setRestStatus(uint16_t restStatus_) noexcept
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

void PlayerBase::setStatus(PlayerStatus status_) noexcept
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

bool PlayerBase::MapPosition(LevelMap& map, const PairFloat& pos)
{
	return PlayerMove::MapPosition(*this, map, pos);
}

bool PlayerBase::move(LevelMap& map, const PairFloat& pos)
{
	return PlayerMove::move(*this, map, pos);
}

bool PlayerBase::getProperty(const std::string_view prop, Variable& var) const
{
	return PlayerLevelObject::getProperty(*this, prop, var);
}

bool PlayerBase::setProperty(const std::string_view prop, const Variable& var)
{
	return PlayerLevelObject::setProperty(*this, prop, var);
}

QueryObject PlayerBase::getQueryable(const std::string_view prop) const
{
	return PlayerLevelObject::getQueryable(*this, prop);
}

std::vector<VarOrQueryObject> PlayerBase::getQueryableList(const std::string_view prop) const
{
	return PlayerLevelObject::getQueryableList(*this, prop);
}

bool PlayerBase::getNumber(const std::string_view prop, Number32& value) const
{
	return PlayerLevelObject::getNumber(*this, prop, value);
}

bool PlayerBase::getNumberByHash(const Queryable& owner, uint16_t propHash, LevelObjValue& value) const
{
	return PlayerLevelObject::getNumberByHash(*this, propHash, value);
}

bool PlayerBase::getNumberByHash(uint16_t propHash, const std::string_view props, Number32& value) const noexcept
{
	return PlayerLevelObject::getNumberByHash(*this, propHash, props, value);
}

bool PlayerBase::getIntByHash(uint16_t propHash, LevelObjValue& value) const noexcept
{
	return properties.getIntByHash(*Class(), propHash, value);
}

bool PlayerBase::getUIntByHash(uint16_t propHash, uint32_t& value) const noexcept
{
	return properties.getUIntByHash(propHash, value);
}

bool PlayerBase::getUInt(const std::string_view prop, uint32_t& value) const noexcept
{
	return getUIntByHash(str2int16(prop), value);
}

bool PlayerBase::setIntByHash(uint16_t propHash, LevelObjValue value) noexcept
{
	return properties.setIntByHash(*Class(), propHash, value);
}

bool PlayerBase::setUIntByHash(uint16_t propHash, uint32_t value, const Level* level) noexcept
{
	return properties.setUIntByHash(*this, propHash, value, level);
}

bool PlayerBase::setUInt(const std::string_view prop, uint32_t value, const Level* level) noexcept
{
	return setUIntByHash(str2int16(prop), value, level);
}

bool PlayerBase::setNumber(const std::string_view prop, const Number32& value, const Level* level) noexcept
{
	return setNumberByHash(str2int16(prop), value, level);
}

bool PlayerBase::setNumberByHash(uint16_t propHash, const Number32& value) noexcept
{
	return properties.setNumberByHash(*this, propHash, value, nullptr);
}

bool PlayerBase::setNumberByHash(uint16_t propHash, const Number32& value, const Level* level) noexcept
{
	return properties.setNumberByHash(*this, propHash, value, level);
}

std::shared_ptr<Item> PlayerBase::SelectedItem(std::shared_ptr<Item> item) noexcept
{
	return inventories.SelectedItem(*this, item);
}

bool PlayerBase::hasEquipedItemType(const std::string_view type) const
{
	return inventories.hasEquipedItemType(type);
}

bool PlayerBase::hasEquipedItemSubType(const std::string_view type) const
{
	return inventories.hasEquipedItemSubType(type);
}

bool PlayerBase::getFreeItemSlot(const Item& item, size_t& invIdx, size_t& itemIdx, InventoryPosition invPos) const
{
	return inventories.getFreeSlot(item, invIdx, itemIdx, invPos);
}

bool PlayerBase::hasFreeItemSlot(const Item& item) const
{
	return inventories.hasFreeSlot(item);
}

bool PlayerBase::setItem(PlayerInventory inv, size_t itemIdx, std::shared_ptr<Item>& item)
{
	return setItem((size_t)inv, itemIdx, item);
}

bool PlayerBase::setItem(PlayerInventory inv, size_t itemIdx, std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem)
{
	return setItem((size_t)inv, itemIdx, item, oldItem);
}

bool PlayerBase::setItem(size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item)
{
	return inventories.setItem(*this, invIdx, itemIdx, item);
}

bool PlayerBase::setItem(size_t invIdx, size_t itemIdx, std::shared_ptr<Item>& item, std::shared_ptr<Item>& oldItem)
{
	return inventories.setItem(*this, invIdx, itemIdx, item, oldItem);
}

bool PlayerBase::setItemInFreeSlot(PlayerInventory inv, std::shared_ptr<Item>& item, InventoryPosition invPos, bool splitIntoMultipleQuantities)
{
	return setItemInFreeSlot((size_t)inv, item, invPos, splitIntoMultipleQuantities);
}

bool PlayerBase::setItemInFreeSlot(size_t invIdx, std::shared_ptr<Item>& item, InventoryPosition invPos, bool splitIntoMultiple)
{
	return inventories.setItemInFreeSlot(*this, invIdx, item, invPos, splitIntoMultiple);
}

LevelObjValue PlayerBase::addItemQuantity(const ItemClass& itemClass, const LevelObjValue amount, InventoryPosition invPos)
{
	return inventories.addQuantity(itemClass, amount, invPos, this);
}

uint32_t PlayerBase::getMaxItemCapacity(const ItemClass& itemClass) const
{
	return inventories.getMaxCapacity(itemClass);
}

void PlayerBase::SelectedSpell(const std::string_view id) noexcept
{
	spells.SelectedSpell(id);
}

bool PlayerBase::hasSpell(const std::string_view key) const
{
	return spells.hasSpell(key);
}

void PlayerBase::addSpell(const std::string_view key, Spell* spell, LevelObjValue spellLevel)
{
	spells.addSpell(key, spell, this, spellLevel);
}

Spell* PlayerBase::getSpell(const std::string_view key) const
{
	return spells.getSpell(key);
}

const SpellInstance* PlayerBase::getSpellInstance(const std::string_view key) const
{
	return spells.getSpellInstance(key);
}

void PlayerBase::playSound(const std::string_view key, size_t soundNum)
{
	auto sndBuffer = Class()->getSound(key, soundNum);
	if (sndBuffer == nullptr)
	{
		return;
	}
	currentSound.setBuffer(*sndBuffer);
	currentSound.play();
}

void PlayerBase::updateLevelFromExperience(const Level& level, bool updatePoints)
{
	auto oldLevel = properties.currentLevel;
	properties.currentLevel = level.getLevelFromExperience(properties.experience);
	properties.expNextLevel = level.getExperienceFromLevel(properties.currentLevel);
	if (updatePoints == true &&
		properties.currentLevel > oldLevel &&
		oldLevel != 0)
	{
		properties.lifeDamage = 0;
		properties.manaDamage = 0;
		Number32 levelUp;
		if (getNumberByHash(ItemProp::LevelUp, {}, levelUp) == true)
		{
			properties.points += levelUp.getUInt32() * (properties.currentLevel - oldLevel);
			queueAction(*class_, str2int16("levelChange"));
		}
	}
}

bool PlayerBase::canUseObject(const LevelObjectQueryable& obj) const
{
	LevelObjValue value = 0;
	if (obj.getNumberByHash(*this, ItemProp::RequiredStrength, value) == true &&
		value > properties.StrengthNow())
	{
		return false;
	}
	if (obj.getNumberByHash(*this, ItemProp::RequiredMagic, value) == true &&
		value > properties.MagicNow())
	{
		return false;
	}
	if (obj.getNumberByHash(*this, ItemProp::RequiredDexterity, value) == true &&
		value > properties.DexterityNow())
	{
		return false;
	}
	if (obj.getNumberByHash(*this, ItemProp::RequiredVitality, value) == true &&
		value > properties.VitalityNow())
	{
		return false;
	}
	if (obj.getNumberByHash(*this, ItemProp::RequiredLife, value) == true &&
		value > properties.LifeNow())
	{
		return false;
	}
	if (obj.getNumberByHash(*this, ItemProp::RequiredMana, value) == true &&
		value > properties.ManaNow())
	{
		return false;
	}
	return true;
}
