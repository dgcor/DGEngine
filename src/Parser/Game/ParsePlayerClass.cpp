#include "ParsePlayerClass.h"
#include "Game.h"
#include "Game/Level.h"
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

	PlayerClass* parsePlayerClassHelper(const Game& game,
		Level& level, const Value& elem, std::string& id)
	{
		if (isValidString(elem, "id") == false)
		{
			return nullptr;
		}
		id = std::string(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return nullptr;
		}

		auto playerClass = level.getClass<PlayerClass>(id);
		if (playerClass == nullptr)
		{
			if (level.hasClass(id) == true)
			{
				return nullptr;
			}

			std::unique_ptr<PlayerClass> playerClassPtr;

			if (isValidString(elem, "fromId") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				if (fromId != id)
				{
					auto obj = level.getClass<PlayerClass>(fromId);
					if (obj != nullptr)
					{
						playerClassPtr = std::make_unique<PlayerClass>(*obj);
					}
				}
			}
			else
			{
				playerClassPtr = std::make_unique<PlayerClass>();
			}
			if (playerClassPtr != nullptr)
			{
				playerClass = playerClassPtr.get();
				playerClass->Id(id);
				level.addClass(id, std::move(playerClassPtr));
			}
		}
		return playerClass;
	}

	void parsePlayerClass(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		std::string id;
		auto playerClass = parsePlayerClassHelper(game, *level, elem, id);
		if (playerClass == nullptr)
		{
			return;
		}

		playerClass->Id(id);

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

		if (elem.HasMember("anchorOffset") == true)
		{
			playerClass->setAnchorOffset(
				getVector2fVal<sf::Vector2f>(elem["anchorOffset"])
			);
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

		if (elem.HasMember("formulas") == true)
		{
			const auto& formulas = elem["formulas"];
			if (formulas.IsObject() == true)
			{
				for (auto it = formulas.MemberBegin(); it != formulas.MemberEnd(); ++it)
				{
					auto nameHash = str2int16(getStringViewVal(it->name));
					if (nameHash != str2int16(""))
					{
						playerClass->setFormula(nameHash, getStringVal(it->value));
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

		if (elem.HasMember("attackSound") == true)
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
