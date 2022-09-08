#include "ParseLevelObject.h"
#include "Game/Level/Level.h"
#include "Game/LevelObject/LevelObjectQueryable.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	bool parseLevelObjectProperties(LevelObjectQueryable& levelObj, Level& level, const Value& elem)
	{
		if (elem.HasMember("properties"sv) == false)
		{
			return false;
		}
		const auto& props = elem["properties"sv];
		if (props.IsObject() == false)
		{
			return false;
		}
		for (auto it = props.MemberBegin(); it != props.MemberEnd(); ++it)
		{
			if (it->name.GetStringLength() > 0)
			{
				auto name = it->name.GetStringView();
				auto nameHash = str2int16(name);
				level.setPropertyName(nameHash, name);
				levelObj.setNumberByHash(nameHash, getMinMaxNumber32Val(it->value));
			}
		}
		return true;
	}
}
