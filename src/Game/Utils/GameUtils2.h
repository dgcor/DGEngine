#pragma once

#include "Game/Properties/InventoryPosition.h"
#include "Game/Properties/PlayerAnimation.h"
#include "Game/Properties/PlayerDirection.h"
#include "Game/Properties/PlayerInventory.h"
#include "Game/Properties/PlayerItemMount.h"
#include "Game/Properties/PlayerStatus.h"
#include <string_view>

namespace GameUtils
{
	InventoryPosition getInventoryPosition(const std::string_view str, InventoryPosition val = InventoryPosition::TopLeft);

	PlayerAnimation getPlayerAnimation(const std::string_view str, PlayerAnimation val);

	PlayerDirection getPlayerDirection(const std::string_view str, PlayerDirection val);

	PlayerInventory getPlayerInventory(const std::string_view str, PlayerInventory val = PlayerInventory::Body);

	size_t getPlayerInventoryIndex(const std::string_view str, PlayerInventory val = PlayerInventory::Body);

	PlayerItemMount getPlayerItemMount(const std::string_view str, PlayerItemMount val = PlayerItemMount::LeftHand);

	size_t getPlayerItemMountIndex(const std::string_view str, PlayerItemMount val = PlayerItemMount::LeftHand);

	PlayerStatus getPlayerStatus(const std::string_view str, PlayerStatus val);
}
