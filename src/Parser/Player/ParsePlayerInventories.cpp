#include "ParsePlayerInventories.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/Player/Player.h"
#include "Parser/Item/ParseItem.h"
#include "Parser/Utils/ParseUtils.h"
#include "Parser/Utils/ParseUtilsGameVal.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parsePlayerItem(Player& player, Level& level, const Inventory& inventory, size_t invIdx, const Value& elem)
	{
		if (elem.HasMember("index"sv) == false)
		{
			return;
		}
		size_t itemIdx = 0;
		const auto& idxElem = elem["index"sv];
		if (idxElem.IsString() == true)
		{
			itemIdx = GameUtils::getPlayerItemMountIndex(idxElem.GetStringView());
		}
		else if (idxElem.IsArray() == true)
		{
			itemIdx = inventory.getIndex(getItemXYVal(idxElem));
		}
		else if (idxElem.IsUint() == true)
		{
			itemIdx = idxElem.GetUint();
		}
		else
		{
			return;
		}
		if (itemIdx >= inventory.Size())
		{
			return;
		}
		auto item = parseItemObj(level, elem);
		player.setItem(invIdx, itemIdx, item);
	}

	void parsePlayerInventory(Player& player, Level& level, const Value& elem)
	{
		if (elem.HasMember("index"sv) == false)
		{
			return;
		}

		size_t invIdx = 0;
		const auto& invElem = elem["index"sv];
		if (invElem.IsString() == true)
		{
			invIdx = (size_t)GameUtils::getPlayerInventory(invElem.GetStringView());
		}
		else if (invElem.IsUint() == true)
		{
			invIdx = invElem.GetUint();
		}
		if (invIdx >= player.getInventorySize())
		{
			return;
		}
		auto& inventory = player.getInventory(invIdx);
		bool wasInitialized = false;
		if (elem.HasMember("size"sv) == true)
		{
			const auto& sizeElem = elem["size"sv];
			if (sizeElem.IsUint() == true)
			{
				inventory.init(sizeElem.GetUint());
				wasInitialized = true;
			}
			else if (sizeElem.IsArray() == true)
			{
				inventory.init(getItemXYVal(sizeElem));
				wasInitialized = true;
			}
		}
		if (wasInitialized == false
			&& invIdx == (size_t)PlayerInventory::Body)
		{
			inventory.init((size_t)PlayerItemMount::Size);
		}
		if (inventory.Size() == 0)
		{
			return;
		}

		inventory.EnforceItemSize(getBoolKey(elem, "enforceItemSize"));

		if (getBoolKey(elem, "bodyInventory") == true)
		{
			player.setBodyInventoryIdx(invIdx);
		}
		if (elem.HasMember("allowedClassTypes"sv) == true)
		{
			const auto& classesElem = elem["allowedClassTypes"sv];
			if (classesElem.IsString() == true)
			{
				auto name = Utils::toLower(getStringViewVal(classesElem));
				auto nameHash = str2int16(name);
				level.setPropertyName(nameHash, name);
				inventory.allowTypeByHash(nameHash);
			}
			else if (classesElem.IsArray() == true)
			{
				for (const auto& arrElem : classesElem)
				{
					auto name = Utils::toLower(getStringViewVal(arrElem));
					auto nameHash = str2int16(name);
					level.setPropertyName(nameHash, name);
					inventory.allowTypeByHash(nameHash);
				}
			}
		}
		if (elem.HasMember("item"sv) == true)
		{
			const auto& itemsElem = elem["item"sv];
			if (itemsElem.IsArray() == true)
			{
				for (const auto& val : itemsElem)
				{
					parsePlayerItem(player, level, inventory, invIdx, val);
				}
			}
			else if (itemsElem.IsObject() == true)
			{
				parsePlayerItem(player, level, inventory, invIdx, itemsElem);
			}
		}
	}

	void parsePlayerInventories(Player& player, Level& level, const Value& elem)
	{
		if (elem.HasMember("inventory"sv) == true)
		{
			const auto& invElem = elem["inventory"sv];
			if (invElem.IsArray() == true)
			{
				for (const auto& val : invElem)
				{
					parsePlayerInventory(player, level, val);
				}
			}
			else if (invElem.IsObject() == true)
			{
				parsePlayerInventory(player, level, invElem);
			}
		}
	}
}
