#pragma once

#include "Game/Properties/AnimationSpeed.h"
#include "Game/Properties/InventoryPosition.h"
#include "Game/Properties/PlayerInventory.h"
#include "Game/Item/ItemLocation.h"
#include "Game/LightSource.h"
#include "Json/JsonParser.h"
#include "Utils/PairXY.h"

namespace Parser
{
	size_t getInventoryItemIndexVal(const rapidjson::Value& elem, PlayerInventory inv);

	InventoryPosition getInventoryPositionVal(const rapidjson::Value& elem, InventoryPosition val = InventoryPosition::TopLeft);

	ItemCoordInventory getItemCoordInventoryVal(const rapidjson::Value& elem);

	ItemLocation getItemLocationVal(const rapidjson::Value& elem);

	PairUInt8 getItemXYVal(const rapidjson::Value& elem, const PairUInt8& val = {});

	LightSource getLightSourceVal(const rapidjson::Value& elem, LightSource val = {});

	AnimationSpeed getPlayerAnimationSpeedVal(const rapidjson::Value& elem);

	PlayerInventory getPlayerInventoryVal(const rapidjson::Value& elem, PlayerInventory val = PlayerInventory::Body);
}
