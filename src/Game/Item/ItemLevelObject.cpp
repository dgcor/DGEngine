#include "ItemLevelObject.h"
#include "Game/Game.h"
#include "Game/GameHashes.h"
#include "Game/Level/Level.h"
#include "Game/Utils/UIObjectUtils.h"

bool ItemLevelObject::getTexture(const Item& item, uint32_t textureNumber, TextureInfo& ti)
{
	switch (textureNumber)
	{
	case 0:
		return item.getCurrentTexture(ti);
	case 1:
		return item.Class()->getInventoryTexture(item, ti);
	default:
		return false;
	}
}

bool ItemLevelObject::getIntByHash(const Item& item, uint16_t propHash, LevelObjValue& value)
{
	switch (propHash)
	{
	case ItemProp::Identified:
		value = (LevelObjValue)item.identified;
		break;
	case str2int16("indestructible"):
		value = (LevelObjValue)(getIntByHash(item, ItemProp::DurabilityMax)
			== std::numeric_limits<LevelObjValue>::max());
		break;
	case str2int16("unlimitedCharges"):
		value = (LevelObjValue)(getIntByHash(item, ItemProp::ChargesMax)
			== std::numeric_limits<LevelObjValue>::max());
		break;
	default:
		return item.properties.getValue(propHash, value);
	}
	return true;
}

LevelObjValue ItemLevelObject::getIntByHash(const Item& item, uint16_t propHash)
{
	LevelObjValue value = 0;
	getIntByHash(item, propHash, value);
	return value;
}

bool ItemLevelObject::setIntByHash(Item& item, uint16_t propHash, LevelObjValue value)
{
	switch (propHash)
	{
	case ItemProp::Identified:
		item.identified = value != 0;
		break;
	default:
	{
		if (item.properties.setValue(propHash, value) == false)
		{
			return false;
		}
	}
	}
	item.updateClassifierVals = true;
	return true;
}

bool ItemLevelObject::getNumber(const Item& item, const std::string_view prop, Number32& value)
{
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	LevelObjValue val;
	if (getNumberByHash(item, item, propHash, props.second, val) == true)
	{
		value.setInt32(val);
		return true;
	}
	return false;
}

bool ItemLevelObject::getNumberByHash(const Item& item, const Queryable& owner, uint16_t propHash, LevelObjValue& value)
{
	return getNumberByHash(item, owner, propHash, {}, value);
}

bool ItemLevelObject::getNumberByHash(const Item& item, const Queryable& owner, uint16_t propHash,
	const std::string_view minMaxNumber, LevelObjValue& value)
{
	updateClassifierValues(item);
	switch (propHash)
	{
	case str2int16("identified"):
		value = item.identified == true ? 1 : 0;
		break;
	case str2int16("pricePrefix1"):
		value = item.pricePrefix1;
		break;
	case str2int16("pricePrefix2"):
		value = item.pricePrefix2;
		break;
	case str2int16("priceSuffix1"):
		value = item.priceSuffix1;
		break;
	case str2int16("priceSuffix2"):
		value = item.priceSuffix2;
		break;
	case str2int16("baseSpell"):
	{
		auto spelli = item.getBaseSpell();
		if (spelli != nullptr)
		{
			auto props2 = Utils::splitStringIn2(minMaxNumber, '.');
			auto propHash2 = str2int16(props2.first);
			return spelli->getNumberByHash(propHash2, props2.second, value);
		}
		return false;
	}
	case str2int16("spell"):
	{
		if (item.spell != nullptr)
		{
			auto props2 = Utils::splitStringIn2(minMaxNumber, '.');
			auto propHash2 = str2int16(props2.first);
			return item.spell->getNumberByHash(owner, propHash2, props2.second, value);
		}
		return false;
	}
	default:
	{
		if (item.getIntByHash(propHash, value) == true)
		{
			break;
		}
		else if (item.Class()->evalFormula(propHash, item, owner, value, minMaxNumber) == true)
		{
			break;
		}
		return false;
	}
	}
	return true;
}

void ItemLevelObject::updatePrice(const Item& item)
{
	auto class_ = item.Class();
	item.pricePrefix1 = class_->getPricePrefix1(item);
	item.pricePrefix2 = class_->getPricePrefix2(item);
	item.priceSuffix1 = class_->getPriceSuffix1(item);
	item.priceSuffix2 = class_->getPriceSuffix2(item);
}

void ItemLevelObject::updateClassifierValues(const Item& item)
{
	if (item.updateClassifierVals == true)
	{
		item.updateClassifierVals = false;
		updatePrice(item);
		if (item.identified == false)
		{
			item.name = item.SimpleName();
		}
		else
		{
			if (item.Class()->getFullName(item, item.name) == false)
			{
				item.name = item.SimpleName();
			}
		}
		for (size_t i = 0; i < item.descriptions.size(); i++)
		{
			item.Class()->getDescription(i, item, item.descriptions[i]);
		}
	}
}

