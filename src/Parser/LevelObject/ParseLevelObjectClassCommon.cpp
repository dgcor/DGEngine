#include "ParseLevelObjectClassCommon.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parseLevelObjectClassActions(LevelObjectClass& levelObjClass, Game& game, const Value& elem)
	{
		if (elem.HasMember("actions"sv) == false)
		{
			return;
		}
		const auto& actions = elem["actions"sv];
		if (actions.IsObject() == false)
		{
			return;
		}
		for (auto it = actions.MemberBegin(); it != actions.MemberEnd(); ++it)
		{
			if (it->name.GetStringLength() > 0)
			{
				std::shared_ptr<Action> action;
				if (it->value.IsString() == true)
				{
					action = levelObjClass.getAction(str2int16(getStringViewVal(it->value)));
				}
				if (action == nullptr)
				{
					action = getActionVal(game, it->value);
				}
				levelObjClass.setAction(str2int16(getStringViewVal(it->name)), action);
			}
		}
	}
}
