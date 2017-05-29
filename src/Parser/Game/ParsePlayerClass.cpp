#include "ParsePlayerClass.h"
#include "Game/PlayerClass.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

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
		auto celTexture = game.Resources().getCelTextureCacheVec(getStringKey(elem, "name"));
		if (celTexture == nullptr)
		{
			return;
		}
		playerClass.setCelTexture(getUIntKey(elem, "palette"), celTexture);
	}

	void parsePlayerAnimationSpeed(PlayerClass& playerClass, const Value& elem)
	{
		playerClass.setSpeed(getPlayerStatusKey(elem, "name"), getPlayerAnimationSpeedVal(elem));
	}

	void parsePlayerClass(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false ||
			elem.HasMember("celTextures") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}

		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		if (level->getPlayerClass(id) != nullptr)
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

		if (elem.HasMember("defaults") == true)
		{
			const auto& defaults = elem["defaults"];
			if (defaults.IsObject() == true)
			{
				for (auto it = defaults.MemberBegin(); it != defaults.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						playerClass->setDefault(it->name.GetString(),
							getMinMaxNumber32Val(it->value));
					}
				}
			}
		}

		if (elem.HasMember("animationSpeeds") == true)
		{
			const auto& speeds = elem["animationSpeeds"];
			if (speeds.IsObject() == true)
			{
				parsePlayerAnimationSpeed(*playerClass, speeds);
			}
			else if (speeds.IsArray() == true)
			{
				for (const auto& val : speeds)
				{
					parsePlayerAnimationSpeed(*playerClass, val);
				}
			}
		}

		playerClass->MaxStrength(getIntKey(elem, "maxStrength", 250));
		playerClass->MaxMagic(getIntKey(elem, "maxMagic", 250));
		playerClass->MaxDexterity(getIntKey(elem, "maxDexterity", 250));
		playerClass->MaxVitality(getIntKey(elem, "maxVitality", 250));

		playerClass->MaxResistMagic(getIntKey(elem, "maxResistMagic", 100));
		playerClass->MaxResistFire(getIntKey(elem, "maxResistFire", 100));
		playerClass->MaxResistLightning(getIntKey(elem, "maxResistLightning", 100));

		if (elem.HasMember("lifeFormula") == true)
		{
			playerClass->setLifeFormula(getStringVal(elem["lifeFormula"]));
		}
		if (elem.HasMember("manaFormula") == true)
		{
			playerClass->setManaFormula(getStringVal(elem["manaFormula"]));
		}
		if (elem.HasMember("armorFormula") == true)
		{
			playerClass->setArmorFormula(getStringVal(elem["armorFormula"]));
		}
		if (elem.HasMember("toHitFormula") == true)
		{
			playerClass->setToHitFormula(getStringVal(elem["toHitFormula"]));
		}
		if (elem.HasMember("damageFormula") == true)
		{
			playerClass->setDamageFormula(getStringVal(elem["damageFormula"]));
		}

		level->addPlayerClass(id, playerClass);
	}
}
