#include "ParsePlayer.h"
#include "Game/Game.h"
#include "Game/Level/Level.h"
#include "Game/Player/Player.h"
#include "Game/Utils/GameUtils.h"
#include "ParsePlayerInventories.h"
#include "ParsePlayerSpells.h"
#include "Parser/Item/ParseItem.h"
#include "Parser/LevelObject/ParseLevelObjectCommon.h"
#include "Parser/Utils/ParseUtils.h"
#include "Parser/Utils/ParseUtilsGameKey.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	void parsePlayer(Game& game, const Value& elem)
	{
		if (isValidString(elem, "class") == false)
		{
			return;
		}

		auto level = game.Resources().getLevel<Level>(getStringViewKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}

		auto mapPos = getVector2UnsignedNumberKey<PairFloat, float>(elem, "mapPosition");
		if (level->Map().isMapCoordValid(mapPos) == false)
		{
			return;
		}
		auto& mapCell = level->Map()[mapPos];

		if (mapCell.getObject<Player>() != nullptr)
		{
			return;
		}

		auto class_ = level->LevelObjects().getClass<PlayerClass>(elem["class"sv].GetStringView());

		if (class_ == nullptr ||
			class_->hasTextures() == false)
		{
			return;
		}

		auto id = getStringViewKey(elem, "id");
		if (isValidId(id) == false)
		{
			id = {};
		}
		if (id.empty() == false && level->LevelObjects().get(id) != nullptr)
		{
			return;
		}

		auto player = std::make_shared<Player>(class_, *level);

		player->Id(id);

		player->Hoverable(getBoolKey(elem, "enableHover", true));

		player->setDirection(getPlayerDirectionKey(elem, "direction"));
		player->setAnimation(getPlayerAnimationKey(elem, "animation"));
		player->setTextureIdx(getUIntKey(elem, "textureIndex"));
		player->setStatus(getPlayerStatusKey(elem, "status"));
		player->setRestStatus((uint8_t)getUIntKey(elem, "restStatus"));

		auto outline = getColorKey(elem, "outline", class_->Outline());
		auto outlineIgnore = getColorKey(elem, "outlineIgnore", class_->OutlineIgnore());
		player->setOutline(outline, outlineIgnore);
		player->setOutlineOnHover(getBoolKey(elem, "outlineOnHover", true));

		player->AIType(getIntKey(elem, "AI"));

		player->Name(getStringViewKey(elem, "name"));

		if (parseLevelObjectProperties(*player, *level, elem) == true)
		{
			player->updateLevelFromExperience(*level, false);
		}

		parsePlayerInventories(*player, *level, elem);
		parsePlayerSpells(*player, *level, elem);

		player->MapPosition(level->Map(), mapPos);
		player->updateProperties();
		player->updateTexture();

		if (getBoolKey(elem, "currentPlayer") == true)
		{
			level->setCurrentPlayer(player);
		}

		level->addLevelObject(std::move(player));
	}
}
