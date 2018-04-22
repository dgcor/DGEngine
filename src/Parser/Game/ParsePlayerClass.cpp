#include "ParsePlayerClass.h"
#include "Game/PlayerClass.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "TexturePacks/CachedTexturePack.h"

namespace Parser
{
	using namespace rapidjson;

	void parsePlayerClassTexturePackIndex(PlayerClass& playerClass, const Value& elem)
	{
		playerClass.setStatusTexturePackIndex(getPlayerAnimationKey(elem, "name"),
			getUIntKey(elem, "index"));
	}

	void parsePlayerClassTexturePack(Game& game,
		PlayerClass& playerClass, const Value& elem)
	{
		auto texturePack = game.Resources().getTexturePack(getStringVal(elem));
		if (texturePack == nullptr)
		{
			return;
		}
		PlayerClass::Ranges ranges;
		auto cMultiTexPack = dynamic_cast<CachedMultiTexturePack*>(texturePack.get());
		if (cMultiTexPack != nullptr)
		{
			auto ranges2 = cMultiTexPack->getRanges();
			for (const auto& range : ranges2)
			{
				PlayerClass::Range range2;
				range2.range = range;
				range2.animType = AnimationType::Looped;
				ranges.push_back(range2);
			}
		}
		playerClass.addTexturePack(texturePack, ranges);
	}

	void parsePlayerClassTexturePackWithAnims(Game& game,
		PlayerClass& playerClass, const Value& elem)
	{
		auto texturePack = game.Resources().getTexturePack(getStringKey(elem, "texturePack"));
		if (texturePack == nullptr)
		{
			return;
		}
		PlayerClass::Ranges ranges;

		if (elem.HasMember("animations") == true &&
			elem["animations"].IsArray() == true)
		{
			for (const auto& val : elem["animations"])
			{
				if (val.IsArray() == true)
				{
					PlayerClass::Range range;
					range.range = getVector2uVal<std::pair<size_t, size_t>>(val);
					range.animType = AnimationType::Looped;
					ranges.push_back(range);
				}
				else if (val.IsObject() == true)
				{
					PlayerClass::Range range;
					range.range = getVector2uKey<std::pair<size_t, size_t>>(val, "range");
					range.animType = getAnimationTypeKey(val, "type");
					ranges.push_back(range);
				}
			}
		}

		playerClass.addTexturePack(texturePack, std::move(ranges));
	}

	void parsePlayerAnimationSpeed(PlayerClass& playerClass, const Value& elem)
	{
		playerClass.setSpeed(
			getPlayerAnimationKey(elem, "name"),
			getPlayerAnimationSpeedVal(elem)
		);
	}

	void parsePlayerSound(Game& game, PlayerClass& playerClass, const Value& elem)
	{
		auto id = getStringKey(elem, "id");
		if (isValidId(id) == false)
		{
			return;
		}
		auto sndBuffer = game.Resources().getSoundBuffer(id);
		if (sndBuffer == nullptr)
		{
			return;
		}
		auto idx = getIntKey(elem, "index");
		if (idx < 0)
		{
			return;
		}
		playerClass.setSound((size_t)idx, *sndBuffer);
	}

	void parsePlayerClass(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false)
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

		PlayerClass* playerClass = level->getPlayerClass(id);

		if (playerClass == nullptr)
		{
			auto playerClassPtr = std::make_unique<PlayerClass>();
			playerClass = playerClassPtr.get();
			playerClass->Id(id);
			level->addPlayerClass(id, std::move(playerClassPtr));
		}

		if (elem.HasMember("texturePacks") == true &&
			playerClass->hasTextures() == false)
		{
			const auto& texturePacks = elem["texturePacks"];

			if (texturePacks.IsString() == true)
			{
				parsePlayerClassTexturePack(game, *playerClass, texturePacks);
			}
			if (texturePacks.IsObject() == true)
			{
				parsePlayerClassTexturePackWithAnims(game, *playerClass, texturePacks);
			}
			else if (texturePacks.IsArray() == true)
			{
				for (const auto& val : texturePacks)
				{
					if (val.IsObject() == true)
					{
						parsePlayerClassTexturePackWithAnims(game, *playerClass, val);
					}
					else
					{
						parsePlayerClassTexturePack(game, *playerClass, val);
					}
				}
			}
		}

		if (elem.HasMember("textureIndexes") == true)
		{
			const auto& textureIndexes = elem["textureIndexes"];

			if (textureIndexes.IsObject() == true)
			{
				parsePlayerClassTexturePackIndex(*playerClass, textureIndexes);
			}
			else if (textureIndexes.IsArray() == true)
			{
				for (const auto& val : textureIndexes)
				{
					parsePlayerClassTexturePackIndex(*playerClass, val);
				}
			}
		}

