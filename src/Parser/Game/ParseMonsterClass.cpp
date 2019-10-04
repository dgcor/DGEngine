#include "ParseMonsterClass.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/MonsterClass.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"
#include "TexturePacks/CachedTexturePack.h"

namespace Parser
{
	using namespace rapidjson;

	void parseMonsterClassTexturePackIndex(MonsterClass& monsterClass, const Value& elem)
	{
		monsterClass.setStatusTexturePackIndex(getCharacterAnimationKey(elem, "name"),
			getUIntKey(elem, "index"));
	}

	void parseMonsterClassTexturePack(Game& game,
		MonsterClass& monsterClass, const Value& elem)
	{
		auto id = getStringVal(elem);
		auto texturePack = game.Resources().getTexturePack(id);
		if (texturePack != nullptr)
		{
			monsterClass.addTexturePack(std::move(texturePack));
			return;
		}
		auto compTex = game.Resources().getCompositeTexture(id);
		if (compTex != nullptr)
		{
			monsterClass.addTexturePack(std::move(compTex));
			return;
		}
	}

	void parseMonsterAnimationSpeed(MonsterClass& monsterClass, const Value& elem)
	{
		monsterClass.setSpeed(
			getCharacterAnimationKey(elem, "name"),
			getCharacterAnimationSpeedVal(elem)
		);
	}

