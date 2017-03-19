#include "ParseVariable.h"
#include "Game.h"
#include "GameUtils.h"
#include "InputText.h"
#include "Utils.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::vector<std::pair<std::string, Variable>> getVariables(const Value& elem)
	{
		std::vector<std::pair<std::string, Variable>> vars;
		for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
		{
			auto key = getStringVal(it->name);
			if (key.empty() == false)
			{
				const auto& value = it->value;
				Variable var;
				if (value.IsString())
				{
					var.set<std::string>((value.GetString()));
				}
				else if (value.IsInt64())
				{
					var.set<int64_t>(value.GetInt64());
				}
				else if (value.IsDouble())
				{
					var.set<double>(value.GetDouble());
				}
				else if (value.IsBool())
				{
					var.set<bool>(value.GetBool());
				}
				else
				{
					continue;
				}

				vars.push_back(std::make_pair(key, var));
			}
		}
		return vars;
	}

	void parseVariable(Game& game, const Value& elem)
	{
		for (auto it = elem.MemberBegin(); it != elem.MemberEnd(); ++it)
		{
			auto key = getStringVal(it->name);
			if (key.size() > 0)
			{
				const auto& value = it->value;
				Variable var;
				if (value.IsString())
				{
					var.set<std::string>(std::string(value.GetString()));
				}
				else if (value.IsInt64())
				{
					var.set<int64_t>(value.GetInt64());
				}
				else if (value.IsDouble())
				{
					var.set<double>(value.GetDouble());
				}
				else if (value.IsBool())
				{
					var.set<bool>(value.GetBool());
				}
				else
				{
					continue;
				}
				game.setVariable(key, var);
			}
		}
	}
}
