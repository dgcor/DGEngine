#include "SaveItem.h"
#include "Game/GameHashes.h"
#include "Game/Level.h"
#include "Game/Save/SaveUtils.h"
#include "Json/JsonParser.h"

using namespace rapidjson;
using namespace SaveUtils;

void Save::serialize(void* serializeObj, const Properties& props,
	const Game& game, const Level& level, const Item& item)
{
	auto& writer = *((PrettyWriter<StringBuffer>*)serializeObj);
	const auto& itemClass = *item.Class();

	bool saveDefaults = getBoolProperty(props, "saveDefaults");

	writer.StartObject();

	if (hasProperty(props, "index") == true)
	{
		writeUInt(writer, "index", (unsigned)getInt64Property(props, "index"));
	}

	writeString(writer, "class", itemClass.Id());

	if (item.MapPosition().x >= 0)
	{
		writeVector2fi(writer, "mapPosition", item.MapPosition());
	}

	if (item.properties.empty() == false)
	{
		writeKeyStringView(writer, "properties");
		writer.StartObject();
		if (saveDefaults == true ||
			itemClass.isDefault({ ItemProp::Identified, item.Identified() }) == false)
		{
			writeBool(writer, "identified", item.Identified());
		}
		for (const auto& prop : item)
		{
			if (saveDefaults == false && itemClass.isDefault(prop) == true)
			{
				continue;
			}
			writeKeyStringView(writer, level.getPropertyName(prop.first));
			switch (prop.first)
			{
			case ItemProp::UseOn:
			case ItemProp::UseOp:
				writeStringView(writer, level.getPropertyName(prop.second));
				break;
			case ItemProp::Magical:
				writer.Bool(prop.second != 0);
				break;
			default:
				writer.Int(prop.second);
				break;
			}
		}
		writer.EndObject();
	}

	if (saveDefaults == true ||
		item.sprite.getOutline() != itemClass.Outline())
	{
		writeUInt(writer, "outline", item.sprite.getOutline().toInteger());
	}

	if (saveDefaults == true ||
		item.sprite.getOutlineIgnore() != itemClass.OutlineIgnore())
	{
		writeUInt(writer, "outlineIgnore", item.sprite.getOutlineIgnore().toInteger());
	}

	if (saveDefaults == true ||
		item.outlineOnHover != true)
	{
		writeBool(writer, "outlineOnHover", item.outlineOnHover);
	}

	writer.EndObject();
}
