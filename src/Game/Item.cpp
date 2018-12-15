#include "Item.h"
#include "Game.h"
#include "GameHashes.h"
#include "GameUtils.h"
#include "Game/Level.h"
#include "Player.h"
#include "Utils/Utils.h"

Item::Item(const ItemClass* class__) : LevelObject(class__)
{
	texturePack = class__->getDropTexturePack();
	animation.textureIndexRange = class__->getDropTextureIndexRange();
	animation.textureIndexRange.second--;
	animation.currentTextureIdx = animation.textureIndexRange.second;
	animation.frameTime = class__->AnimationSpeed();
	animation.animType = AnimationType::PlayOnce;
	hoverCellSize = 1;
	sprite.setOutline(class__->Outline(), class__->OutlineIgnore());
	updateTexture();
	applyDefaults();
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

bool Item::getTexture(size_t textureNumber, TextureInfo& ti) const
{
	switch (textureNumber)
	{
	case 0:
		return getCurrentTexture(ti);
	case 1:
		return Class()->getInventoryTexture(*this, ti);
	default:
		return false;
	}
}

void Item::update(Game& game, Level& level)
{
	processQueuedActions(game);
	updateHover(game, level);

	if (hasValidState() == true &&
		animation.update(game.getElapsedTime()) == true)
	{
		if (wasHoverEnabledOnItemDrop == true)
		{
			enableHover = true;
			wasHoverEnabledOnItemDrop = false;
		}
		if (updateTexture() == true)
		{
			updateDrawPosition(level.Map());
		}
	}
}

bool Item::getProperty(const std::string_view prop, Variable& var) const
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	if (GameUtils::getLevelObjProp(*this, propHash, props.second, var) == true)
	{
		return true;
	}
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
		size_t idx = Utils::strtou(props.second);
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
		size_t idx = Utils::strtou(props.second);
		if (idx < descriptions.size())
		{
			hasDescr = descriptions[idx].empty() == false;
		}
		var = Variable(hasDescr);
		break;
	}
	case str2int16("hasSpell"):
		var = Variable(hasSpell());
		break;
	case str2int16("prices"):
	{
		LevelObjValue value;
		if (Class()->evalFormula(str2int16(props.second), *this, value) == false)
		{
			if (getIntByHash(propHash, value) == false)
			{
				return false;
			}
		}
		var = Variable((int64_t)value);
		break;
	}
	case str2int16("identified"):
		var = Variable(identified);
		break;
	case str2int16("itemType"):
		var = Variable(ItemType());
		break;
	case str2int16("itemSubType"):
		var = Variable(ItemSubType());
		break;
	case str2int16("isUsable"):
		var = Variable(isUsable());
		break;
	case str2int16("needsRecharge"):
		var = Variable(needsRecharge());
		break;
	case str2int16("needsRepair"):
		var = Variable(needsRepair());
		break;
	case str2int16("propertyCount"):
		var = Variable((int64_t)properties.size());
		break;
	case str2int16("hasProperty"):
		var = Variable(hasInt(props.second));
		break;
	case str2int16("spell"):
	{
		auto spell = getSpell();
		if (spell != nullptr)
		{
			return spell->getProperty(props.second, var);
		}
		break;
	}
	default:
	{
		LevelObjValue value;
		if (getIntByHash(propHash, value) == true)
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

void Item::setProperty(const std::string_view prop, const Variable& val)
{
	if (prop.empty() == true)
	{
		return;
	}
	LevelObjValue val2 = 0;
	if (std::holds_alternative<bool>(val) == true)
	{
		val2 = std::get<bool>(val) ? 1 : 0;
	}
	else if (std::holds_alternative<int64_t>(val) == true)
	{
		val2 = (LevelObjValue)std::get<int64_t>(val);
	}
	setIntByHash(str2int16(prop), val2);
}

bool Item::hasIntByHash(uint16_t propHash) const noexcept
{
	return properties.hasValue(propHash);
}

bool Item::hasInt(const std::string_view prop) const noexcept
{
	return hasIntByHash(str2int16(prop));
}

LevelObjValue Item::getIntByHash(uint16_t propHash) const
{
	LevelObjValue value = 0;
	getIntByHash(propHash, value);
	return value;
}

LevelObjValue Item::getInt(const std::string_view prop) const
{
	LevelObjValue value = 0;
	getInt(prop, value);
	return value;
}

bool Item::getIntByHash(uint16_t propHash, LevelObjValue& value) const
{
	switch (propHash)
	{
	case ItemProp::Identified:
		value = (LevelObjValue)identified;
		break;
	case str2int16("indestructible"):
		value = (LevelObjValue)(getIntByHash(ItemProp::DurabilityMax)
			== std::numeric_limits<LevelObjValue>::max());
		break;
	case str2int16("unlimitedCharges"):
		value = (LevelObjValue)(getIntByHash(ItemProp::ChargesMax)
			== std::numeric_limits<LevelObjValue>::max());
		break;
	default:
		return properties.getValue(propHash, value);
	}
	return true;
}

bool Item::getInt(const std::string_view prop, LevelObjValue& value) const
{
	return getIntByHash(str2int16(prop), value);
}

void Item::setIntByHash(uint16_t propHash, LevelObjValue value)
{
	switch (propHash)
	{
	case ItemProp::Identified:
		identified = value != 0;
		break;
	default:
	{
		if (properties.setValue(propHash, value) == false)
		{
			return;
		}
	}
	}
	updateNameAndDescr = true;
}

void Item::setInt(const std::string_view prop, LevelObjValue value)
{
	setIntByHash(str2int16(prop), value);
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
			if (Class()->getFullName(*this, name) == false)
			{
				name = SimpleName();
			}
		}
		for (size_t i = 0; i < descriptions.size(); i++)
		{
			Class()->getDescription(i, *this, descriptions[i]);
		}
	}
}