bool ItemLevelObject::getProperty(const Item& item, const std::string_view prop, Variable& var)
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	if (item.getLevelObjProp(propHash, props.second, var) == true)
	{
		return true;
	}
	updateClassifierValues(item);
	switch (propHash)
	{
	case str2int16("shortName"):
		var = Variable(item.ShortName());
		break;
	case str2int16("name"):
		var = Variable(item.Name());
		break;
	case str2int16("simpleName"):
		var = Variable(item.SimpleName());
		break;
	case str2int16("d"):
	case str2int16("description"):
	{
		size_t idx = Utils::strtou(props.second);
		if (idx >= item.descriptions.size())
		{
			idx = 0;
		}
		var = Variable(item.descriptions[idx]);
		break;
	}
	case str2int16("eval"):
		var = Variable((int64_t)Formula::evalString(props.second, &item, item.itemOwner));
		break;
	case str2int16("evalMin"):
		var = Variable((int64_t)Formula::evalMinString(props.second, &item, item.itemOwner));
		break;
	case str2int16("evalMax"):
		var = Variable((int64_t)Formula::evalMaxString(props.second, &item, item.itemOwner));
		break;
	case str2int16("evalf"):
		var = Variable(Formula::evalString(props.second, &item, item.itemOwner));
		break;
	case str2int16("evalMinf"):
		var = Variable(Formula::evalMinString(props.second, &item, item.itemOwner));
		break;
	case str2int16("evalMaxf"):
		var = Variable(Formula::evalMaxString(props.second, &item, item.itemOwner));
		break;
	case str2int16("hasDescription"):
	{
		bool hasDescr = false;
		size_t idx = Utils::strtou(props.second);
		if (idx < item.descriptions.size())
		{
			hasDescr = item.descriptions[idx].empty() == false;
		}
		var = Variable(hasDescr);
		break;
	}
	case str2int16("hasSpell"):
		var = Variable(item.hasSpell());
		break;
	case str2int16("identified"):
		var = Variable(item.identified);
		break;
	case str2int16("inventorySize"):
		var = UIObjectUtils::getTuple2iProp(item.Class()->InventorySize(), props.second);
		break;
	case str2int16("itemType"):
		var = Variable(item.ItemType());
		break;
	case str2int16("itemSubType"):
		var = Variable(item.ItemSubType());
		break;
	case str2int16("isUsable"):
		var = Variable(item.isUsable());
		break;
	case str2int16("needsRecharge"):
		var = Variable(item.needsRecharge());
		break;
	case str2int16("needsRepair"):
		var = Variable(item.needsRepair());
		break;
	case str2int16("pricePrefix1"):
		var = Variable((int64_t)item.pricePrefix1);
		break;
	case str2int16("pricePrefix2"):
		var = Variable((int64_t)item.pricePrefix2);
		break;
	case str2int16("priceSuffix1"):
		var = Variable((int64_t)item.priceSuffix1);
		break;
	case str2int16("priceSuffix2"):
		var = Variable((int64_t)item.priceSuffix2);
		break;
	case str2int16("propertyCount"):
		var = Variable((int64_t)item.properties.size());
		break;
	case str2int16("hasProperty"):
		var = Variable(item.properties.hasValue(str2int16(props.second)));
		break;
	case str2int16("baseSpell"):
	{
		auto spelli = item.getBaseSpell();
		if (spelli != nullptr)
		{
			return spelli->getProperty(props.second, var);
		}
		return false;
	}
	case str2int16("spell"):
	{
		if (item.spell != nullptr)
		{
			return item.spell->getProperty(props.second, var);
		}
		return false;
	}
	default:
	{
		LevelObjValue value;
		if (item.getIntByHash(propHash, value) == true)
		{
			var = Variable((int64_t)value);
			break;
		}
		else if (item.Class()->evalFormula(propHash, item, item, value, props.second) == true)
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

void ItemLevelObject::update(Item& item, Game& game, Level& level, const std::shared_ptr<LevelObject>& thisPtr)
{
	item.processQueuedActions(game);
	item.updateHover(game, level, thisPtr);

	if (item.hasValidState() == true &&
		item.animation.update(game.getElapsedTime()) == true)
	{
		if (item.wasHoverEnabledOnItemDrop == true)
		{
			item.enableHover = true;
			item.wasHoverEnabledOnItemDrop = false;
		}
		if (item.updateTexture() == true)
		{
			item.updateDrawPosition(level.Map());
		}
	}
}
