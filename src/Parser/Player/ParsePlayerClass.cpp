#include "ParsePlayerClass.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/Player/PlayerClass.h"
#include "Parser/ParseAction.h"
#include "Parser/LevelObject/ParseLevelObjectClassCommon.h"
#include "Parser/Utils/ParseUtils.h"
#include "Parser/Utils/ParseUtilsGameKey.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parsePlayerClassTexturePackIndex(PlayerClass& playerClass, const Value& elem)
	{
		playerClass.setStatusTexturePackIndex(getPlayerAnimationKey(elem, "name"),
			getUIntKey(elem, "index"));
	}

	void parsePlayerClassTexturePack(Game& game, PlayerClass& playerClass, const Value& elem)
	{
		auto id = getStringViewVal(elem);
		auto texturePack = game.Resources().getTexturePack(id);
		if (texturePack != nullptr)
		{
			playerClass.addTexturePack(texturePack);
			return;
		}
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
		auto sndId = getStringViewKey(elem, "sound");
		if (isValidId(sndId) == false)
		{
			return;
		}
		auto sndBuffer = game.Resources().getSoundBuffer(sndId);
		if (sndBuffer == nullptr)
		{
			return;
		}
		auto id = getStringViewKey(elem, "id");
		if (isValidId(id) == false)
		{
			return;
		}
		playerClass.addSound(id, *sndBuffer);
	}

	PlayerClass* parsePlayerClassHelper(const Game& game, Level& level, const Value& elem, std::string& id)
	{
		if (isValidString(elem, "id") == false)
		{
			return nullptr;
		}
		id = elem["id"sv].GetStringView();
		if (isValidId(id) == false)
		{
			return nullptr;
		}

		auto playerClass = level.LevelObjects().getClass<PlayerClass>(id);
		if (playerClass == nullptr)
		{
			if (level.LevelObjects().hasClass(id) == true)
			{
				return nullptr;
			}

			std::unique_ptr<PlayerClass> playerClassPtr;

			if (isValidString(elem, "fromId") == true)
			{
				auto fromId = elem["fromId"sv].GetStringView();
				if (fromId != id)
				{
					auto obj = level.LevelObjects().getClass<PlayerClass>(fromId);
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
				level.LevelObjects().addClass(id, std::move(playerClassPtr));
			}
		}
		return playerClass;
	}

	void parsePlayerClass(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel<Level>(getStringViewKey(elem, "level"));
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

		if (elem.HasMember("texturePacks"sv) == true &&
			playerClass->hasTextures() == false)
		{
			const auto& texturePacks = elem["texturePacks"sv];

			if (texturePacks.IsString() == true)
			{
				parsePlayerClassTexturePack(game, *playerClass, texturePacks);
			}
			else if (texturePacks.IsArray() == true)
			{
				for (const auto& val : texturePacks)
				{
					if (val.IsString() == true)
					{
						parsePlayerClassTexturePack(game, *playerClass, val);
					}
				}
			}
		}

		if (elem.HasMember("textureIndexes"sv) == true)
		{
			const auto& textureIndexes = elem["textureIndexes"sv];

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

		if (elem.HasMember("anchorOffset"sv) == true)
		{
			playerClass->setAnchorOffset(
				getVector2fVal<sf::Vector2f>(elem["anchorOffset"sv])
			);
		}
		if (elem.HasMember("name"sv) == true)
		{
			playerClass->Name(getStringViewVal(elem["name"sv]));
		}
		if (elem.HasMember("type"sv) == true)
		{
			playerClass->Type(getStringViewVal(elem["type"sv]));
		}

		if (elem.HasMember("nameClassifier"sv) == true)
		{
			playerClass->setNameClassifier(
				level->LevelObjects().getClassifier(getStringViewKey(elem, "nameClassifier")));
		}

		parseLevelObjectClassActions(*playerClass, game, elem);
		parseLevelObjectClassDefaults(*playerClass, *level, elem);
		parseLevelObjectClassDescriptions(*playerClass, *level, elem);
		parseLevelObjectClassFormulas(*playerClass, elem);

		if (elem.HasMember("animationSpeeds"sv) == true)
		{
			const auto& speeds = elem["animationSpeeds"sv];
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

		if (elem.HasMember("sounds"sv) == true)
		{
			const auto& sounds = elem["sounds"sv];
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

		if (elem.HasMember("maxStrength"sv) == true)
		{
			playerClass->MaxStrength(getIntVal(elem["maxStrength"sv], 250));
		}
		if (elem.HasMember("maxMagic"sv) == true)
		{
			playerClass->MaxMagic(getIntVal(elem["maxMagic"sv], 250));
		}
		if (elem.HasMember("maxDexterity"sv) == true)
		{
			playerClass->MaxDexterity(getIntVal(elem["maxDexterity"sv], 250));
		}
		if (elem.HasMember("maxVitality"sv) == true)
		{
			playerClass->MaxVitality(getIntVal(elem["maxVitality"sv], 250));
		}
		if (elem.HasMember("maxResistMagic"sv) == true)
		{
			playerClass->MaxResistMagic(getIntVal(elem["maxResistMagic"sv], 100));
		}
		if (elem.HasMember("maxResistFire"sv) == true)
		{
			playerClass->MaxResistFire(getIntVal(elem["maxResistFire"sv], 100));
		}
		if (elem.HasMember("maxResistLightning"sv) == true)
		{
			playerClass->MaxResistLightning(getIntVal(elem["maxResistLightning"sv], 100));
		}

		if (elem.HasMember("outline"sv) == true)
		{
			playerClass->Outline(getColorVal(elem["outline"sv], sf::Color::Transparent));
		}
		if (elem.HasMember("outlineIgnore"sv) == true)
		{
			playerClass->OutlineIgnore(getColorVal(elem["outlineIgnore"sv], sf::Color::Transparent));
		}
		if (elem.HasMember("light"sv) == true)
		{
			playerClass->setLightSource(
				getLightSourceVal(elem["light"sv], { 255, 10 })
			);
		}
	}
}
