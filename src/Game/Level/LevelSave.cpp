#include "LevelSave.h"
#include "Game/Level/Level.h"
#include "Game/Player/Player.h"
#include "Game/SimpleLevelObject/SimpleLevelObject.h"
#include "Game/Utils/FileUtils.h"
#include "Json/JsonParser.h"
#include "Json/SaveUtils.h"

using namespace rapidjson;
using namespace SaveUtils;

void LevelSave::save(const Level& level, const std::string_view filePath, const UnorderedStringMap<Variable>& props)
{
	StringBuffer buffer(0, std::numeric_limits<uint16_t>::max());
	PrettyWriter<StringBuffer> writer(buffer);
	writer.SetIndent(' ', 2);

	serialize(level, &writer, props);

	FileUtils::saveText(filePath, { buffer.GetString(), buffer.GetSize() });
}

void LevelSave::serialize(const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props)
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
	for (uint32_t i = 0; i < LevelCell::NumberOfLayers; i++)
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

	if (getBoolProperty(props, "saveQuests") == true)
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
	for (const auto& obj : level.levelObjects.Objects())
	{
		auto item = dynamic_cast<Item*>(obj.get());
		if (item != nullptr)
		{
			item->serialize(level, serializeObj, props);
		}
	}
	writer.EndArray();

	writeKeyStringView(writer, "levelObject");
	writer.StartArray();
	for (const auto& obj : level.levelObjects.Objects())
	{
		auto levelObj = dynamic_cast<SimpleLevelObject*>(obj.get());
		if (levelObj != nullptr)
		{
			levelObj->serialize(level, serializeObj, props);
		}
	}
	writer.EndArray();

	writeKeyStringView(writer, "player");
	writer.StartArray();
	auto currentPlayer = level.levelObjects.CurrentPlayer();
	for (const auto& obj : level.levelObjects.Objects())
	{
		auto player = dynamic_cast<Player*>(obj.get());
		if (player != nullptr)
		{
			if (getBoolProperty(props, "saveCurrentPlayer") == false &&
				player == currentPlayer.get())
			{
				continue;
			}
			player->serialize(level, serializeObj, props);
		}
	}
	writer.EndArray();

	// root
	writer.EndObject();
}
