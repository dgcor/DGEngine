#include "ParsePlayer.h"
#include "Game/Player.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parsePlayer(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false
			|| isValidString(elem, "class") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}

		auto level = game.Resources().getLevel(getStringKey(elem, "idLevel"));
		if (level == nullptr)
		{
			return;
		}

		auto mapPos = getVector2uKey<MapCoord>(elem, "mapPosition");
		auto mapSize = level->Map().MapSize();
		if (mapPos.x >= mapSize.x || mapPos.y >= mapSize.y)
		{
			return;
		}
		auto& mapCell = level->Map()[mapPos.x][mapPos.y];

		if (mapCell.object != nullptr)
		{
			return;
		}

		auto playerClass = level->getPlayerClass(elem["class"].GetString());
		if (playerClass == nullptr)
		{
			return;
		}

		auto player = std::make_shared<Player>(playerClass);

		player->MapPosition(mapPos);
		mapCell.object = player;

		player->setDirection(getPlayerDirectionKey(elem, "direction"));
		player->setStatus(getPlayerStatusKey(elem, "status"));
		player->setPalette(getUIntKey(elem, "palette"));

		player->Id(id);
		player->Name(getStringKey(elem, "name"));
		player->Level_(getIntKey(elem, "level"));
		player->Experience(getIntKey(elem, "experience"));
		player->ExpNextLevel(getIntKey(elem, "expNextLevel"));
		player->Points(getIntKey(elem, "points"));
		player->Gold(getIntKey(elem, "gold"));
		auto strength = getIntKey(elem, "strengthBase");
		player->StrengthBase(strength);
		player->StrengthNow(getIntKey(elem, "strengthNow", strength));
		auto magic = getIntKey(elem, "magicBase");
		player->MagicBase(magic);
		player->MagicNow(getIntKey(elem, "magicNow", magic));
		auto dexterity = getIntKey(elem, "dexterityBase");
		player->DexterityBase(dexterity);
		player->DexterityNow(getIntKey(elem, "dexterityNow", dexterity));
		auto vitality = getIntKey(elem, "vitalityBase");
		player->VitalityBase(vitality);
		player->VitalityNow(getIntKey(elem, "vitalityNow", vitality));
		auto life = getIntKey(elem, "lifeBase");
		player->LifeBase(life);
		player->LifeNow(getIntKey(elem, "lifeNow", life));
		auto mana = getIntKey(elem, "manaBase");
		player->ManaBase(mana);
		player->ManaNow(getIntKey(elem, "manaNow", mana));
		player->ArmorClass(getIntKey(elem, "armorClass"));
		player->ToHit(getIntKey(elem, "toHit"));
		player->DamageMin(getIntKey(elem, "damageMin"));
		player->DamageMax(getIntKey(elem, "damageMax"));
		player->ResistMagic(getIntKey(elem, "resistMagic"));
		player->ResistFire(getIntKey(elem, "resistFire"));
		player->ResistLightning(getIntKey(elem, "resistLightning"));

		if (elem.HasMember("action") == true)
		{
			player->setAction(parseAction(game, elem["action"]));
		}

		level->addPlayer(player);

		if (getBoolKey(elem, "currentPlayer") == true)
		{
			level->setCurrentPlayer(player.get());
		}
	}
}
