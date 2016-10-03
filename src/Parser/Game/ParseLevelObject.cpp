#include "ParseLevelObject.h"
#include "Parser/ParseAction.h"
#include "Parser/ParseUtils.h"
#include "Game/ImageLevelObject.h"

namespace Parser
{
	using namespace rapidjson;

	void parseLevelObject(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false
			|| isValidString(elem, "texture") == false)
		{
			return;
		}

		auto level = game.Resources().getLevel(getStringKey(elem, "idLevel"));
		if (level == nullptr)
		{
			return;
		}

		auto mapPos = getVector2iKey<sf::Vector2i>(elem, "mapPosition");
		auto mapSize = level->Map().Size();
		if (mapPos.x >= mapSize.x || mapPos.y >= mapSize.y)
		{
			return;
		}
		auto& mapCell = level->Map()[mapPos.x][mapPos.y];

		if (mapCell.object != nullptr)
		{
			return;
		}

		auto texture = game.Resources().getTexture(elem["texture"].GetString());
		if (texture == nullptr)
		{
			return;
		}

		auto levelObj = std::make_shared<ImageLevelObject>(*texture);

		levelObj->MapPosition(mapPos);
		mapCell.object = levelObj;

		if (elem.HasMember("textureRect"))
		{
			sf::IntRect rect(0, 0, 32, 32);
			levelObj->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}

		levelObj->Id(elem["id"].GetString());
		levelObj->Name(getStringKey(elem, "name"));

		if (elem.HasMember("action") == true)
		{
			levelObj->setAction(parseAction(game, elem["action"]));
		}

		level->addLevelObject(levelObj);
	}
}
