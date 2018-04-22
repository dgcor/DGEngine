#include "SaveLevel.h"
#include "FileUtils.h"
#include "Game/Level.h"
#include "Game/SimpleLevelObject.h"
#include "Json/JsonParser.h"
#include "SaveItem.h"

using namespace rapidjson;

void Save::save(const std::string& filePath, const Level& level,
	bool skipDefaults, bool skipCurrentPlayer)
{
	StringBuffer buffer(0, std::numeric_limits<uint16_t>::max());
	PrettyWriter<StringBuffer> writer(buffer);
	writer.SetIndent(' ', 2);

	serialize(&writer, level, skipDefaults, skipCurrentPlayer);

	FileUtils::saveText(filePath.c_str(), buffer.GetString(), buffer.GetSize());
}

void Save::save(const std::string& filePath, const Level& level,
	const LevelObject& obj, bool skipDefaults)
{
	StringBuffer buffer(0, std::numeric_limits<uint16_t>::max());
	PrettyWriter<StringBuffer> writer(buffer);
	writer.SetIndent(' ', 2);

	writer.StartObject();
	Variable var;
	if (obj.getProperty("type", var) == true)
	{
		writer.Key(std::get<std::string>(var));
	}
	else
	{
		writer.Key("levelObject");
	}
	obj.serialize(&writer, level, skipDefaults);
	writer.EndObject();

	FileUtils::saveText(filePath.c_str(), buffer.GetString(), buffer.GetSize());
}

void Save::serialize(void* serializeObj, const Level& level,
	bool skipDefaults, bool skipCurrentPlayer)
{
	auto& writer = *((PrettyWriter<StringBuffer>*)serializeObj);

	writer.StartObject();

	writer.Key("level");
	writer.StartObject();

	writer.Key("id");
	writer.String(level.id);

	writer.Key("name");
	writer.String(level.name);

	writer.Key("path");
	writer.String(level.path);

	writer.Key("mapPosition");
	writer.StartArray();
	writer.Int(level.map.MapSize().x);
	writer.Int(level.map.MapSize().y);
	writer.EndArray();

	//writer.Key("map");
	//writer.StartArray();
	//for (const auto& cell : level.map)
	//{
	//	writer.Int(cell.TileIndexBack());
	//}
	//writer.EndArray();

	writer.EndObject();

	writer.Key("item");
	writer.StartArray();
	for (const auto& obj : level.levelObjects)
	{
		auto item = dynamic_cast<Item*>(obj.get());
		if (item != nullptr)
		{
			serialize(serializeObj, level, *item, skipDefaults);
		}
	}
	writer.EndArray();

	writer.Key("levelObject");
	writer.StartArray();
	for (const auto& obj : level.levelObjects)
	{
		auto levelObj = dynamic_cast<SimpleLevelObject*>(obj.get());
		if (levelObj != nullptr)
		{
			serialize(serializeObj, level, *levelObj, skipDefaults);
		}
	}
	writer.EndArray();

	writer.Key("player");
	writer.StartArray();
	for (const auto& obj : level.players)
	{
		auto player = obj.get();
		if (player != nullptr)
		{
			if (skipCurrentPlayer == true &&
				player == level.getCurrentPlayer())
			{
				continue;
			}
			serialize(serializeObj, level, *player, skipDefaults);
		}
	}
	writer.EndArray();

	writer.EndObject();
}