	void parseMonsterSound(Game& game, MonsterClass& monsterClass, const Value& elem)
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
		monsterClass.setSound((size_t)idx, *sndBuffer);
	}

	void parseDescriptionValue(MonsterClass& monsterClass,
		const Level& level, const Value& elem)
	{
		monsterClass.setDescription(
			getUIntKey(elem, "index"),
			level.getClassifier(getStringKey(elem, "name")),
			(uint16_t)getUIntKey(elem, "skip"));
	}

	MonsterClass* parseMonsterClassHelper(const Game& game,
		Level& level, const Value& elem, std::string& id)
	{
		if (isValidString(elem, "id") == false)
		{
			return nullptr;
		}
		id = elem["id"].GetStringStr();
		if (isValidId(id) == false)
		{
			return nullptr;
		}

		auto monsterClass = level.getClass<MonsterClass>(id);
		if (monsterClass == nullptr)
		{
			if (level.hasClass(id) == true)
			{
				return nullptr;
			}

			std::unique_ptr<MonsterClass> monsterClassPtr;

			if (isValidString(elem, "fromId") == true)
			{
				auto fromId = elem["fromId"].GetStringStr();
				if (fromId != id)
				{
					auto obj = level.getClass<MonsterClass>(fromId);
					if (obj != nullptr)
					{
						monsterClassPtr = std::make_unique<MonsterClass>(*obj);
					}
				}
			}
			else
			{
				monsterClassPtr = std::make_unique<MonsterClass>();
			}
			if (monsterClassPtr != nullptr)
			{
				monsterClass = monsterClassPtr.get();
				monsterClass->Id(id);
				level.addClass(id, std::move(monsterClassPtr));
			}
		}
		return monsterClass;
	}

	void parseMonsterClass(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		std::string id;
		auto monsterClass = parseMonsterClassHelper(game, *level, elem, id);
		if (monsterClass == nullptr)
		{
			return;
		}

		monsterClass->Id(id);

		if (elem.HasMember("texturePacks") == true &&
			monsterClass->hasTextures() == false)
		{
			const auto& texturePacks = elem["texturePacks"];

			if (texturePacks.IsString() == true)
			{
				parseMonsterClassTexturePack(game, *monsterClass, texturePacks);
			}
			else if (texturePacks.IsArray() == true)
			{
				for (const auto& val : texturePacks)
				{
					if (val.IsString() == true)
					{
						parseMonsterClassTexturePack(game, *monsterClass, val);
					}
				}
			}
		}

		if (elem.HasMember("textureIndexes") == true)
		{
			const auto& textureIndexes = elem["textureIndexes"];

			if (textureIndexes.IsObject() == true)
			{
				parseMonsterClassTexturePackIndex(*monsterClass, textureIndexes);
			}
			else if (textureIndexes.IsArray() == true)
			{
				for (const auto& val : textureIndexes)
				{
					parseMonsterClassTexturePackIndex(*monsterClass, val);
				}
			}
		}

		if (elem.HasMember("anchorOffset") == true)
		{
			monsterClass->setAnchorOffset(
				getVector2fVal<sf::Vector2f>(elem["anchorOffset"])
			);
		}
		if (elem.HasMember("name") == true)
		{
			monsterClass->Name(getStringVal(elem["name"]));
		}
		if (elem.HasMember("type") == true)
		{
			monsterClass->Type(getStringVal(elem["type"]));
		}

		if (elem.HasMember("nameClassifier") == true)
		{
			monsterClass->setNameClassifier(
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
						monsterClass->setDefaultByHash(nameHash,
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
							action = monsterClass->getAction(str2int16(getStringViewVal(it->value)));
						}
						if (action == nullptr)
						{
							action = parseAction(game, it->value);
						}
						monsterClass->setAction(str2int16(getStringViewVal(it->name)), action);
					}
				}
			}
		}

		if (elem.HasMember("descriptions") == true)
		{
			const auto& descriptions = elem["descriptions"];
			if (descriptions.IsObject() == true)
			{
				parseDescriptionValue(*monsterClass, *level, descriptions);
			}
			else if (descriptions.IsArray() == true)
			{
				for (const auto& val : descriptions)
				{
					parseDescriptionValue(*monsterClass, *level, val);
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
						monsterClass->setFormula(nameHash, getStringVal(it->value));
					}
				}
			}
		}

		if (elem.HasMember("animationSpeeds") == true)
		{
			const auto& speeds = elem["animationSpeeds"];
			if (speeds.IsObject() == true)
			{
				parseMonsterAnimationSpeed(*monsterClass, speeds);
			}
			else if (speeds.IsArray() == true)
			{
				for (const auto& val : speeds)
				{
					parseMonsterAnimationSpeed(*monsterClass, val);
				}
			}
		}

		if (elem.HasMember("sounds") == true)
		{
			const auto& sounds = elem["sounds"];
			if (sounds.IsObject() == true)
			{
				parseMonsterSound(game, *monsterClass, sounds);
			}
			else if (sounds.IsArray() == true)
			{
				for (const auto& val : sounds)
				{
					parseMonsterSound(game, *monsterClass, val);
				}
			}
		}

		if (elem.HasMember("attackSound") == true)
		{
			monsterClass->setAttackSound((int16_t)getIntVal(elem["attackSound"], -1));
		}
		if (elem.HasMember("defendSound") == true)
		{
			monsterClass->setDefendSound((int16_t)getIntVal(elem["defendSound"], -1));
		}
		if (elem.HasMember("dieSound") == true)
		{
			monsterClass->setDieSound((int16_t)getIntVal(elem["dieSound"], -1));
		}
		if (elem.HasMember("hitSound") == true)
		{
			monsterClass->setHitSound((int16_t)getIntVal(elem["hitSound"], -1));
		}
		if (elem.HasMember("walkSound") == true)
		{
			monsterClass->setWalkSound((int16_t)getIntVal(elem["walkSound"], -1));
		}
		if (elem.HasMember("maxResistMagic") == true)
		{
			monsterClass->MaxResistMagic(getIntVal(elem["maxResistMagic"], 100));
		}
		if (elem.HasMember("maxResistFire") == true)
		{
			monsterClass->MaxResistFire(getIntVal(elem["maxResistFire"], 100));
		}
		if (elem.HasMember("maxResistLightning") == true)
		{
			monsterClass->MaxResistLightning(getIntVal(elem["maxResistLightning"], 100));
		}

		if (elem.HasMember("outline") == true)
		{
			monsterClass->Outline(getColorVal(elem["outline"], sf::Color::Transparent));
		}
		if (elem.HasMember("outlineIgnore") == true)
		{
			monsterClass->OutlineIgnore(getColorVal(elem["outlineIgnore"], sf::Color::Transparent));
		}
		if (elem.HasMember("light") == true)
		{
			monsterClass->setLightSource(
				getLightSourceVal(elem["light"], { 255, 10 })
			);
		}
	}
}