void Item::applyDefaults()
{
	for (const auto& prop : Class()->Defaults())
	{
		setIntByHash(prop.first, prop.second);
	}
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
	return (hasIntByHash(ItemProp::UseOn) || hasIntByHash(ItemProp::UseOp));
}

bool Item::useHelper(uint16_t propHash, uint16_t useOpHash,
	LevelObjValue value, Player& player, const Level& level) const noexcept
{
	Number32 origValue2;
	if (player.getNumberByHash(propHash, origValue2) == false)
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
	return player.setNumberByHash(propHash, (LevelObjValue)origValue, &level);
}

bool Item::useHelper(uint16_t propHash, uint16_t useOpHash, uint16_t valueHash,
	uint16_t valueMaxHash, Player& player, const Level& level) const
{
	LevelObjValue value;
	if (Class()->evalFormula(valueHash, *this, player, value) == false)
	{
		return false;
	}
	LevelObjValue valueMax;
	if (Class()->evalFormula(valueMaxHash, *this, player, valueMax) == true)
	{
		value = Utils::Random::get<LevelObjValue>(value, valueMax);
	}
	return useHelper(propHash, useOpHash, value, player, level);
}

bool Item::useItem(Player& player, const Level& level,
	uint16_t useOpHash, uint32_t& quantityLeft)
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
		ret |= useHelper(ItemProp::LifeDamage, useOpHash,
			ItemProp::Value, ItemProp::ValueMax, player, level);
		ret |= useHelper(ItemProp::ManaDamage, useOpHash,
			ItemProp::Value2, ItemProp::Value2Max, player, level);
		break;
	}
	case ItemProp::AllAttributes:
	{
		LevelObjValue value;
		if (Class()->evalFormula(ItemProp::Value, *this, player, value) == false)
		{
			break;
		}
		LevelObjValue valueMax;
		if (Class()->evalFormula(ItemProp::ValueMax, *this, player, valueMax) == true)
		{
			value = Utils::Random::get<LevelObjValue>(value, valueMax);
		}
		ret |= useHelper(ItemProp::Strength, useOpHash, value, player, level);
		ret |= useHelper(ItemProp::Magic, useOpHash, value, player, level);
		ret |= useHelper(ItemProp::Dexterity, useOpHash, value, player, level);
		ret |= useHelper(ItemProp::Vitality, useOpHash, value, player, level);
		break;
	}
	default:
	{
		ret = useHelper(propHash, useOpHash, ItemProp::Value, ItemProp::ValueMax, player, level);
		break;
	}
	}
	return ret;
}

bool Item::useSpell(Player& player, const Level& level,
	uint16_t useOpHash, uint32_t& quantityLeft)
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
		auto spell = getSpell();
		if (spell != nullptr)
		{
			player.addSpell(spell->Id(), spell);
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

	if (player.canUseItem(*this) == false)
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
			player.updateItemQuantityCache(ItemProp::Quantity);
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
