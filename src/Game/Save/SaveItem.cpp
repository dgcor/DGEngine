#include "SaveItem.h"
#include "Game/Level.h"
#include "Game/ItemProperties.h"
#include "Json/JsonParser.h"

using namespace rapidjson;

void Save::serialize(void* serializeObj, const Level& level,
	const Item& item, bool skipDefaults)
{
	auto& writer = *((PrettyWriter<StringBuffer>*)serializeObj);
	const auto& itemClass = *item.Class();

	writer.StartObject();

	writer.Key("class");
	writer.String(itemClass.Id());

	if (item.MapPosition().x >= 0)
	{
		writer.Key("mapPosition");
		writer.StartArray();
		writer.Int(item.MapPosition().x);
		writer.Int(item.MapPosition().y);
		writer.EndArray();
	}

	writer.Key("properties");
	writer.StartObject();
	if (skipDefaults == false ||
		itemClass.isDefault({ ItemProp::Identified, item.Identified() }) == false)
	{
		writer.Key("identified");
		writer.Bool(item.Identified());
	}
	for (const auto& prop : item)
	{
		if (skipDefaults == true && itemClass.isDefault(prop) == true)
		{
			continue;
		}
		writer.Key(level.getPropertyName(prop.first));
		switch (prop.first)
		{
		case ItemProp::UseOn:
		case ItemProp::UseOp:
			writer.String(level.getPropertyName(prop.second));
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

	if (skipDefaults == false ||
		item.base.sprite.getOutline() != itemClass.DefaultOutline())
	{
		writer.Key("outline");
		writer.Uint(item.base.sprite.getOutline().toInteger());
	}

	if (skipDefaults == false ||
		item.base.sprite.getOutlineIgnore() != itemClass.DefaultOutlineIgnore())
	{
		writer.Key("outlineIgnore");
		writer.Uint(item.base.sprite.getOutlineIgnore().toInteger());
	}

	if (skipDefaults == false ||
		item.base.outlineOnHover != true)
	{
		writer.Key("outlineOnHover");
		writer.Bool(item.base.outlineOnHover);
	}

	writer.EndObject();
}
