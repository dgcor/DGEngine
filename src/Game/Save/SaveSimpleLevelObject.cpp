#include "SaveSimpleLevelObject.h"
#include "Game/Level.h"
#include "Game/SimpleLevelObject.h"
#include "Json/JsonParser.h"

using namespace rapidjson;

void Save::serialize(void* serializeObj, const Level& level,
	const SimpleLevelObject& obj, bool skipDefaults)
{
	auto& writer = *((PrettyWriter<StringBuffer>*)serializeObj);

	writer.StartObject();

	writer.Key("id");
	writer.String(obj.Id());

	writer.Key("name");
	writer.String(obj.Name());

	writer.Key("mapPosition");
	writer.StartArray();
	writer.Int(obj.MapPosition().x);
	writer.Int(obj.MapPosition().y);
	writer.EndArray();

	writer.EndObject();
}
