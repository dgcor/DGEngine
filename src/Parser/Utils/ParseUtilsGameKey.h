#pragma once

#include "ParseUtilsGameVal.h"

namespace Parser
{
	size_t getInventoryItemIndexKey(const rapidjson::Value& elem,
		const std::string_view key, PlayerInventory inv);

	InventoryPosition getInventoryPositionKey(const rapidjson::Value& elem,
		const std::string_view key, InventoryPosition val = InventoryPosition::TopLeft);

	ItemCoordInventory getItemCoordInventoryKey(const rapidjson::Value& elem, const std::string_view key);

	ItemLocation getItemLocationKey(const rapidjson::Value& elem, const std::string_view key);

	PairUInt8 getItemXYKey(const rapidjson::Value& elem, const std::string_view key, const PairUInt8& val = {});

	LightSource getLightSourceKey(const rapidjson::Value& elem,
		const std::string_view key, LightSource val = {});

	PlayerAnimation getPlayerAnimationKey(const rapidjson::Value& elem,
		const std::string_view key, PlayerAnimation val = PlayerAnimation::Stand1);

	PlayerDirection getPlayerDirectionKey(const rapidjson::Value& elem,
		const std::string_view key, PlayerDirection val = PlayerDirection::All);

	PlayerInventory getPlayerInventoryKey(const rapidjson::Value& elem,
		const std::string_view key, PlayerInventory val = PlayerInventory::Body);

	PlayerItemMount getPlayerItemMountKey(const rapidjson::Value& elem,
		const std::string_view key, PlayerItemMount val = PlayerItemMount::LeftHand);

	PlayerStatus getPlayerStatusKey(const rapidjson::Value& elem,
		const std::string_view key, PlayerStatus val = PlayerStatus::Stand);
}