		if (elem.HasMember("name") == true)
		{
			playerClass->Name(getStringVal(elem["name"]));
		}
		if (elem.HasMember("type") == true)
		{
			playerClass->Type(getStringVal(elem["type"]));
		}
		if (elem.HasMember("description") == true)
		{
			playerClass->Description(getStringVal(elem["description"]));
		}

		if (elem.HasMember("defaults") == true)
		{
			const auto& defaults = elem["defaults"];
			if (defaults.IsObject() == true)
			{
				for (auto it = defaults.MemberBegin(); it != defaults.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						auto nameHash = str2int16(it->name.GetString());
						level->setPropertyName(nameHash, it->name.GetString());
						playerClass->setDefaultByHash(nameHash,
							getMinMaxNumber32Val(it->value));
					}
				}
			}
		}

		if (elem.HasMember("actions") == true)
		{
			const auto& actions = elem["actions"];
			if (actions.IsObject() == true)
			{
				for (auto it = actions.MemberBegin(); it != actions.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						std::shared_ptr<Action> action;
						if (it->value.IsString() == true)
						{
							action = playerClass->getAction(str2int16(it->value.GetString()));
						}
						if (action == nullptr)
						{
							action = parseAction(game, it->value);
						}
						playerClass->setAction(str2int16(it->name.GetString()), action);
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

		if (elem.HasMember("sounds") == true)
		{
			const auto& sounds = elem["sounds"];
			if (sounds.IsObject() == true)
			{
				parsePlayerSound(game, *playerClass, sounds);
			}
			else if (sounds.IsArray() == true)
			{
				for (const auto& val : sounds)
				{
					parsePlayerSound(game, *playerClass, val);
				}
			}
		}

		if (elem.HasMember("defaultAttackSound") == true)
		{
			playerClass->setDefaultAttackSound((int16_t)getIntVal(elem["defaultAttackSound"], -1));
		}
		if (elem.HasMember("defaultDefendSound") == true)
		{
			playerClass->setDefaultDefendSound((int16_t)getIntVal(elem["defaultDefendSound"], -1));
		}
		if (elem.HasMember("defaultDieSound") == true)
		{
			playerClass->setDefaultDieSound((int16_t)getIntVal(elem["defaultDieSound"], -1));
		}
		if (elem.HasMember("defaultHitSound") == true)
		{
			playerClass->setDefaultHitSound((int16_t)getIntVal(elem["defaultHitSound"], -1));
		}
		if (elem.HasMember("defaultWalkSound") == true)
		{
			playerClass->setDefaultWalkSound((int16_t)getIntVal(elem["defaultWalkSound"], -1));
		}

		if (elem.HasMember("maxStrength") == true)
		{
			playerClass->MaxStrength(getIntVal(elem["maxStrength"], 250));
		}
		if (elem.HasMember("maxMagic") == true)
		{
			playerClass->MaxMagic(getIntVal(elem["maxMagic"], 250));
		}
		if (elem.HasMember("maxDexterity") == true)
		{
			playerClass->MaxDexterity(getIntVal(elem["maxDexterity"], 250));
		}
		if (elem.HasMember("maxVitality") == true)
		{
			playerClass->MaxVitality(getIntVal(elem["maxVitality"], 250));
		}

		if (elem.HasMember("maxResistMagic") == true)
		{
			playerClass->MaxResistMagic(getIntVal(elem["maxResistMagic"], 100));
		}
		if (elem.HasMember("maxResistFire") == true)
		{
			playerClass->MaxResistFire(getIntVal(elem["maxResistFire"], 100));
		}
		if (elem.HasMember("maxResistLightning") == true)
		{
			playerClass->MaxResistLightning(getIntVal(elem["maxResistLightning"], 100));
		}

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
		if (elem.HasMember("resistMagicFormula") == true)
		{
			playerClass->setResistMagicFormula(getStringVal(elem["resistMagicFormula"]));
		}
		if (elem.HasMember("resistFireFormula") == true)
		{
			playerClass->setResistFireFormula(getStringVal(elem["resistFireFormula"]));
		}
		if (elem.HasMember("resistLightningFormula") == true)
		{
			playerClass->setResistLightningFormula(getStringVal(elem["resistLightningFormula"]));
		}
		if (elem.HasMember("defaultOutline") == true)
		{
			playerClass->DefaultOutline(getColorVal(elem["defaultOutline"], sf::Color::Transparent));
		}
		if (elem.HasMember("defaultOutlineIgnore") == true)
		{
			playerClass->DefaultOutlineIgnore(getColorVal(elem["defaultOutlineIgnore"], sf::Color::Transparent));
		}
	}
}
