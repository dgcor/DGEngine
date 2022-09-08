#include "PlayerLevelObject.h"
#include "Game/Utils/GameUtils2.h"
#include "Game/Player/Player.h"

bool PlayerLevelObject::getNumber(const PlayerBase& player, const std::string_view prop, Number32& value)
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	return getNumberByHash(player, propHash, props.second, value);
}

bool PlayerLevelObject::getNumberByHash(const PlayerBase& player, uint16_t propHash, const std::string_view props, Number32& value)
{
	switch (propHash)
	{
	case str2int16("selectedSpell"):
	{
		if (player.SelectedSpell() != nullptr)
		{
			return player.SelectedSpell()->getNumber(props, value);
		}
		return false;
	}
	case str2int16("spell"):
	{
		auto props2 = Utils::splitStringIn2(props, '.');
		auto spell = player.getSpellInstance(props2.first);
		if (spell != nullptr)
		{
			return spell->getNumber(props2.second, value);
		}
		return false;
	}
	default:
		return getNumberByHash(player, propHash, value);
	}
	return true;
}

bool PlayerLevelObject::getNumberByHash(const PlayerBase& player, uint16_t propHash, Number32& value)
{
	LevelObjValue iVal;
	if (player.properties.getIntByHash(*player.Class(), propHash, iVal) == true)
	{
		value.setInt32(iVal);
		return true;
	}
	uint32_t uVal;
	if (player.properties.getUIntByHash(propHash, uVal) == true)
	{
		value.setUInt32(uVal);
		return true;
	}
	return player.properties.custom.getValue(propHash, value);
}

bool PlayerLevelObject::getNumberByHash(const PlayerBase& player, uint16_t propHash, LevelObjValue& value)
{
	Number32 number;
	if (getNumberByHash(player, propHash, number) == true)
	{
		value = number.getInt32();
		return true;
	}
	return false;
}

bool PlayerLevelObject::getProperty(const PlayerBase& player, const std::string_view prop, Variable& var)
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	if (player.getLevelObjProp(propHash, props.second, var) == true)
	{
		return true;
	}
	switch (propHash)
	{
	case str2int16("name"):
		var = Variable(player.Name());
		break;
	case str2int16("simpleName"):
		var = Variable(player.SimpleName());
		break;
	case str2int16("d"):
	case str2int16("description"):
	{
		player.properties.updateNameAndDescriptions(player);
		size_t idx = Utils::strtou(props.second);
		if (idx >= player.properties.descriptions.size())
		{
			idx = 0;
		}
		var = Variable(player.properties.descriptions[idx]);
		break;
	}
	case str2int16("eval"):
		var = Variable((int64_t)Formula::evalString(props.second, player));
		break;
	case str2int16("evalMin"):
		var = Variable((int64_t)Formula::evalMinString(props.second, player));
		break;
	case str2int16("evalMax"):
		var = Variable((int64_t)Formula::evalMaxString(props.second, player));
		break;
	case str2int16("evalf"):
		var = Variable(Formula::evalString(props.second, player));
		break;
	case str2int16("evalMinf"):
		var = Variable(Formula::evalMinString(props.second, player));
		break;
	case str2int16("evalMaxf"):
		var = Variable(Formula::evalMaxString(props.second, player));
		break;
	case str2int16("totalKills"):
		var = Variable((int64_t)player.Class()->TotalKills());
		break;
	case str2int16("hasMaxStats"):
		var = Variable(player.hasMaxStats());
		break;
	case str2int16("hasProperty"):
		var = Variable(player.properties.custom.hasValue(str2int16(props.second)));
		break;
	case str2int16("canUseSelectedItem"):
	{
		if (player.SelectedItem() == nullptr)
		{
			return false;
		}
		var = Variable(player.canUseObject(*player.SelectedItem()));
		break;
	}
	case str2int16("canUseSelectedSpell"):
	{
		if (player.SelectedSpell() == nullptr)
		{
			return false;
		}
		var = Variable(player.canUseObject(*player.SelectedSpell()->spell));
		break;
	}
	case str2int16("canUseItem"):
	{
		std::string_view props2;
		size_t invIdx;
		size_t itemIdx;
		if (player.inventories.parseInventoryAndItem(
			props.second, props2, invIdx, itemIdx) == true)
		{
			auto item = player.inventories[invIdx].get(itemIdx);
			if (item != nullptr)
			{
				var = Variable(player.canUseObject(*item));
				break;
			}
		}
		return false;
	}
	case str2int16("hasEquipedItemType"):
		var = Variable(player.hasEquipedItemType(props.second));
		break;
	case str2int16("hasEquipedItemSubType"):
		var = Variable(player.hasEquipedItemSubType(props.second));
		break;
	case str2int16("hasSelectedItem"):
		var = Variable(player.SelectedItem() != nullptr);
		break;
	case str2int16("hasItem"):
	{
		std::string_view props2;
		size_t invIdx;
		size_t itemIdx;
		if (player.inventories.parseInventoryAndItem(
			props.second, props2, invIdx, itemIdx) == true)
		{
			var = Variable(player.inventories[invIdx].get(itemIdx) != nullptr);
			break;
		}
		return false;
	}
	case str2int16("hasItemClass"):
		var = Variable(player.inventories.hasItem(str2int16(props.second)));
		break;
	case str2int16("hasSpell"):
	{
		var = Variable(player.getSpell(props.second) != nullptr);
		break;
	}
	case str2int16("hasSelectedSpell"):
	{
		var = Variable(player.SelectedSpell() != nullptr);
		break;
	}
	case str2int16("isItemSlotInUse"):
	{
		std::string_view props2;
		size_t invIdx;
		size_t itemIdx;
		if (player.inventories.parseInventoryAndItem(
			props.second, props2, invIdx, itemIdx) == true)
		{
			var = Variable(player.inventories[invIdx].isSlotInUse(itemIdx));
			break;
		}
		return false;
	}
	case str2int16("isStanding"):
		var = Variable(player.playerStatus == PlayerStatus::Stand);
		break;
	case str2int16("isWalking"):
		var = Variable(player.playerStatus == PlayerStatus::Walk);
		break;
	case str2int16("isAttacking"):
		var = Variable(player.playerStatus == PlayerStatus::Attack);
		break;
	case str2int16("isDead"):
		var = Variable(player.playerStatus == PlayerStatus::Dead);
		break;
	case str2int16("selectedItem"):
	{
		if (player.SelectedItem() != nullptr)
		{
			return player.SelectedItem()->getProperty(props.second, var);
		}
		return false;
	}
	case str2int16("item"):
	{
		std::string_view props2;
		size_t invIdx;
		size_t itemIdx;
		if (player.inventories.parseInventoryAndItem(
			props.second, props2, invIdx, itemIdx) == true)
		{
			auto item = player.inventories[invIdx].get(itemIdx);
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
		if (invIdx < player.inventories.size())
		{
			return player.inventories[invIdx].getProperty(props2.second, var);
		}
		return false;
	}
	case str2int16("itemQuantity"):
	{
		auto classIdHash16 = str2int16(props.second);
		uint32_t itemQuantity = 0;
		player.inventories.getQuantity(classIdHash16, itemQuantity);
		var = Variable((int64_t)itemQuantity);
		break;
	}
	case str2int16("selectedSpell"):
	{
		if (player.SelectedSpell() != nullptr)
		{
			return player.SelectedSpell()->getProperty(props.second, var);
		}
		return false;
	}
	case str2int16("spell"):
	{
		auto props2 = Utils::splitStringIn2(props.second, '.');
		auto spell = player.getSpellInstance(props2.first);
		if (spell != nullptr)
		{
			return spell->getProperty(props2.second, var);
		}
		return false;
	}
	default:
	{
		Number32 value;
		if (getNumberByHash(player, propHash, props.second, value) == true)
		{
			var = Variable(value.getInt64());
			break;
		}
		return false;
	}
	}
	return true;
}

