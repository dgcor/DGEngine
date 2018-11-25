#include "SaveItem.h"
#include "Game/GameHashes.h"
#include "Game/Level.h"
#include "Json/JsonParser.h"
#include "SaveUtils.h"

using namespace rapidjson;
using namespace SaveUtils;

void Save::serialize(void* serializeObj, Properties& props,
	const Game& game, const Level& level, const Item& item)
{
	auto& writer = *((PrettyWriter<StringBuffer>*)serializeObj);
	const auto& itemClass = *item.Class();

	writer.StartObject();

	if (props.customProperty != nullptr)
	{
		writeUInt(writer, "index", *((size_t*)props.customProperty));
	}

	writeString(writer, "class", itemClass.Id());

	if (item.MapPosition().x >= 0)
	{
		writeVector2d<MapCoord>(writer, "mapPosition", item.MapPosition());
	}

	if (item.properties.empty() == false)
	{
		writeKeyStringView(writer, "properties");
		writer.StartObject();
		if (props.saveDefaults == true ||
			itemClass.isDefault({ ItemProp::Identified, item.Identified() }) == false)
		{
			writeBool(writer, "identified", item.Identified());
		}
		for (const auto& prop : item)
		{
			if (props.saveDefaults == false && itemClass.isDefault(prop) == true)
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

	if (props.saveDefaults == true ||
		item.base.sprite.getOutline() != itemClass.Outline())
	{
		writeUInt(writer, "outline", item.base.sprite.getOutline().toInteger());
	}

	if (props.saveDefaults == true ||
		item.base.sprite.getOutlineIgnore() != itemClass.OutlineIgnore())
	{
		writeUInt(writer, "outlineIgnore", item.base.sprite.getOutlineIgnore().toInteger());
	}

	if (props.saveDefaults == true ||
		item.base.outlineOnHover != true)
	{
		writeBool(writer, "outlineOnHover", item.base.outlineOnHover);
	}

	writer.EndObject();
}
