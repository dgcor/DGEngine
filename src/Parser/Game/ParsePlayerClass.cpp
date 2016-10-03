#include "ParsePlayerClass.h"
#include "Parser/ParseAction.h"
#include "Parser/ParseUtils.h"
#include "Game/PlayerClass.h"

namespace Parser
{
	using namespace rapidjson;

	void parsePlayerClassCelIndex(PlayerClass& playerClass, const Value& elem)
	{
		playerClass.setStatusCelIndex(getPlayerStatusKey(elem, "name"),
			getUIntKey(elem, "index"));
	}

	void parsePlayerClassCelTexture(Game& game,
		PlayerClass& playerClass, const Value& elem)
	{
		auto celTexture = game.Resources().getCelTextureCache(getStringKey(elem, "name"));
		if (celTexture == nullptr)
		{
			return;
		}
		playerClass.setCelTexture(getUIntKey(elem, "palette"), celTexture);
	}

	void parsePlayerClass(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false ||
			elem.HasMember("celTextures") == false)
		{
			return;
		}

		auto level = game.Resources().getLevel(getStringKey(elem, "idLevel"));
		if (level == nullptr)
		{
			return;
		}

		auto playerClass = std::make_shared<PlayerClass>();

		const auto& celTextures = elem["celTextures"];

		if (celTextures.IsObject() == true)
		{
			parsePlayerClassCelTexture(game, *playerClass, celTextures);
		}
		else if (celTextures.IsArray() == true)
		{
			for (const auto& val : celTextures)
			{
				parsePlayerClassCelTexture(game, *playerClass, val);
			}
		}

		if (playerClass->getCelTexture(0) == nullptr)
		{
			return;
		}

		if (elem.HasMember("celIndexes") == true)
		{
			const auto& celIndexes = elem["celIndexes"];

			if (celIndexes.IsObject() == true)
			{
				parsePlayerClassCelIndex(*playerClass, celIndexes);
			}
			else if (celIndexes.IsArray() == true)
			{
				for (const auto& val : celIndexes)
				{
					parsePlayerClassCelIndex(*playerClass, val);
				}
			}
		}

		playerClass->Name(getStringKey(elem, "name"));
		playerClass->Type(getStringKey(elem, "type"));
		playerClass->Description(getStringKey(elem, "description"));

		level->addPlayerClass(elem["id"].GetString(), playerClass);
	}
}
