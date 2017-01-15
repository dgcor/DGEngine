#include "ParseUtilsVar.h"
#include "Game.h"
#include "SFMLUtils.h"

namespace Parser
{
	sf::Color getColorVar(Game& game, const rapidjson::Value& elem,
		const char* key, const sf::Color& val)
	{
		if (elem.HasMember(key) && elem[key].IsString())
		{
			std::string colorStr(elem[key].GetString());
			Variable var;
			try
			{
				if (game.getVariable(colorStr, var) == true)
				{
					if (var.is<std::string>() == true)
					{
						return SFMLUtils::stringToColor(var.get<std::string>());
					}
				}
				else
				{
					return SFMLUtils::stringToColor(colorStr);
				}
			}
			catch (std::exception ex) {}
		}
		return val;
	}
}
