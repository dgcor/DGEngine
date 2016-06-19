#include "ParsePlayer.h"
#include "ParseAction.h"
#include "ParseUtils.h"
#include "Game/Player.h"

namespace Parser
{
	using namespace rapidjson;

	void parsePlayer(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false
			|| isValidString(elem, "celTexture") == false)
		{
			return;
		}

		auto level = game.Resources().getLevel(getString(elem, "idLevel"));
		if (level == nullptr)
		{
			return;
		}

		auto mapPos = getVector2i<sf::Vector2i>(elem, "mapPosition");
		auto mapSize = level->Map().Size();
		if (mapPos.x >= mapSize.x || mapPos.y >= mapSize.y)
		{
			return;
		}
		auto& mapCell = level->Map()[mapPos.x][mapPos.y];

		if (mapCell.drawable != nullptr)
		{
			return;
		}

		auto celTexture = game.Resources().getCelTextureCache(elem["celTexture"].GetString());
		if (celTexture == nullptr)
		{
			return;
		}

		auto player = std::make_shared<Player>(celTexture);

		player->MapPosition(mapPos);
		mapCell.drawable = player.get();

		player->setCelIndex(getUInt(elem, "celIndex", 1) - 1);
		player->setNumFrames(getUInt(elem, "frames", 1));
		player->setFrameIndex(getUInt(elem, "frameIndex", 1));

		player->Id(elem["id"].GetString());
		player->Name(getString(elem, "name"));
		player->Class(getString(elem, "class"));
		player->Level_(getInt(elem, "level"));
		player->Experience(getInt(elem, "experience"));
		player->ExpNextLevel(getInt(elem, "expNextLevel"));
		player->Points(getInt(elem, "points"));
		player->Gold(getInt(elem, "gold"));
		auto strength = getInt(elem, "strengthBase");
		player->StrengthBase(strength);
		player->StrengthNow(getInt(elem, "strengthNow", strength));
		auto magic = getInt(elem, "magicBase");
		player->MagicBase(magic);
		player->MagicNow(getInt(elem, "magicNow", magic));
		auto dexterity = getInt(elem, "dexterityBase");
		player->DexterityBase(dexterity);
		player->DexterityNow(getInt(elem, "dexterityNow", dexterity));
		auto vitality = getInt(elem, "vitalityBase");
		player->VitalityBase(vitality);
		player->VitalityNow(getInt(elem, "vitalityNow", vitality));
		auto life = getInt(elem, "lifeBase");
		player->LifeBase(life);
		player->LifeNow(getInt(elem, "lifeNow", life));
		auto mana = getInt(elem, "manaBase");
		player->ManaBase(mana);
		player->ManaNow(getInt(elem, "manaNow", mana));
		player->ArmorClass(getInt(elem, "armorClass"));
		player->ToHit(getInt(elem, "toHit"));
		player->DamageMin(getInt(elem, "damageMin"));
		player->DamageMax(getInt(elem, "damageMax"));
		player->ResistMagic(getInt(elem, "resistMagic"));
		player->ResistFire(getInt(elem, "resistFire"));
		player->ResistLightning(getInt(elem, "resistLightning"));

		if (elem.HasMember("onClick") == true)
		{
			player->setClickAction(parseAction(game, elem["onClick"]));
		}

		if (getBool(elem, "mainPlayer") == true)
		{
			level->setMainPlayer(player);
		}
		else
		{
			level->addPlayer(player);
		}

		if (getBool(elem, "currentPlayer") == true)
		{
			level->setCurrentPlayer(player);
		}
	}
}
