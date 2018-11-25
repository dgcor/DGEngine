#include "SaveSimpleLevelObject.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/SimpleLevelObject.h"
#include "Json/JsonParser.h"
#include "SaveUtils.h"

using namespace rapidjson;
using namespace SaveUtils;

void Save::serialize(void* serializeObj, Properties& props,
	const Game& game, const Level& level, const SimpleLevelObject& obj)
{
	auto& writer = *((PrettyWriter<StringBuffer>*)serializeObj);
	const auto& objClass = *obj.class_;

	writer.StartObject();

	writeString(writer, "id", obj.Id());
	writeString(writer, "class", objClass.Id());

	// only save name if it's not equal to classfier's generated name
	std::string nameFromClass;
	if (objClass.getFullName(obj, nameFromClass) == false &&
		nameFromClass.empty() == true)
	{
		nameFromClass = obj.SimpleName();
	}
	if (props.saveDefaults == true ||
		nameFromClass != obj.Name())
	{
		writeString(writer, "name", obj.Name());
	}

	writeVector2d<MapCoord>(writer, "mapPosition", obj.MapPosition());

	if (props.saveDefaults == true ||
		(props.saveDefaults == false && obj.Text1() != objClass.Text1()))
	{
		writeString(writer, "text1", obj.Text1());
	}
	if (props.saveDefaults == true ||
		(props.saveDefaults == false && obj.Text2() != objClass.Text2()))
	{
		writeString(writer, "text2", obj.Text2());
	}

	if (obj.properties.empty() == false)
	{
		writeKeyStringView(writer, "properties");
		writer.StartObject();
		for (const auto& prop : obj.properties)
		{
			if (props.saveDefaults == false && objClass.isDefault(prop) == true)
			{
				continue;
			}
			writeInt(writer, level.getPropertyName(prop.first), prop.second);
		}
		writer.EndObject();
	}

	writer.EndObject();
}
