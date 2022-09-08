#include "Item.h"
#include "Game/Game.h"
#include "Game/GameHashes.h"
#include "Game/Level/Level.h"
#include "Game/Player/Player.h"
#include "Game/Utils/GameUtils.h"
#include "Game/Utils/UIObjectUtils.h"
#include "ItemLevelObject.h"
#include "ItemSave.h"
#include "Utils/Utils.h"

Item::Item(const ItemClass* class__) : LevelObject(class__)
{
	animation.setTexturePack(class__->getDropTexturePack());
	animation.textureIndexRange = class__->getDropTextureIndexRange();
	animation.currentTextureIdx = animation.textureIndexRange.second;
	animation.elapsedTime = class__->AnimationSpeed();
	animation.animType = AnimationType::PlayOnce;
	lightSource = class__->getLightSource();
	cellSize.x = 1;
	cellSize.y = 1;
	sprite.setOutline(class__->Outline(), class__->OutlineIgnore());
	updateOwner(nullptr);
	updateTexture();
	applyDefaults();
}

void Item::applyDefaults()
{
	for (const auto& prop : Class()->Defaults())
	{
		setIntByHash(prop.first, prop.second);
	}
}

void Item::serialize(const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props) const
{
	ItemSave::serialize(*this, level, serializeObj, props);
}

void Item::resetDropAnimation(Level& level) noexcept
{
	resetDropAnimation(level.Map());
}

void Item::resetDropAnimation(LevelMap& map) noexcept
{
	animation.currentTextureIdx = animation.textureIndexRange.first;
	if (enableHover == true)
	{
		wasHoverEnabledOnItemDrop = true;
		enableHover = false;
	}
	if (updateTexture() == true)
	{
		updateDrawPosition(map);
	}
}

bool Item::getTexture(uint32_t textureNumber, TextureInfo& ti) const
{
	return ItemLevelObject::getTexture(*this, textureNumber, ti);
}

bool Item::getNumber(const std::string_view prop, Number32& value) const
{
	return ItemLevelObject::getNumber(*this, prop, value);
}

void Item::update(Game& game, Level& level, const std::shared_ptr<LevelObject>& thisPtr)
{
	ItemLevelObject::update(*this, game, level, thisPtr);
}

bool Item::getProperty(const std::string_view prop, Variable& var) const
{
	return ItemLevelObject::getProperty(*this, prop, var);
}

bool Item::getNumberByHash(const Queryable& owner, uint16_t propHash, LevelObjValue& value) const
{
	return ItemLevelObject::getNumberByHash(*this, owner, propHash, value);
}

bool Item::getNumberByHash(const Queryable& owner, uint16_t propHash, const std::string_view minMaxNumber, LevelObjValue& value) const
{
	return ItemLevelObject::getNumberByHash(*this, owner, propHash, minMaxNumber, value);
}

bool Item::hasPropertyByHash(uint16_t propHash) const noexcept
{
	return properties.hasValue(propHash);
}

bool Item::getIntByHash(uint16_t propHash, LevelObjValue& value) const
{
	return ItemLevelObject::getIntByHash(*this, propHash, value);
}

LevelObjValue Item::getIntByHash(uint16_t propHash) const
{
	return ItemLevelObject::getIntByHash(*this, propHash);
}

bool Item::setNumberByHash(uint16_t propHash, const Number32& value)
{
	return ItemLevelObject::setIntByHash(*this, propHash, value.getInt32());
}

bool Item::setIntByHash(uint16_t propHash, LevelObjValue value)
{
	return ItemLevelObject::setIntByHash(*this, propHash, value);
}

bool Item::needsRecharge() const
{
	LevelObjValue charges = 0;
	if (getIntByHash(ItemProp::ChargesMax, charges) == true)
	{
		if (charges < std::numeric_limits<LevelObjValue>::max())
		{
			return getIntByHash(ItemProp::Charges) < charges;
		}
	}
	return false;
}

bool Item::needsRepair() const
{
	LevelObjValue durability = 0;
	if (getIntByHash(ItemProp::DurabilityMax, durability) == true)
	{
		if (durability < std::numeric_limits<LevelObjValue>::max())
		{
			return getIntByHash(ItemProp::Durability) < durability;
		}
	}
	return false;
}

bool Item::isUsable() const noexcept
{
	return (properties.hasValue(ItemProp::UseOn) || properties.hasValue(ItemProp::UseOp));
}

bool Item::useValue(uint16_t propHash, uint16_t useOpHash, LevelObjValue value, Player& player, const Level& level) const noexcept
{
	Number32 origValue2;
	if (player.getNumberByHash(propHash, {}, origValue2) == false)
	{
		return false;
	}
	auto origValue = origValue2.getInt64();

	switch (useOpHash)
	{
	default:
	case str2int16("+"):
		origValue += value;
		break;
	case str2int16("-"):
		origValue -= value;
		break;
	case str2int16("="):
		origValue = value;
		break;
	}
	return player.setNumberByHash(propHash, Number32((int32_t)origValue), &level);
}

