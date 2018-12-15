#include "SavePlayer.h"
#include "Game.h"
#include "Game/Level.h"
#include "Json/JsonParser.h"
#include "SaveItem.h"
#include "SaveUtils.h"

using namespace rapidjson;
using namespace SaveUtils;

void Save::serialize(void* serializeObj, Properties& props,
	const Game& game, const Level& level, const Player& player)
{
	auto& writer = *((PrettyWriter<StringBuffer>*)serializeObj);
	const auto& playerClass = *player.Class();

	writer.StartObject();

	writeString(writer, "id", player.Id());
	writeString(writer, "class", playerClass.Id());

	// only save name if it's not equal to classfier's generated name
	std::string nameFromClass;
	if (playerClass.getFullName(player, nameFromClass) == false &&
		nameFromClass.empty() == true)
	{
		nameFromClass = player.SimpleName();
	}
	if (props.saveDefaults == true ||
		nameFromClass != player.Name())
	{
		writeString(writer, "name", player.Name());
	}

	if (level.getCurrentPlayer() == &player)
	{
		writeBool(writer, "currentPlayer", true);
	}

	writeKeyStringView(writer, "status");
	switch (player.playerStatus)
	{
	default:
	case PlayerStatus::Stand:
		writeStringView(writer, "Stand");
		break;
	case PlayerStatus::Walk:
		writeStringView(writer, "Walk");
		break;
	case PlayerStatus::Attack:
		writeStringView(writer, "Attack");
		break;
	case PlayerStatus::Dead:
		writeStringView(writer, "Dead");
		break;
	}

	writeKeyStringView(writer, "direction");
	switch (player.playerDirection)
	{
	case PlayerDirection::Front:
		writeStringView(writer, "Front");
		break;
	case PlayerDirection::FrontLeft:
		writeStringView(writer, "FrontLeft");
		break;
	case PlayerDirection::Left:
		writeStringView(writer, "Left");
		break;
	case PlayerDirection::BackLeft:
		writeStringView(writer, "BackLeft");
		break;
	case PlayerDirection::Back:
		writeStringView(writer, "Back");
		break;
	case PlayerDirection::BackRight:
		writeStringView(writer, "BackRight");
		break;
	case PlayerDirection::Right:
		writeStringView(writer, "Right");
		break;
	case PlayerDirection::FrontRight:
		writeStringView(writer, "FrontRight");
		break;
	case PlayerDirection::All:
	default:
		writeStringView(writer, "All");
		break;
	}

	writeKeyStringView(writer, "animation");
	switch (player.playerAnimation)
	{
	default:
	case PlayerAnimation::Stand1:
		writeStringView(writer, "Stand1");
		break;
	case PlayerAnimation::Stand2:
		writeStringView(writer, "Stand2");
		break;
	case PlayerAnimation::Walk1:
		writeStringView(writer, "Walk1");
		break;
	case PlayerAnimation::Walk2:
		writeStringView(writer, "Walk2");
		break;
	case PlayerAnimation::Attack1:
		writeStringView(writer, "Attack1");
		break;
	case PlayerAnimation::Attack2:
		writeStringView(writer, "Attack2");
		break;
	case PlayerAnimation::Attack3:
		writeStringView(writer, "Attack3");
		break;
	case PlayerAnimation::Attack4:
		writeStringView(writer, "Attack4");
		break;
	case PlayerAnimation::Defend1:
		writeStringView(writer, "Defend1");
		break;
	case PlayerAnimation::Defend2:
		writeStringView(writer, "Defend2");
		break;
	case PlayerAnimation::Defend3:
		writeStringView(writer, "Defend3");
		break;
	case PlayerAnimation::Defend4:
		writeStringView(writer, "Defend4");
		break;
	case PlayerAnimation::Hit1:
		writeStringView(writer, "Hit1");
		break;
	case PlayerAnimation::Hit2:
		writeStringView(writer, "Hit2");
		break;
	case PlayerAnimation::Die1:
		writeStringView(writer, "Die1");
		break;
	case PlayerAnimation::Die2:
		writeStringView(writer, "Die2");
		break;
	}

	writeVector2d<MapCoord>(writer, "mapPosition", player.MapPosition());

	if (props.saveDefaults == true ||
		player.enableHover == false)
	{
		writeBool(writer, "enableHover", player.enableHover);
	}

	if (props.saveDefaults == true ||
		player.useAI == true)
	{
		writeBool(writer, "AI", player.useAI);
	}

	writeKeyStringView(writer, "properties");
	writer.StartObject();
	{
		auto savePlayerProp = [&](const std::string_view prop)
		{
			Number32 numVal;
			uint16_t propHash = str2int16(prop);
			if (player.getNumberByHash(propHash, numVal) == true)
			{
				if (props.saveDefaults == false &&
					(playerClass.isDefault(propHash, numVal) == true ||
						numVal.getInt32() == 0))
				{
					return;
				}
				writeNumber32(writer, prop, numVal);
			}
		};

		savePlayerProp("experience");
		savePlayerProp("points");

		savePlayerProp("strength");
		savePlayerProp("strengthItems");
		savePlayerProp("magic");
		savePlayerProp("magicItems");
		savePlayerProp("dexterity");
		savePlayerProp("dexterityItems");
		savePlayerProp("vitality");
		savePlayerProp("vitalityItems");
		savePlayerProp("life");
		savePlayerProp("lifeItems");
		savePlayerProp("lifeDamage");
		savePlayerProp("mana");
		savePlayerProp("manaItems");
		savePlayerProp("manaDamage");
		savePlayerProp("armor");
		savePlayerProp("armorItems");
		savePlayerProp("toArmor");
		savePlayerProp("toHit");
		savePlayerProp("toHitItems");
		savePlayerProp("damageMin");
		savePlayerProp("damageMax");
		savePlayerProp("damageMinItems");
		savePlayerProp("damageMaxItems");
		savePlayerProp("toDamage");
		savePlayerProp("resistMagic");
		savePlayerProp("resistFire");
		savePlayerProp("resistLightning");

		for (const auto& prop : player.customProperties)
		{
			if (props.saveDefaults == false && playerClass.isDefault(prop) == true)
			{
				continue;
			}
			writeNumber32(writer, level.getPropertyName(prop.first), prop.second);
		}
	}
	writer.EndObject();

	if (player.inventories.empty() == false)
	{
		writeKeyStringView(writer, "inventory");
		writer.StartArray();
		for (size_t i = 0; i < player.inventories.size(); i++)
		{
			const auto& inv = player.inventories[i];
			if (inv.empty() == true)
			{
				continue;
			}

			writer.StartObject();

			writeUInt(writer, "index", i);

			if (inv.getXYSize().y != 1)
			{
				writeVector2d<ItemXY>(writer, "size", inv.getXYSize());
			}
			else
			{
				writeUInt(writer, "size", inv.Size());
			}

			if (inv.AllowedTypes().empty() == false)
			{
				writer.SetFormatOptions(PrettyFormatOptions::kFormatSingleLineArray);
				writeKeyStringView(writer, "allowedClassTypes");
				writer.StartArray();
				for (const auto& typeHash16 : inv.AllowedTypes())
				{
					auto prop = level.getPropertyName(typeHash16);
					if (prop.empty() == false)
					{
						writeStringView(writer, prop);
					}
				}
				writer.EndArray();
				writer.SetFormatOptions(PrettyFormatOptions::kFormatDefault);
			}

			if (i == player.bodyInventoryIdx)
			{
				writeBool(writer, "bodyInventory", true);
			}

			if (inv.getEnforceItemSize() == true)
			{
				writeBool(writer, "enforceItemSize", true);
			}

			writeKeyStringView(writer, "item");
			writer.StartArray();
			auto oldCustomProp = props.customProperty;
			for (auto it = inv.begin(); it != inv.end(); ++it)
			{
				props.customProperty = &it.state.idx;
				serialize(serializeObj, props, game, level, *it);
			}
			props.customProperty = oldCustomProp;
			writer.EndArray();

			writer.EndObject();
		}
		writer.EndArray();
	}

	if (player.spells.empty() == false)
	{
		writeKeyStringView(writer, "spell");
		writer.StartArray();
		for (const auto& spell : player.spells)
		{
			writer.StartObject();
			writeString(writer, "class", spell.first);
			writer.EndObject();
		}
		writer.EndArray();
	}

	writer.EndObject();
}
