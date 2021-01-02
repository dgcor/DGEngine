#include "GameUtilsGame.h"
#include <charconv>
#include "Utils/Utils.h"

namespace GameUtils
{
	InventoryPosition getInventoryPosition(const std::string_view str, InventoryPosition val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("topleft"):
			return InventoryPosition::TopLeft;
		case str2int16("topright"):
			return InventoryPosition::TopRight;
		case str2int16("bottomleft"):
			return InventoryPosition::BottomLeft;
		case str2int16("bottomright"):
			return InventoryPosition::BottomRight;
		}
		return val;
	}

	PlayerAnimation getPlayerAnimation(const std::string_view str, PlayerAnimation val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("stand1"):
			return PlayerAnimation::Stand1;
		case str2int16("stand2"):
			return PlayerAnimation::Stand2;
		case str2int16("walk1"):
			return PlayerAnimation::Walk1;
		case str2int16("walk2"):
			return PlayerAnimation::Walk2;
		case str2int16("attack1"):
			return PlayerAnimation::Attack1;
		case str2int16("attack2"):
			return PlayerAnimation::Attack2;
		case str2int16("attack3"):
			return PlayerAnimation::Attack3;
		case str2int16("attack4"):
			return PlayerAnimation::Attack4;
		case str2int16("defend1"):
			return PlayerAnimation::Defend1;
		case str2int16("defend2"):
			return PlayerAnimation::Defend2;
		case str2int16("defend3"):
			return PlayerAnimation::Defend3;
		case str2int16("defend4"):
			return PlayerAnimation::Defend4;
		case str2int16("hit1"):
			return PlayerAnimation::Hit1;
		case str2int16("hit2"):
			return PlayerAnimation::Hit2;
		case str2int16("die1"):
			return PlayerAnimation::Die1;
		case str2int16("die2"):
			return PlayerAnimation::Die2;
		default:
			return val;
		}
	}

	PlayerDirection getPlayerDirection(const std::string_view str, PlayerDirection val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("all"):
			return PlayerDirection::All;
		case str2int16("front"):
			return PlayerDirection::Front;
		case str2int16("frontleft"):
			return PlayerDirection::FrontLeft;
		case str2int16("left"):
			return PlayerDirection::Left;
		case str2int16("backleft"):
			return PlayerDirection::BackLeft;
		case str2int16("back"):
			return PlayerDirection::Back;
		case str2int16("backright"):
			return PlayerDirection::BackRight;
		case str2int16("right"):
			return PlayerDirection::Right;
		case str2int16("frontright"):
			return PlayerDirection::FrontRight;
		default:
			return val;
		}
	}

	PlayerInventory getPlayerInventory(const std::string_view str, PlayerInventory val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("body"):
			return PlayerInventory::Body;
		case str2int16("belt"):
			return PlayerInventory::Belt;
		case str2int16("stash"):
			return PlayerInventory::Stash;
		default:
			return val;
		}
	}

	size_t getPlayerInventoryIndex(const std::string_view str, PlayerInventory val)
	{
		size_t num = (size_t)val;
		auto err = std::from_chars(str.data(), str.data() + str.size(), num);
		if (err.ec == std::errc() ||
			err.ec == std::errc::result_out_of_range)
		{
			return num;
		}
		return (size_t)getPlayerInventory(str, val);
	}

	PlayerItemMount getPlayerItemMount(const std::string_view str, PlayerItemMount val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("lefthand"):
			return PlayerItemMount::LeftHand;
		case str2int16("righthand"):
			return PlayerItemMount::RightHand;
		case str2int16("armor"):
			return PlayerItemMount::Armor;
		case str2int16("helmet"):
			return PlayerItemMount::Helmet;
		case str2int16("amulet"):
			return PlayerItemMount::Amulet;
		case str2int16("leftring"):
			return PlayerItemMount::LeftRing;
		case str2int16("rightring"):
			return PlayerItemMount::RightRing;
		default:
			return val;
		}
	}

	size_t getPlayerItemMountIndex(const std::string_view str, PlayerItemMount val)
	{
		size_t num = (size_t)val;
		auto err = std::from_chars(str.data(), str.data() + str.size(), num);
		if (err.ec == std::errc() ||
			err.ec == std::errc::result_out_of_range)
		{
			return num;
		}
		return (size_t)getPlayerItemMount(str, val);
	}

	PlayerStatus getPlayerStatus(const std::string_view str, PlayerStatus val)
	{
		switch (str2int16(Utils::toLower(str)))
		{
		case str2int16("stand"):
			return PlayerStatus::Stand;
		case str2int16("walk"):
			return PlayerStatus::Walk;
		case str2int16("attack"):
			return PlayerStatus::Attack;
		case str2int16("dead"):
			return PlayerStatus::Dead;
		default:
			return val;
		}
	}
}