bool Item::getAndUseValue(uint16_t propHash, uint16_t useOpHash, uint16_t valueHash, Player& player, const Level& level) const noexcept
{
	LevelObjValue value;
	if (Class()->evalFormula(valueHash, *this, player, value) == false)
	{
		return false;
	}
	return useValue(propHash, useOpHash, value, player, level);
}

bool Item::useItem(Player& player, const Level& level, uint16_t useOpHash, uint32_t& quantityLeft)
{
	LevelObjValue useOn;
	if (getIntByHash(ItemProp::UseOn, useOn) == false ||
		useOn == str2int16(""))
	{
		return false;
	}

	auto propHash = (uint16_t)useOn;
	bool ret = false;

	switch (propHash)
	{
	case ItemProp::LifeAndManaDamage:
	{
		ret |= getAndUseValue(ItemProp::LifeDamage, useOpHash, ItemProp::Value, player, level);
		ret |= getAndUseValue(ItemProp::ManaDamage, useOpHash, ItemProp::Value2, player, level);
		break;
	}
	case ItemProp::AllAttributes:
	{
		LevelObjValue value;
		if (Class()->evalFormula(ItemProp::Value, *this, player, value) == false)
		{
			break;
		}
		ret |= useValue(ItemProp::Strength, useOpHash, value, player, level);
		ret |= useValue(ItemProp::Magic, useOpHash, value, player, level);
		ret |= useValue(ItemProp::Dexterity, useOpHash, value, player, level);
		ret |= useValue(ItemProp::Vitality, useOpHash, value, player, level);
		break;
	}
	default:
	{
		ret = getAndUseValue(propHash, useOpHash, ItemProp::Value, player, level);
		break;
	}
	}
	return ret;
}

bool Item::useSpell(Player& player, const Level& level, uint16_t useOpHash, uint32_t& quantityLeft)
{
	bool ret = false;
	switch (useOpHash)
	{
	case ItemProp::Cast:
	{
		ret = true;
		break;
	}
	case ItemProp::Learn:
	{
		if (spell != nullptr)
		{
			player.addSpell(spell->spell->Id(), spell->spell);
		}
		ret = true;
		break;
	}
	default:
		break;
	}
	return ret;
}

bool Item::use(Player& player, const Level& level, uint32_t& quantityLeft)
{
	quantityLeft = 0;

	if (player.canUseObject(*this) == false)
	{
		return false;
	}

	LevelObjValue itemQuantity;
	bool hasQuantity = getIntByHash(ItemProp::Quantity, itemQuantity);
	if (hasQuantity == true &&
		itemQuantity <= 0)
	{
		return true;
	}

	bool ret = false;
	LevelObjValue useOp;
	if (getIntByHash(ItemProp::UseOp, useOp) == true &&
		(useOp == ItemProp::Cast || useOp == ItemProp::Learn))
	{
		ret = useSpell(player, level, (uint16_t)useOp, quantityLeft);
	}
	else
	{
		ret = useItem(player, level, (uint16_t)useOp, quantityLeft);
	}

	if (ret == true)
	{
		if (hasQuantity == true)
		{
			itemQuantity--;
			setIntByHash(ItemProp::Quantity, itemQuantity);
			quantityLeft = (uint32_t)itemQuantity;
		}
		player.updateProperties();
	}
	return ret;
}

LevelObjValue Item::addQuantity(LevelObjValue& amount)
{
	LevelObjValue capacity;
	if (amount == 0 ||
		getIntByHash(ItemProp::Capacity, capacity) == false)
	{
		return 0;
	}
	auto quantity = getIntByHash(ItemProp::Quantity);
	if (amount < 0)
	{
		auto absAmount = std::abs(amount);
		if (absAmount <= quantity)
		{
			auto newQuant = quantity - absAmount;
			setIntByHash(ItemProp::Quantity, newQuant);
			amount = 0;
			return newQuant;
		}
		else
		{
			setIntByHash(ItemProp::Quantity, 0);
			amount += quantity;
			return 0;
		}
	}
	else
	{
		if (amount + quantity <= capacity)
		{
			auto newQuant = amount + quantity;
			setIntByHash(ItemProp::Quantity, newQuant);
			amount = 0;
			return newQuant;
		}
		else
		{
			setIntByHash(ItemProp::Quantity, capacity);
			amount -= (capacity - quantity);
			return capacity;
		}
	}
}

void Item::updateOwner(Queryable* obj)
{
	itemOwner = obj;
	auto classSpell = getBaseSpell();
	if (obj != nullptr && classSpell != nullptr)
	{
		auto player = dynamic_cast<Player*>(obj);
		if (player != nullptr)
		{
			auto playerSpell = player->getSpellInstance(classSpell->spell->Id());
			if (playerSpell != nullptr)
			{
				spell = playerSpell;
				return;
			}
		}
	}
	spell = classSpell;
}
