#include "SaveLevel.h"
#include "FileUtils.h"
#include "Game/Level.h"
#include "Game/Player.h"
#include "Game/SimpleLevelObject.h"
#include "Json/JsonParser.h"
#include "SaveItem.h"
#include "SaveUtils.h"

using namespace rapidjson;
using namespace SaveUtils;

void Save::save(const std::string_view filePath, Properties& props,
	const Game& game, const Level& level)
{
	StringBuffer buffer(0, std::numeric_limits<uint16_t>::max());
	PrettyWriter<StringBuffer> writer(buffer);
	writer.SetIndent(' ', 2);

	serialize(&writer, props, game, level);

	FileUtils::saveText(filePath, { buffer.GetString(), buffer.GetSize() });
}

void Save::serialize(void* serializeObj, Properties& props,
	const Game& game, const Level& level)
{
	auto& writer = *((PrettyWriter<StringBuffer>*)serializeObj);

	// root
	writer.StartObject();

	writeKeyStringView(writer, "level");
	// level
	writer.StartObject();

	writeStringView(writer, "id", level.id);
	writeStringView(writer, "name", level.name);
	writeStringView(writer, "path", level.path);

	writeVector2i(writer, "mapSize", level.map.MapSizei());

	writeKeyStringView(writer, "map");
	// map
	writer.StartObject();
	writeKeyStringView(writer, "layers");
	writer.StartArray();
	for (size_t i = 0; i < LevelCell::NumberOfLayers; i++)
	{
		if (level.map.isLayerUsed(i) == false)
		{
			continue;
		}

		// layer
		writer.StartObject();

		if (i == LevelCell::FlagsLayer)
		{
			writeString(writer, "index", "flags");
		}
		else
		{
			writeInt(writer, "index", i);
		}
		writeInt(writer, "width", level.map.MapSizei().x);
		writeInt(writer, "height", level.map.MapSizei().y);

		writeKeyStringView(writer, "data");
		writer.SetFormatOptions(PrettyFormatOptions::kFormatSingleLineArray);
		writer.StartArray();
		for (const auto& cell : level.map)
		{
			writer.Int(cell.getTileIndex(i));
		}
		writer.EndArray();
		writer.SetFormatOptions(PrettyFormatOptions::kFormatDefault);

		// layer
		writer.EndObject();
	}
	writer.EndArray();
	// map
	writer.EndObject();

	// level
	writer.EndObject();

	if (props.saveQuests == true)
	{
		writeKeyStringView(writer, "quest");
		writer.StartArray();
		for (const auto& quest : level.quests)
		{
			writer.StartObject();
			writeString(writer, "id", quest.Id());
			writeString(writer, "name", quest.Name());
			writeInt(writer, "state", quest.State());
			writer.EndObject();
		}
		writer.EndArray();
	}

	writeKeyStringView(writer, "item");
	writer.StartArray();
	for (const auto& obj : level.levelObjects)
	{
		auto item = dynamic_cast<Item*>(obj.get());
		if (item != nullptr)
		{
			serialize(serializeObj, props, game, level, *item);
		}
	}
	writer.EndArray();

	writeKeyStringView(writer, "levelObject");
	writer.StartArray();
	for (const auto& obj : level.levelObjects)
	{
		auto levelObj = dynamic_cast<SimpleLevelObject*>(obj.get());
		if (levelObj != nullptr)
		{
			serialize(serializeObj, props, game, level, *levelObj);
		}
	}
	writer.EndArray();

	writeKeyStringView(writer, "player");
	writer.StartArray();
	for (const auto& obj : level.levelObjects)
	{
		auto player = dynamic_cast<Player*>(obj.get());
		if (player != nullptr)
		{
			if (props.saveCurrentPlayer == false &&
				player == level.getCurrentPlayer())
			{
				continue;
			}
			serialize(serializeObj, props, game, level, *player);
		}
	}
	writer.EndArray();

	// root
	writer.EndObject();
}
