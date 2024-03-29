#include "SimpleLevelObjectSave.h"
#include "Game/Level/Level.h"
#include "Game/SimpleLevelObject/SimpleLevelObject.h"
#include "Json/JsonParser.h"
#include "Json/SaveUtils.h"

using namespace rapidjson;
using namespace SaveUtils;

void SimpleLevelObjectSave::serialize(const SimpleLevelObject& obj, const Level& level, void* serializeObj, const UnorderedStringMap<Variable>& props)
{
	auto& writer = *((PrettyWriter<StringBuffer>*)serializeObj);
	const auto& objClass = *obj.Class();

	bool saveDefaults = getBoolProperty(props, "saveDefaults");

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
	if (saveDefaults == true ||
		nameFromClass != obj.Name())
	{
		writeString(writer, "name", obj.Name());
	}

	writeVector2fi(writer, "mapPosition", obj.MapPosition());

	if (saveDefaults == true ||
		(saveDefaults == false && obj.getCellSize() != objClass.getCellSize()))
	{
		writeVector2i(writer, "size", obj.getCellSize());
	}
	if (saveDefaults == true ||
		(saveDefaults == false && obj.Text1() != objClass.Text1()))
	{
		writeString(writer, "text1", obj.Text1());
	}
	if (saveDefaults == true ||
		(saveDefaults == false && obj.Text2() != objClass.Text2()))
	{
		writeString(writer, "text2", obj.Text2());
	}

	if (obj.properties.empty() == false)
	{
		writeKeyStringView(writer, "properties");
		writer.StartObject();
		for (const auto& prop : obj.properties)
		{
			if (saveDefaults == false && objClass.isDefault(prop) == true)
			{
				continue;
			}
			writeInt(writer, level.getPropertyName(prop.first), prop.second);
		}
		writer.EndObject();
	}

	writer.EndObject();
}
