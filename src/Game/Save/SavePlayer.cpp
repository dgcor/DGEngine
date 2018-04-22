#include "SavePlayer.h"
#include "Game/Level.h"
#include "Json/JsonParser.h"
#include "SaveItem.h"

using namespace rapidjson;

void Save::serialize(void* serializeObj, const Level& level,
	const Player& player, bool skipDefaults)
{
	auto& writer = *((PrettyWriter<StringBuffer>*)serializeObj);

	writer.StartObject();

	writer.Key("id");
	writer.String(player.Id());

	writer.Key("class");
	writer.String(player.getPlayerClass()->Id());

	writer.Key("name");
	writer.String(player.Name());

	if (level.getCurrentPlayer() == &player)
	{
		writer.Key("currentPlayer");
		writer.Bool(true);
	}

	writer.Key("direction");
	switch (player.direction)
	{
	case PlayerDirection::Front:
		writer.String("Front");
		break;
	case PlayerDirection::FrontLeft:
		writer.String("FrontLeft");
		break;
	case PlayerDirection::Left:
		writer.String("Left");
		break;
	case PlayerDirection::BackLeft:
		writer.String("BackLeft");
		break;
	case PlayerDirection::Back:
		writer.String("Back");
		break;
	case PlayerDirection::BackRight:
		writer.String("BackRight");
		break;
	case PlayerDirection::Right:
		writer.String("Right");
		break;
	case PlayerDirection::FrontRight:
		writer.String("FrontRight");
		break;
	case PlayerDirection::All:
	default:
		writer.String("All");
		break;
	}

	writer.Key("mapPosition");
	writer.StartArray();
	writer.Int(player.MapPosition().x);
	writer.Int(player.MapPosition().y);
	writer.EndArray();

	if (skipDefaults == false ||
		player.base.enableHover == false)
	{
		writer.Key("enableHover");
		writer.Bool(player.base.enableHover);
	}

	writer.Key("inventory");
	writer.StartArray();
	for (size_t i = 0; i < player.inventories.size(); i++)
	{
		const auto& inv = player.inventories[i];
		if (inv.Size() == 0)
		{
			continue;
		}

		writer.StartObject();

		writer.Key("index");
		writer.Uint(i);

		if (i == player.bodyInventoryIdx)
		{
			writer.Key("bodyInventory");
			writer.Bool(true);
		}

		writer.Key("item");
		writer.StartArray();
		for (const auto& item : inv)
		{
			serialize(serializeObj, level, item, skipDefaults);
		}
		writer.EndArray();

		writer.EndObject();
	}
	writer.EndArray();

	writer.EndObject();
}
