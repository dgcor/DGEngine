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

	void parseDescriptionValue(PlayerClass& playerClass,
		const Level& level, const Value& elem)
	{
		playerClass.setDescription(
			getUIntKey(elem, "index"),
			level.getClassifier(getStringKey(elem, "name")),
			(uint16_t)getUIntKey(elem, "skip"));
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

		PlayerClass* playerClass = level->getClass<PlayerClass>(id);

		if (playerClass == nullptr)
		{
			if (level->hasClass(id) == true)
			{
				return;
			}
			auto playerClassPtr = std::make_unique<PlayerClass>();
			playerClass = playerClassPtr.get();
			playerClass->Id(id);
			level->addClass(id, std::move(playerClassPtr));
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

		if (elem.HasMember("nameClassifier") == true)
		{
			playerClass->setNameClassifier(
				level->getClassifier(getStringKey(elem, "nameClassifier")));
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
						auto name = getStringViewVal(it->name);
						auto nameHash = str2int16(name);
						level->setPropertyName(nameHash, name);
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
							action = playerClass->getAction(str2int16(getStringViewVal(it->value)));
						}
						if (action == nullptr)
						{
							action = parseAction(game, it->value);
						}
						playerClass->setAction(str2int16(getStringViewVal(it->name)), action);
					}
				}
			}
		}

		if (elem.HasMember("descriptions") == true)
		{
			const auto& descriptions = elem["descriptions"];
			if (descriptions.IsObject() == true)
			{
				parseDescriptionValue(*playerClass, *level, descriptions);
			}
			else if (descriptions.IsArray() == true)
			{
				for (const auto& val : descriptions)
				{
					parseDescriptionValue(*playerClass, *level, val);
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

		if (elem.HasMember("aAttackSound") == true)
		{
			playerClass->setAttackSound((int16_t)getIntVal(elem["attackSound"], -1));
		}
		if (elem.HasMember("defendSound") == true)
		{
			playerClass->setDefendSound((int16_t)getIntVal(elem["defendSound"], -1));
		}
		if (elem.HasMember("dieSound") == true)
		{
			playerClass->setDieSound((int16_t)getIntVal(elem["dieSound"], -1));
		}
		if (elem.HasMember("hitSound") == true)
		{
			playerClass->setHitSound((int16_t)getIntVal(elem["hitSound"], -1));
		}
		if (elem.HasMember("walkSound") == true)
		{
			playerClass->setWalkSound((int16_t)getIntVal(elem["walkSound"], -1));
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
			playerClass->setLifeFormula(getStringViewVal(elem["lifeFormula"]));
		}
		if (elem.HasMember("manaFormula") == true)
		{
			playerClass->setManaFormula(getStringViewVal(elem["manaFormula"]));
		}
		if (elem.HasMember("armorFormula") == true)
		{
			playerClass->setArmorFormula(getStringViewVal(elem["armorFormula"]));
		}
		if (elem.HasMember("toHitFormula") == true)
		{
			playerClass->setToHitFormula(getStringViewVal(elem["toHitFormula"]));
		}
		if (elem.HasMember("damageFormula") == true)
		{
			playerClass->setDamageFormula(getStringViewVal(elem["damageFormula"]));
		}
		if (elem.HasMember("resistMagicFormula") == true)
		{
			playerClass->setResistMagicFormula(getStringViewVal(elem["resistMagicFormula"]));
		}
		if (elem.HasMember("resistFireFormula") == true)
		{
			playerClass->setResistFireFormula(getStringViewVal(elem["resistFireFormula"]));
		}
		if (elem.HasMember("resistLightningFormula") == true)
		{
			playerClass->setResistLightningFormula(getStringViewVal(elem["resistLightningFormula"]));
		}
		if (elem.HasMember("outline") == true)
		{
			playerClass->Outline(getColorVal(elem["outline"], sf::Color::Transparent));
		}
		if (elem.HasMember("outlineIgnore") == true)
		{
			playerClass->OutlineIgnore(getColorVal(elem["outlineIgnore"], sf::Color::Transparent));
		}
		if (elem.HasMember("light") == true)
		{
			playerClass->setLightSource(
				getLightSourceVal(elem["light"], { 0, 255, 16, LightEasing::Linear })
			);
		}
	}
}