bool PlayerLevelObject::setProperty(PlayerBase& player, const std::string_view prop, const Variable& var)
{
	if (prop.empty() == true)
	{
		return false;
	}
	auto propHash16 = str2int16(prop);
	switch (propHash16)
	{
	case str2int16("name"):
	{
		if (std::holds_alternative<std::string>(var) == true)
		{
			player.Name(std::get<std::string>(var));
			return true;
		}
	}
	break;
	default:
	{
		if (std::holds_alternative<int64_t>(var) == true)
		{
			if (player.setNumberByHash(propHash16, Number32((int32_t)std::get<int64_t>(var)), nullptr))
			{
				player.properties.updateProperties(player);
				return true;
			}
		}
	}
	break;
	}
	return false;
}

QueryObject PlayerLevelObject::getQueryable(const PlayerBase& player, const std::string_view prop)
{
	if (prop.empty() == true)
	{
		return &player;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	auto propHash = str2int16(props.first);
	QueryObject queryObj;
	switch (propHash)
	{
	case str2int16("selectedItem"):
		queryObj = player.SelectedItem();
		break;
	case str2int16("item"):
	{
		size_t invIdx;
		size_t itemIdx;
		if (player.inventories.parseInventoryAndItem(
			props.second, props.second, invIdx, itemIdx) == true)
		{
			queryObj = player.inventories[invIdx].get(itemIdx);
			break;
		}
	}
	break;
	case str2int16("selectedSpell"):
		queryObj = player.SelectedSpell();
		break;
	case str2int16("spell"):
		return player.getSpellInstance(props.second);
	default:
		break;
	}
	auto queryable = getQueryablePtr(queryObj);
	if (queryable != nullptr &&
		props.second.empty() == false)
	{
		return queryable->getQueryable(props.second);
	}
	return queryObj;
}

std::vector<VarOrQueryObject> PlayerLevelObject::getQueryableList(const PlayerBase& player, const std::string_view prop)
{
	std::vector<VarOrQueryObject> queryableList;

	auto props = Utils::splitStringIn2(prop, '.');
	if (props.first.empty() == false)
	{
		auto invIdx = GameUtils::getPlayerInventoryIndex(props.second);
		if (invIdx < player.inventories.size())
		{
			const auto& inventory = player.inventories[invIdx];
			for (size_t i = 0; i < inventory.Size(); i++)
			{
				if (inventory.isSlotInUse(i) == true)
				{
					queryableList.push_back(std::make_pair(inventory.get(i), i));
				}
			}
		}
	}
	return queryableList;
}
