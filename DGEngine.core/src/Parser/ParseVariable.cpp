#include "ParseVariable.h"
#include "Game/Game.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseVariable(Game& game, const Value& elem)
	{
		for (const auto& it : std::ranges::subrange(elem.MemberBegin(), elem.MemberEnd()))
		{
			auto key = it.name.GetStringStr();
			if (isValidId(key) == true)
			{
				Variable var;
				if (getVariableVal(it.value, var) == true)
				{
					game.Variables().setVariable(key, var);
				}
			}
		}
	}
}
