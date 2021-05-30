#include "ParseLevelObjectClass.h"
#include "Game.h"
#include "Game/Level.h"
#include "Game/SimpleLevelObjectClass.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::unique_ptr<SimpleLevelObjectClass> parseLevelObjectClassHelper(const Game& game,
		const Level& level, const Value& elem, std::string& id)
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
		if (level.hasClass(id) == true)
		{
			return nullptr;
		}

		if (isValidString(elem, "fromId") == true)
		{
			auto fromId = elem["fromId"sv].GetStringView();
			if (fromId != id)
			{
				auto obj = level.getClass<SimpleLevelObjectClass>(fromId);
				if (obj == nullptr)
				{
					return nullptr;
				}
				return std::make_unique<SimpleLevelObjectClass>(*obj);
			}
		}

		std::unique_ptr<SimpleLevelObjectClass> levelObjClass;

		if (isValidString(elem, "texture") == true)
		{
			auto texture = game.Resources().getTexture(elem["texture"sv].GetStringView());
			if (texture == nullptr)
			{
				return nullptr;
			}
			levelObjClass = std::make_unique<SimpleLevelObjectClass>(*texture);

			if (elem.HasMember("textureRect"sv) == true)
			{
				auto texSize = texture->getSize();
				sf::IntRect rect(0, 0, (int)texSize.x, (int)texSize.y);
				levelObjClass->setTextureRect(getIntRectKey(elem, "textureRect", rect));
			}
		}
		else if (isValidString(elem, "texturePack") == true)
		{
			auto texPack = game.Resources().getTexturePack(elem["texturePack"sv].GetStringView());
			if (texPack == nullptr)
			{
				return nullptr;
			}
			auto frames = std::make_pair(0u, texPack->size() - 1);
			frames = getFramesKey(elem, "frames", frames);
			auto refresh = getTimeKey(elem, "refresh", sf::milliseconds(50));
			levelObjClass = std::make_unique<SimpleLevelObjectClass>(
				texPack, frames, refresh, AnimationType::Looped);
		}
		return levelObjClass;
	}

	void parseDescriptionValue(SimpleLevelObjectClass& levelObjClass,
		const Level& level, const Value& elem)
	{
		levelObjClass.setDescription(
			getUIntKey(elem, "index"),
			level.getClassifier(getStringViewKey(elem, "name")),
			(uint16_t)getUIntKey(elem, "skip"));
	}

	void parseLevelObjectClass(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel<Level>(getStringViewKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		std::string id;
		auto levelObjClass = parseLevelObjectClassHelper(game, *level, elem, id);
		if (levelObjClass == nullptr)
		{
			return;
		}

		levelObjClass->Id(id);

		if (elem.HasMember("anchorOffset"sv) == true)
		{
			levelObjClass->setAnchorOffset(
				getVector2fVal<sf::Vector2f>(elem["anchorOffset"sv])
			);
		}
		if (elem.HasMember("name"sv) == true)
		{
			levelObjClass->Name(getStringViewVal(elem["name"sv]));
		}
		if (elem.HasMember("text1"sv) == true)
		{
			levelObjClass->Text1(getStringViewVal(elem["text1"sv]));
		}
		if (elem.HasMember("text2"sv) == true)
		{
			levelObjClass->Text2(getStringViewVal(elem["text2"sv]));
		}
		if (elem.HasMember("size"sv) == true)
		{
			levelObjClass->setCellSize(getVector2iVal<PairInt8>(elem["size"sv]));
		}
		if (elem.HasMember("nameClassifier"sv) == true)
		{
			levelObjClass->setNameClassifier(
				level->getClassifier(getStringViewKey(elem, "nameClassifier")));
		}

		if (elem.HasMember("defaults"sv) == true)
		{
			const auto& defaults = elem["defaults"sv];
			if (defaults.IsObject() == true)
			{
				for (auto it = defaults.MemberBegin(); it != defaults.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						auto name = it->name.GetStringView();
						auto nameHash = str2int16(name);
						level->setPropertyName(nameHash, name);
						levelObjClass->setDefaultByHash(nameHash,
							getMinMaxIntVal<LevelObjValue>(it->value));
					}
				}
			}
		}

		if (elem.HasMember("actions"sv) == true)
		{
			const auto& actions = elem["actions"sv];
			if (actions.IsObject() == true)
			{
				for (auto it = actions.MemberBegin(); it != actions.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						std::shared_ptr<Action> action;
						if (it->value.IsString() == true)
						{
							action = levelObjClass->getAction(str2int16(getStringViewVal(it->value)));
						}
						if (action == nullptr)
						{
							action = getActionVal(game, it->value);
						}
						levelObjClass->setAction(str2int16(getStringViewVal(it->name)), action);
					}
				}
			}
		}

		if (elem.HasMember("descriptions"sv) == true)
		{
			const auto& descriptions = elem["descriptions"sv];
			if (descriptions.IsObject() == true)
			{
				parseDescriptionValue(*levelObjClass, *level, descriptions);
			}
			else if (descriptions.IsArray() == true)
			{
				for (const auto& val : descriptions)
				{
					parseDescriptionValue(*levelObjClass, *level, val);
				}
			}
		}

		level->addClass(id, std::move(levelObjClass));
	}
}
